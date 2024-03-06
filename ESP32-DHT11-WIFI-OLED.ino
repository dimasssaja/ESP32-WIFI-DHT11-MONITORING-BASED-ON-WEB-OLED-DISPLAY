#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels

const char *ssid = "Change this with your SSID WIFI";
const char *password = "Change this with your Password Wifi";

// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int DHT_Pin = 18;                           
float temperature ;                             
float  humidity;
float kelvin;
float fahrenheit;

WebServer server(80);
DHT dht(18, DHT11);

void handleRoot() {
  char msg[1500];

  snprintf(msg, 1500,
"<html>\
  <head>\
    <meta http-equiv='refresh' content='4'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.7.2/css/all.css' integrity='sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr' crossorigin='anonymous'>\
    <title>ESP32 DHT Server</title>\
    <style>\
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center;}\
    h2 { font-size: 3.0rem; }\
    p { font-size: 3.0rem; }\
    .units { font-size: 1.2rem; }\
    .dht-labels{ font-size: 1.5rem; vertical-align:middle; padding-bottom: 15px;}\
    </style>\
  </head>\
  <body>\
      <h2>ESP32 DHT Monitoring Server & OLED Display </h2>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Temperature</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;C</sup>\
      </p>\
      <p>\
        <i class='fas fa-tint' style='color:#00add6;'></i>\
        <span class='dht-labels'>Humidity</span>\
        <span>%.2f</span>\
        <sup class='units'>&percnt;</sup>\
      </p>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Fahrenheit</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;F</sup>\
      </p>\
      <p>\
        <i class='fas fa-thermometer-half' style='color:#ca3517;'></i>\
        <span class='dht-labels'>Kelvin</span>\
        <span>%.2f</span>\
        <sup class='units'>&deg;K</sup>\
      </p>\
  </body>\
</html>",
           readDHTTemperature(), readDHTHumidity(), readFahrenheit(), readKelvin()
          );
  server.send(200, "text/html", msg);
}
   
void setup() {
  Serial.begin(115200);
  dht.begin();
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }
  Serial.println(("Projek Membuat Sensor suhu dan menampilkannya ke layar OLED Dan Web Server Wifi"));
  Serial.println(("By Dimas Abi Mesti"));
  delay(2000);         
  oled.clearDisplay(); 
  oled.setTextSize(1);         
  oled.setTextColor(WHITE);    
  oled.setCursor(0, 2);       
  oled.display();      
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}
           
void loop() {
  server.handleClient();
  delay(2);
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  fahrenheit=(temperature*1.8)+32;
  kelvin=temperature+273.15;

  oled.clearDisplay();
  oled.setCursor(0, 0);

  oled.setCursor(0, 12);
  oled.print("Celsius ");
  oled.println("Humidity ");
  oled.print(String(dht.readTemperature(false)));
  oled.println("     "+String(humidity));
  oled.println("Fahrenheit");
  oled.println(fahrenheit);
  oled.println("Kelvin");
  oled.println(kelvin);

  oled.setCursor(0, 24);

  oled.display();

  delay(1000);
}

float readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Gagal Mendapatkan Data Dari DHT 11 SENSOR");
    return -1;
  }
  else {
    Serial.println(t);
    return t;
  }
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Gagal Mendapatkan Data Dari DHT 11 SENSOR");
    return -1;
  }
  else {
    Serial.println(h);
    return h;
  }
}
float readFahrenheit(){
  float temperature = dht.readTemperature();
  fahrenheit=(temperature*1.8)+32;
  if (isnan(temperature)) {    
    Serial.println("Gagal Mendapatkan Data Dari DHT 11 SENSOR");
    return -1;
  }
  else {
    Serial.println(fahrenheit);
    return fahrenheit;
  } 
}
float readKelvin(){
  float temperature1 = dht.readTemperature();
  kelvin=temperature1+273.15;
  if (isnan(temperature1)) {    
    Serial.println("Gagal Mendapatkan Data Dari DHT 11 SENSOR");
    return -1;
  }
  else {
    Serial.println(kelvin);
    return kelvin;
  } 
}
