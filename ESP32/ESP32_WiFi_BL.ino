//Librarys
#include "BluetoothSerial.h"
#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "time.h"

//BL
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

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

//NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 3600;


// variable para entender de donde proviene la información
String action;
char dato;
char Vlock;
//variables para dividir la información por "/"
int contador = 0;
String MensajeTemp;
char Trash;
// variables de VLockapp
String action_door;
String Input_password;
//Firebase
String rutanodo = "";

void setup() {
  Serial.begin(115200);
  SerialBT.begin("VLOCK"); //Bluetooth device name
}

void loop() {
  if (SerialBT.available()) {
    dato = SerialBT.read();
    if (dato == '/') {
      if (contador == 0) {
        action = MensajeTemp; //action tendra el valor de donde proviene la info, 9 = VLockAPP
      }
      else if (contador == 1 && action == "9") {
        action_door = MensajeTemp;
      }
      else if (contador == 2 && action == "9") {
        Input_password = MensajeTemp;
      }
      contador += 1;
      MensajeTemp = "";
      if (contador == 3) {
        contador = 0;
        action = "";
        Serial.print("p/" + action_door + "/" + Input_password + "/");
        //Check privileges in DB
        SerialBT.end();
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
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) { //owners
          sendDataPrevMillis = millis();
          rutanodo = "/owners/";
          rutanodo += Input_password;
          if (Firebase.RTDB.getBool(&fbdo, rutanodo)) {
            if (fbdo.dataType() == "boolean") {
              Serial.print("f/true/");
              WiFi.mode(WIFI_OFF);
              SerialBT.begin("VLOCK");
            }
          } else { // admins
            rutanodo = "/admins/";
            rutanodo += Input_password;
            if (Firebase.RTDB.getBool(&fbdo, rutanodo)) {
              if (fbdo.dataType() == "boolean") {
                Serial.print("f/true/");
                WiFi.mode(WIFI_OFF);
                SerialBT.begin("VLOCK");
              }
            } else { //users
              rutanodo = "/users/";
              rutanodo += Input_password;
              if (Firebase.RTDB.getArray(&fbdo, rutanodo)) {
                if (fbdo.dataType() == "array") {
                  FirebaseJsonArray &arr = fbdo.jsonArray();
                  FirebaseJsonData result;
                  struct tm timeinfo;
                  if (!getLocalTime(&timeinfo)) {
                    Serial.println("Failed to obtain time");
                  }
                  char timeWeekDay[10];
                  strftime(timeWeekDay, 10, "%a", &timeinfo);
                  if (strcmp(timeWeekDay, "Mon") == 0) {
                    arr.get(result, 0);
                  } else if (strcmp(timeWeekDay, "Tue") == 0) {
                    arr.get(result, 1);
                  } else if (strcmp(timeWeekDay, "Wed") == 0) {
                    arr.get(result, 2);
                  } else if (strcmp(timeWeekDay, "Thu") == 0) {
                    arr.get(result, 3);
                  } else if (strcmp(timeWeekDay, "Fri") == 0) {
                    arr.get(result, 4);
                  } else if (strcmp(timeWeekDay, "Sat") == 0) {
                    arr.get(result, 5);
                  } else if (strcmp(timeWeekDay, "Sun") == 0) {
                    arr.get(result, 6);
                  }
                  if (result.to<String>() == "true") {
                    Serial.print("f/true/");
                    WiFi.mode(WIFI_OFF);
                    SerialBT.begin("VLOCK");
                  }
                }
              } else {
                WiFi.mode(WIFI_OFF);
                SerialBT.begin("VLOCK");
              }
            }
          }
        }
      }
    } else {
      MensajeTemp += dato;
    }

  }
}
