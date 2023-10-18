/*
Revisado el 17-10-23
Desarrollo Ingenieria Higher Power EIRL
Autor Oscar Huenulef Cardenas
*/

//---------------------------------- LIBRERIAS -------------------------------------
#include <HardwareSerial.h>  //Ayuda a configurar I/O pins para comunicacion serial
#include "mqttService.h"

//-------------- HEARTBEAT - Envío del estado del sistema --------------------------
void estadoDispositivo() {
  if ((Minutes == 30) || (Minutes == 0)) {
    if (heartBeat == 1) {
      Serial.print("Enviando Estado\n");
      Enviar(MQTT_ROUTING_KEY2, MQTT_CLIENT, timeEpoch, wifiSignal);
    }
  } else if (deviceState == "Porton de entrada") {
    Serial.println(deviceState);
    Enviar(MQTT_ROUTING_KEY5, MQTT_CLIENT, timeEpoch, wifiSignal);
  } else if (deviceState == "Porton de salida") {
    Serial.println(deviceState);
    Enviar(MQTT_ROUTING_KEY6, MQTT_CLIENT, timeEpoch, wifiSignal);
  } else {
    heartBeat = 1;
  }
}

//---------------------------------- SETUP ----------------------------------------
void setup() {
  Serial.begin(115200);
  //----------------RELAYS-------------------------------------
  pinMode(START_1, OUTPUT);
  digitalWrite(START_1, HIGH);
  pinMode(START_2, OUTPUT);
  digitalWrite(START_2, HIGH);
  pinMode(STOP_1, OUTPUT);
  digitalWrite(STOP_1, HIGH);
  pinMode(STOP_2, OUTPUT);
  digitalWrite(STOP_2, HIGH);
  //-----------------INPUTS-----------------------------------
  pinMode(Fotocelda_1, INPUT_PULLUP);
  pinMode(Fotocelda_2, INPUT_PULLUP);
  pinMode(Fotocelda_3, INPUT_PULLUP);
  pinMode(Fotocelda_4, INPUT_PULLUP);
  pinMode(puertoRTU, INPUT_PULLUP);
  //--------------- CONFIGURACION FECHA Y HORA --------------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //----------------- CONFIGURACION & CONEXION ----------------
  WiFi.macAddress().toCharArray(MQTT_CLIENT, WiFi.macAddress().length() + 1);
  MQTT_TOPIC_SUBSCRIBE1 = String(MQTT_CLIENT);  //Genera el nombre para suscribir el servicio
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);       //Configuracion de servidor MQTT
  mqtt.setCallback(callback_mqtt);              //Configura Callback
  setup_mqtt();                                 //Conexion
  //-------------REINICIO------------------------------------
  RazonReinicio = String(esp_reset_reason());
  Serial.print("Razon de reinicio: " + RazonReinicio + "\n");
}

//------------------------------------ LOOP -----------------------------------------
void loop() {
  tiempo = millis();    //tiempo de reloj interno
  setup_mqtt();         //Conexion a WiFi y servicio MQTT
  LocalTime();          //Obtiene la fecha actual
  estadoDispositivo();  //heartbeat
  redPill_loop();       //Logica de activaciones
  mqtt.loop();          //Loop del servicio MQTT
}
