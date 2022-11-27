 //Librarys
#include "BluetoothSerial.h"
#include <Arduino.h>

//BL
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

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

 void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("VLOCK"); //Bluetooth device name
}

void loop() {
  // put your main code here, to run repeatedly:
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
      }
    } else {
      MensajeTemp += dato;
      }
}}
