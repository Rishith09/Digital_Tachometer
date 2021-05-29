#include <ESP8266TimerInterrupt.h>
#include <ESP8266_ISR_Timer.h>
#include <ArduinoJson.h>
#include <Firebase.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>
#define FIREBASE_HOST "tachometer-80111.firebaseio.com"
#define WIFI_SSID "YOUR WIFI NAME"
#define WIFI_PASSWORD "YOUR WIFI PASSWORD"
int IRSensorPin = 2; // the number of the IR sensor input pin
int inputState; // the current state from the input pin
int lastInputState = LOW; // the previous InputState from the input pin
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 5; // the debounce time; increase if the output flickers
long Time;
long endTime;
long startTime;
int RPM = 0;
float lnTime = 0;
void setup() {
// put your setup code here, to run once:
pinMode(IRSensorPin, INPUT);
delay(5000);
endTime = 0;
attachInterrupt(digitalPinToInterrupt(IRSensorPin), timerIsr, RISING); // Attach
the service routine here
{
Serial.begin(115200);
Serial.print("Connecting to ");
Serial.println(WIFI_SSID);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
Firebase.begin(FIREBASE_HOST);
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
}
}
void timerIsr()
{
Time = millis() / 1000;
}
void loop() {
Time = millis();
int currentSwitchState = digitalRead(IRSensorPin);
if (currentSwitchState != lastInputState) {
lastDebounceTime = millis();
}
if ((millis() - lastDebounceTime) > debounceDelay) {
if (currentSwitchState != inputState) {
inputState = currentSwitchState;
if (inputState == LOW) {
startTime = lastDebounceTime;
lnTime = startTime - endTime;
RPM = 60000 / ((startTime - endTime)*3);
Serial.print(Time);
Serial.print(" RPM: ");
Serial.println(RPM);
Firebase.setInt ("RPM",RPM);
delay(200);
endTime = startTime;
}
}
}
