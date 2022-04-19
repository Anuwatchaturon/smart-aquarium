#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <Time.h>
#include <Servo.h>
Servo myservo; 
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D14 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define echoPin2 D0// Echo Pin
#define trigPin2 D12 // Trigger 
long duration, distance; // Duration used to calculate distance

#include <Wire.h>
#include "Adafruit_TCS34725.h"
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

 
String gettime;
String check_auto;
String getauto;
String getautotime;
String servo_check;
String send_distance;
String send_detect_pump;
double temp;
float flow;

int list_data_rgb[3]; // r , g , b

#define flow_sensor D13

// light
int redpin = D2; // select the pin for the red LED
int bluepin = D5; // select the pin for the blue LED
int greenpin = D7 ;// select the pin for the green LED

String getqua_check;
String send_quality;


const char* ssid     = "DESKTOP-9GRKCEU"; //NAYNE //Kanokwan  //DESKTOP-9GRKCEU 7962
const char* password = "b348@77S"; //nayne38657 //Frogs2512 //b348@77S

//const char* host = "192.168.137.1";  //172.20.10.4 //192.168.1.100 //192.168.137.1
const char* host = "192.168.137.1"; 
void setup() {
  Serial.begin(9600);
//wifi connet


  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  sensors.begin();
//wifi manager
//WiFiManager wifiManager;
    //wifiManager.setTimeout(180);    
    //wifiManager.resetSettings();    
    //if(!wifiManager.autoConnect("Aquarium Sensor")) 
    //{
      //Serial.println("Failed to connect and hit timeout");
      //delay(3000);
      //ESP.reset();
      //delay(5000);
    //}
    //WiFi.printDiag(Serial);
    //Serial.println();
    //Serial.println("connected...OK");  

  myservo.attach(D9);
  pinMode(flow_sensor, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
    
    myservo.write(1);

    
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  }
  
  }

void loop() {

  // getr //
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readr.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          list_data_rgb[0]  = payload.toInt();
          
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
  }
  // getg //
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readg.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
           list_data_rgb[1]  = payload.toInt();
         
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
  }

    // getb //
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readb.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          list_data_rgb[2]  = payload.toInt();
          
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
  }


  // getquality //
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readqualitycheck.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          getqua_check  = payload;
          
 if(getqua_check!="0"){
WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url47 = "/project/sendquality.php?quality="; 
  url47 += send_quality;             
  Serial.print("Requesting URL: ");
  client.print(String("GET ") + url47 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout47 = millis();
  while (client.available() == 0) {
    if (millis() - timeout47 > 100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");  

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url55 = "/project/sendqualitycheck.php?qualitycheck="; 
  url55 += "0";             
  Serial.print("Requesting URL: ");
  client.print(String("GET ") + url55 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout55 = millis();
  while (client.available() == 0) {
    if (millis() - timeout55 > 100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");  
  
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
  }
  }

 
// servo auto
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readautofeed.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          String getauto = payload;
          Serial.println("getauto "+getauto);
 if (getauto !="0"){
        myservo.write(180); 
        delay(1000);
        myservo.write(1);
        delay(1000);
        Serial.println("feeding");
delay(1000);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url77 = "/project/sendautofeed.php?autofeed="; 
  url77 += "0";                        
  Serial.print("Requesting URL: ");
  client.print(String("GET ") + url77 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout77 = millis();
  while (client.available() == 0) {
    if (millis() - timeout77 > 100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");   
}
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
        
  }

  
// servo
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "http://192.168.137.1/project/readservo.php")) {  // HTTP
      int httpCode = http.GET();
      if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          String servo_check = payload;
          Serial.println("servo_check "+servo_check);
 if (servo_check !="0"){
        myservo.write(180); // สั่งให้ Servo ปิด
        delay(1000);
        myservo.write(1); // สั่งให้ Servo หมุนไปองศาที่ 0
        delay(1000);
        Serial.println("feeding");
delay(1000);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = "/project/sendservo.php?servo="; 
  url += "0";                          
  Serial.print("Requesting URL: ");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");   
}
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } 
  }

      analogWrite(redpin,   255-list_data_rgb[0] );
      analogWrite(bluepin,  255-list_data_rgb[2] );
      analogWrite(greenpin, 255-list_data_rgb[1] );
  
delay(1000);

 // water temp
sensors.requestTemperatures(); //สั่งอ่านค่าอุณหภูมิ
temp=(sensors.getTempCByIndex(0));
String tempx = String(temp);
Serial.println(temp);
delay(1000);

//water level
digitalWrite(trigPin2, LOW);
delayMicroseconds(5);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
unsigned int PulseWidth = pulseIn(echoPin2, HIGH);
unsigned int distance = PulseWidth * 0.0173681; 
if (distance < 18){
   send_distance = "normal";
  }else{
    send_distance = "danger";
    }
Serial.print("Distance is ");
Serial.print(distance);
Serial.println(" cm.");




//quality

  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
      if(lux > 100){
        send_quality ="Clean";
      }else{
        send_quality ="Dirty";
      }
  Serial.println(send_quality);
  Serial.println(" ");

// pump detect 1 turbidity
  int sensor = analogRead(A0);
  float volte = sensor*(5.0/1024.0);
  Serial.println(volte);
  delay(1000);
 // เหลือ เงื่อนไข quality

if (volte<2.5){
  send_detect_pump = "Work";
  }else{
    send_detect_pump = "Stop";
    }

// water-pump detect 2 water flow
//  uint32_t pulse = pulseIn(flow_sensor,HIGH); //xx
//  Serial.println(pulse); //xx
//  if(pulse < 1){ //xx
//    send_detect_pump = "Stop"; //xx
//    Serial.println("pump detect not work"+LOW); //xx
//  }else{ //xx
//    float Hz = 1/(2*pulse*pow(10,-6));
//    flow = 0.0137*(float)Hz + 8*pow(10,-15);
//String flowx;
//flowx=0.0137*Hz + 8*pow(10,-15);
   // send_detect_pump = "Work"; //xx
  //  Serial.println("pump working"); //xx
  // }  //xx

    delay(1000);
String datalist;
datalist= tempx+","+send_distance+","+"volte"+","+send_detect_pump+","+"(flow)" ;

   WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url5 = "/project/senddatax.php?datax="; 
  url5 += datalist;             
  Serial.print("Requesting URL: ");
  client.print(String("GET ") + url5 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout5 = millis();
  while (client.available() == 0) {
    if (millis() - timeout5 > 100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");

}// end loop







  
