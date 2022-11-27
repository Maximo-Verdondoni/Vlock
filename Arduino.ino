

// variable para entender de donde proviene la información
String action;
char dato;
//variables para dividir la información por "/"
int contador = 0;
int contadorf = 0;
String MensajeTemp;
// variables de VLockapp
String action_door;
String Input_password;

int SensorPin = 3;
int state;
int RelePin = 4;

String PassOff1 = "Off1";
String PassOff2 = "SJ2043";
String PassOff3 = "UsuarioOff";

void setup() {
  Serial.begin(115200);
  pinMode(SensorPin, INPUT_PULLUP);
  pinMode(RelePin, OUTPUT);
  digitalWrite(RelePin, HIGH);
}

void loop() {
  if (Serial.available()) {
    dato = Serial.read();
    if (dato == '/') {
      if (contador == 0) {
        action = MensajeTemp; //action tendra el valor de donde proviene la info, p = ESP32 manda p/action/password/
        }
      if (contador == 1 && action == "p") {
        action_door = MensajeTemp;
        }
      if (contador == 1 && action == "f") { //Abrir / cerrar la puerta
          contadorf = 1;
      }
      if (contador == 2 && action == "p") {
        Input_password = MensajeTemp;  
      }
      contador += 1;
      MensajeTemp = "";
      if (contador == 3) {
        contador = 0;
        action = "";
        Serial.println("El privilegio es: " + Input_password + " y la acción es: " + action_door);
        Serial.println("Action: " + action);
        Serial.println("Action_door: " + action_door);
        Serial.println("Password: " + Input_password);
        if (Input_password == PassOff1 || Input_password == PassOff2 || Input_password == PassOff3) { //Abrir / cerrar puerta usuarios offline
          Serial.print("Abriendo la puerta");
          digitalWrite(RelePin, LOW);
          state = 1;
          delay(3000);
          digitalWrite(RelePin, HIGH);
          state = 0;
        }
      } else if (contadorf == 1) {
          Serial.print("Abriendo la puerta");
          digitalWrite(RelePin, LOW);
          state = 1;
          delay(3000);
          digitalWrite(RelePin, HIGH);
          state = 0;
          MensajeTemp = "";
          contador = 0;
          action = "";
          contadorf = 0;
        }
    } else {
      MensajeTemp += dato;
      }
  }
}
