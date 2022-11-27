#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define API_KEY "YOUR_API_KEY"
#define DATABASE_URL "YOUR_RTDB_URL"
//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

//Firebase
String rutanodo = "";
int estado;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
  }
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    rutanodo = "/puerta/estado";
    if (Firebase.RTDB.getBool(&fbdo, rutanodo)) {
      if (fbdo.dataType() == "boolean") {
        estado = fbdo.boolData();
        if (estado == 1) {
            Serial.print("f/true/");
            Firebase.RTDB.setBool(&fbdo, rutanodo, false));
          }
      }
    }
  }
}
