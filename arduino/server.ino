#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <WiFiClient.h>

const char *ssid = "ssid";
const char *password = "password";

ESP8266WebServer server(80);

extern "C" {
  #include "user_interface.h"
}

ADC_MODE(ADC_VCC);

#define MOTOR_LEFT_PIN 4
#define MOTOR_RIGHT_PIN 5

Servo MotorLeftServo;
Servo MotorRightServo;

int freq_l = 1600;
int freq_r = 1600;

void handleNotFound() {
  String message = "";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.arg(0);
  message += "\n";

  for(uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

float per2ppm(float perc) {
  if(perc < 0 && perc >= -1) {
    return round(1600.0 - perc * 1600.0 * -1);
  }  else if(perc > 0 && perc <= 1) {
    return round(1600.0 + perc * 1600.0);
  } else {
    return 1600.0;
  }
}

void handleCtrl() {
  float perc_l = server.arg(0).toFloat();
  float perc_r = server.arg(1).toFloat();

  freq_l = (int) per2ppm(perc_l);
  freq_r = (int) per2ppm(perc_r*-1);

  server.send(200);
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.println("connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println ();
  Serial.print   ("Connected to ");
  Serial.println (ssid);
  Serial.print   ("IP address: ");
  Serial.println (WiFi.localIP());

  if(MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/motor", handleCtrl);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");


  MotorLeftServo.attach(MOTOR_LEFT_PIN);
  MotorRightServo.attach(MOTOR_RIGHT_PIN);

  Serial.println("Setup Finished");
}

void loop()
{
  server.handleClient();
  MotorLeftServo.writeMicroseconds(freq_l);
  MotorRightServo.writeMicroseconds(freq_r);
}
