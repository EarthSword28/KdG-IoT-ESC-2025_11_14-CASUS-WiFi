#include <Arduino.h>

#include <config.h>
#include <secrets.h>
#include <wifiPersonalLibrary.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

unsigned long printTimer = 0;
unsigned long currentMillis = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .card {background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
}
    </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  %JOKEPLACEHOLDER%
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var) {
  //Serial.println(var);
  if(var == "JOKEPLACEHOLDER") {
    String jokes = "";
    jokes += "<h4>Joke: </h4><label class=\"card\"><span class=\"joke\"></span></label>";
  }
}

void stopProgram() {
  while (1 == 1)
  {
    if (currentMillis >= printTimer) {
      printTimer = PRINT_DELAY + millis();
      Serial.println("Failed to reconnect. Check connection details and restart");
    }
  }
  
}

void setup() {
  Serial.begin(115200);

  // WiFi
  wifiInit(KNOWN_WIFI_AMOUNT, KNOWN_WIFI_SSIDs, KNOWN_WIFI_PASSWORDs, WIFI_CONNECT_ATTEMPS, WIFI_CONNECT_DELAY);
  wifiConnect(WIFI_HOST_NAME);
  WiFi.onEvent(onDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void loop() {
  currentMillis = millis();

  if (endProgram_global == HIGH) {
    stopProgram();
  }
}