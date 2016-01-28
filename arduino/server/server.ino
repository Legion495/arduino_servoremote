#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
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
bool state = false;

String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download"))
    return "application/octet-stream";
  else if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
    path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
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
  if(server.argName(0).equals("l") == true && server.argName(1).equals("r") == true) {
    float perc_l = server.arg(0).toFloat();
    float perc_r = server.arg(1).toFloat();

    freq_l = (int) per2ppm(perc_l);
    freq_r = (int) per2ppm(perc_r*-1);

    server.send(200);
  } else if(server.argName(0).equals("state") == true) {
    if(server.arg(0).equals("on") == true) {
      state = true;
      MotorLeftServo.attach(MOTOR_LEFT_PIN);
      MotorRightServo.attach(MOTOR_RIGHT_PIN);
      server.send(200);
    } else if(server.arg(0).equals("off") == true) {
      state = false;
      MotorLeftServo.detach();
      MotorRightServo.detach();
      server.send(200);
    } else { server.send(400); }
  } else { server.send(400); }
}

void setup()
{
  Serial.begin(115200);

  SPIFFS.begin();

  Dir dir = SPIFFS.openDir("/");

  Serial.println();

  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.println("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
  }

  Serial.println();

  WiFi.begin(ssid, password);

  Serial.println("\nconnecting");

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
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
    server.send(404, "text/plain", "FileNotFound");
  });
  server.begin();
  Serial.println("HTTP server started");

  Serial.println("Setup Finished");
}

void loop()
{
  server.handleClient();
  if(state == true) {
    MotorLeftServo.writeMicroseconds(freq_l);
    MotorRightServo.writeMicroseconds(freq_r);
  }
}
