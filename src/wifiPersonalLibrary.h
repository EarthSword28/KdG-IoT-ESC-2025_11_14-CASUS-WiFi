/*
  BRONNEN:
  ArduinoDocs, refresher listed strings (23/10/2025): https://docs.arduino.cc/language-reference/en/variables/data-types/string/
*/

#include <WiFi.h>

int wifiAmounts_PL = 0;   // the amount of known WiFi networks
char *knownWifiSsid_PL[10] = {"", "", "", "", "", "", "", "", "", ""};      // the known WiFi networks
char *knownWifiPassword_PL[10] = {"", "", "", "", "", "", "", "", "", ""};  // the passwords for the known WiFi networks
int wifiAttemps_PL = 5;   // amount of allowed attemps for device to connect to WiFi
int wifiDelay_PL = 500;   // how much time between attemps

String lastSsid_PL = "";
String lastPassword_PL = "";

boolean endProgram_global = LOW;

// Initiate the WiFi Personal Library
void wifiInit(int wifiAmounts_i_PL, char *knownWifiSsid_i_PL[], char *knownWifiPassword_i_PL[], int wifiAttemps_i_PL = 5, int wifiDelay_i_PL = 500, char* wifiHostName_i_PL = "") {
  wifiAmounts_PL = wifiAmounts_i_PL;
  if (wifiAmounts_PL > 0) {
    if (wifiAmounts_PL > 10) {
      Serial.println("WARNING: too many networks, only the first 10 will be saved");
    }
    for (int i = 0; i < wifiAmounts_PL && i < 10; i++) {
      knownWifiSsid_PL[i] = knownWifiSsid_i_PL[i];
      knownWifiPassword_PL[i] = knownWifiPassword_i_PL[i];
    }
    wifiAttemps_PL = wifiAttemps_i_PL;
    wifiDelay_PL = wifiDelay_i_PL;
  }
  else {
    Serial.println("ERROR: no networks given.");
  }
}

// Connect to a known WiFi network
void wifiConnect(const char* wifiHostName_PL = "", wifi_mode_t wifiMode_PL = WIFI_STA) {
  WiFi.mode(wifiMode_PL);
  int networks = WiFi.scanNetworks();
  if (networks > 0) {
    for (int i = 0; i < wifiAmounts_PL; i++) {
      Serial.println(knownWifiSsid_PL[i]);
      for (int n = 0; n < networks && WiFi.status() != WL_CONNECTED; n++) {
        String ssid = WiFi.SSID(n);
        if (ssid == "Hotspot-Jorden") {
          Serial.println("Hotspot-Jorden");
        }
        if (ssid == knownWifiSsid_PL[i]) {
          // if hostname, set hostname
          if (wifiHostName_PL != "") {
            WiFi.setHostname(wifiHostName_PL);
          }
          // connect to WiFi
          String password = knownWifiPassword_PL[i];
          int connectAttemps = 0;
          WiFi.begin(ssid, password);
          while (WiFi.status() != WL_CONNECTED && connectAttemps < wifiAttemps_PL) {
            connectAttemps++;
            delay(wifiDelay_PL);
            Serial.print("connecting to: ");
            Serial.print(ssid);
            Serial.print(" attempt: [");
            Serial.print(connectAttemps);
            Serial.print("/");
            Serial.print(wifiAttemps_PL);
            Serial.println("]");
          }
          if (WiFi.status() == WL_CONNECTED) {
            lastSsid_PL = ssid;
            lastPassword_PL = password;

            Serial.println("\nConnected to WiFi!");
            Serial.println(WiFi.localIP());
            break;
          }
          else {
            Serial.println("\nConnection Failed");
            continue;
          }
        }
      }
    }
  }
  else {
    Serial.println("No networks found");
  }
}

// Event-handler functies
void onDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi Disconnected, Attempting to Reconnect...");

  int connectAttempt = 0;
  WiFi.reconnect();
  while ((WiFi.status() != WL_CONNECTED) && (connectAttempt < wifiAttemps_PL)) {
    connectAttempt++;
    delay(wifiDelay_PL);
    Serial.print("reconnecting to: ");
    Serial.print(lastSsid_PL);
    Serial.print(" attempt: [");
    Serial.print(connectAttempt);
    Serial.print("/");
    Serial.print(wifiAttemps_PL);
    Serial.println("]");

    if (connectAttempt >= wifiAttemps_PL) {
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Reconnected");
  }
  else {
    endProgram_global = HIGH;
  }
}
