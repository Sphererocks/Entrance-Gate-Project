#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>
#include <U8g2lib.h>
#include <Adafruit_ADS1X15.h>
#include <AHTxx.h>
#include <time.h>

#define SDA_PIN 5
#define SCL_PIN 4
#define GATE_OPEN_PIN 2    // Clearly Changed to GPIO2 (D2)
#define GATE_CLOSED_PIN 0

const char* ssid = "SphereMako";
const char* password = "5phere7708";

WebServer server(80);
Adafruit_ADS1115 ads;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);
AHTxx aht20(AHTXX_ADDRESS_X38);

float currentAmps = 0.0, voltage = 0.0, power = 0.0;
float temperatureF = 0.0, humidity = 0.0;
String gateStatus = "Unknown";
unsigned long oledTimer = 0;
int displayPage = 0;

struct GateEvent {
  String eventType;
  String timestamp;
  float maxCurrent;
  float voltage;
};

GateEvent openLog[5], closeLog[5];
int openLogIndex = 0, closeLogIndex = 0;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;
const int daylightOffset_sec = 3600;

String getTimeStamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "NTP Error";
  char buffer[25];
  strftime(buffer,25,"%Y-%m-%d %H:%M:%S",&timeinfo);
  return String(buffer);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta http-equiv='refresh' content='5'><title>ESP32 Gate Monitor</title></head><body>";
  html += "<h2>Sensor Data</h2>";
  html += "Gate Status: " + gateStatus + "<br>";
  html += "Current: " + String(currentAmps,2) + "A<br>";
  html += "Voltage: " + String(voltage,2) + "V<br>";
  html += "Power: " + String(power,2) + "W<br>";
  html += "Temp: " + String(temperatureF,1) + "F<br>";
  html += "Humidity: " + String(humidity,1) + "%<br>";

  html += "<h3>Last 5 Gate Open Events</h3><ul>";
  for(int i=0;i<5;i++)if(openLog[i].timestamp!="")html+="<li>"+openLog[i].timestamp+" | "+String(openLog[i].maxCurrent,2)+"A | "+String(openLog[i].voltage,2)+"V</li>";
  html += "</ul><h3>Last 5 Gate Close Events</h3><ul>";
  for(int i=0;i<5;i++)if(closeLog[i].timestamp!="")html+="<li>"+closeLog[i].timestamp+" | "+String(closeLog[i].maxCurrent,2)+"A | "+String(closeLog[i].voltage,2)+"V</li>";
  html += "</ul></body></html>";
  server.send(200,"text/html",html);
}

void readSensors() {
  int16_t adc0 = ads.readADC_SingleEnded(0);
  int16_t adc1 = ads.readADC_SingleEnded(1);

  voltage = adc0 * 0.125 / 1000.0;
  currentAmps = adc1 * 0.125 / 1000.0;
  if(abs(currentAmps)<0.05)currentAmps=0;
  power = voltage * currentAmps;

  float tempC = aht20.readTemperature();
  humidity = aht20.readHumidity();
  temperatureF = isnan(tempC)?-999.0:(tempC*1.8+32);

  Serial.print("ADC0: "); Serial.print(adc0); Serial.print(" ADC1: "); Serial.println(adc1);
}

void checkGateStatus() {
  static bool lastOpen=HIGH,lastClose=HIGH;
  bool openNow=digitalRead(GATE_OPEN_PIN)==LOW;
  bool closeNow=digitalRead(GATE_CLOSED_PIN)==LOW;
  gateStatus=openNow?"OPEN":(closeNow?"CLOSED":"MOVING");

  if(openNow&&!lastOpen){
    openLog[openLogIndex++]={"Open",getTimeStamp(),currentAmps,voltage};
    openLogIndex%=5;
  }
  if(closeNow&&!lastClose){
    closeLog[closeLogIndex++]={"Close",getTimeStamp(),currentAmps,voltage};
    closeLogIndex%=5;
  }
  lastOpen=openNow; lastClose=closeNow;
}

void updateOLED() {
  if(millis()-oledTimer>3000){
    oledTimer=millis(); oled.clearBuffer(); displayPage=(displayPage+1)%3; oled.setFont(u8g2_font_ncenB08_tr);
    if(displayPage==0){
      oled.drawStr(0,10,"Gate Monitor");
      oled.drawStr(0,25,"Status:"); oled.drawStr(60,25,gateStatus.c_str());
      oled.drawStr(0,40,"Curr:"); oled.setCursor(60,40); oled.print(currentAmps,2);
      oled.drawStr(0,55,"Volt:"); oled.setCursor(60,55); oled.print(voltage,2);
    }else if(displayPage==1){
      oled.drawStr(0,10,"Power:"); oled.setCursor(70,10); oled.print(power,2);
      oled.drawStr(0,25,"Temp:"); oled.setCursor(70,25); oled.print(temperatureF,1);
      oled.drawStr(0,40,"Humidity:"); oled.setCursor(70,40); oled.print(humidity,1);
    }else{
      oled.drawStr(0,10,"IP:"); oled.setCursor(20,25); oled.print(WiFi.localIP());
      oled.drawStr(0,40,"Time:"); oled.setCursor(0,55); oled.print(getTimeStamp());
    }
    oled.sendBuffer();
  }
}

void setup(){
  Serial.begin(115200); Wire.begin(SDA_PIN,SCL_PIN);
  ads.begin(0x48); ads.setGain(GAIN_ONE); oled.begin(); aht20.begin();
  pinMode(GATE_OPEN_PIN,INPUT_PULLUP); pinMode(GATE_CLOSED_PIN,INPUT_PULLUP);
  WiFi.begin(ssid,password); while(WiFi.status()!=WL_CONNECTED)delay(500);
  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer); delay(1000);
  server.on("/",handleRoot); server.begin();
}

void loop(){ readSensors(); checkGateStatus(); updateOLED(); server.handleClient(); delay(10); }
