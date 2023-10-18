//------------- LIBRERIAS -----------------------------------
#include "Json.h"

//------------- VARIABLES -----------------------------------
String RazonReinicio = "";
String MQTT_TOPIC_SUBSCRIBE1 = "";
char MQTT_CLIENT[17];
unsigned long tOffline = 0;
//-------------FUNCION DE CONEXION---------------------------
void setup_mqtt() {
  //Comprueba conexion a WiFi
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  //Loop para intentar conectar al servidor
  else if (tiempo - tOffline >= 1000) {
    if (!mqtt.connected()) {
      Serial.print("MQTT: Intentando la conexion MQTT, MAC: ");
      Serial.println(MQTT_CLIENT);
      //Al conectar se suscribe al servicio
      if (mqtt.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASS)) {
        Serial.println("Conectado al servidor MQTT!");
        mqtt.subscribe(MQTT_TOPIC_SUBSCRIBE1.c_str());
        //Condicion de mensaje por reset
        if (RazonReinicio != "") {
          Enviar(MQTT_ROUTING_KEY3, MQTT_CLIENT, timeEpoch, wifiSignal);
          RazonReinicio = "";
        }
      } else {
        tOffline = tiempo;
      }
    }
  }
}