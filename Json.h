//-----------------LIBRERIAS -------------------------------------
#include <uuid.h>         //Generacion de uuid transaccion
#include <ArduinoJson.h>  //Utilización Json en arduino
#include "RedPill.h"
#include "macrosEEPROM.h"
#include "Network.h"
#include <PubSubClient.h>

//-----------------DEFINICIONES-----------------------------------
const char* deviceName = "Morpheus-beta0.1";
char jsonChar[1000];
byte heartBeat = 0;
byte DatosNoEnviados = 0;
//Parametros de servicio MQTT
WiFiClient espClient;
const int MQTT_PORT = 1883;
const char* MQTT_SERVER = "beaver.rmq.cloudamqp.com";
const char* MQTT_USER = "qhylhbyd:qhylhbyd";
const char* MQTT_PASS = "KGjQKobvQDhz1lI5vJECDRys_gLDnbv4";
PubSubClient mqtt(MQTT_SERVER, MQTT_PORT, espClient);
//Rutas de mensajeria al servicio MQTT
const char* MQTT_ROUTING_KEY = "hipow.cla.porton";
const char* MQTT_ROUTING_KEY2 = "hipow.cla.heartbeat";
const char* MQTT_ROUTING_KEY3 = "hipow.cla.reiniciado";
const char* MQTT_ROUTING_KEY4 = "hipow.cla.actualizado";
const char* MQTT_ROUTING_KEY5 = "hipow.cla.porton_salida";
const char* MQTT_ROUTING_KEY6 = "hipow.cla.porton_entrada";

//----------- Formacion JSON y Envio Rabbit ---------------------------------
//ETIQUETA RFID
void Enviar(String codigo, String MAC, int Fecha, String wifi) {
  DynamicJsonBuffer jsonBuffer(JSON_ARRAY_SIZE(1000));
  JsonObject& equipo = jsonBuffer.createObject();
  JsonObject& emisor = jsonBuffer.createObject();
  JsonObject& cabecera = jsonBuffer.createObject();
  JsonObject& cuerpo = jsonBuffer.createObject();
  JsonObject& attr = jsonBuffer.createObject();
  JsonObject& tx = jsonBuffer.createObject();
  emisor["equipo"] = equipo;
  equipo["id"] = MAC;
  cabecera["codigo"] = codigo;
  cabecera["version"] = deviceName;
  cabecera["uuid"] = StringUUIDGen();
  cuerpo["f"] = Fecha;
  cuerpo["uuid"] = MAC;
  cuerpo["a"] = attr;
  attr["id"] = deviceName;
  attr["wifi_signal"] = wifi;
  tx["emisor"] = emisor;  //Tx junta toda la info y es el dato que se envía a RabbitMQ
  tx["cabecera"] = cabecera;
  tx["cuerpo"] = cuerpo;
  tx.printTo(jsonChar, tx.measureLength() + 1);
  Serial.print("MQTT: ");
  Serial.println(jsonChar);
  if (mqtt.publish(MQTT_ROUTING_KEY, jsonChar)) {
    Serial.println("MQTT: Publicado\n");
  } else {
    Serial.println("MQTT: NO Publicado\n");
    DatosNoEnviados = DatosNoEnviados + 1;
  }
}

//-------------------------------- CALLBACK RABBIT -------------------------------------------
void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  //Transformar en variables un archivo JSON: https://arduinojson.org/v5/assistant/
  Serial.println("MQTT:: MENSAJE RECIBIDO [");
  Serial.print(topic);
  Serial.print("]: ");
  String cadena_in = "";
  for (int i = 0; i < length; i++) {
    cadena_in = cadena_in + (String((char)payload[i]));
  }
  cadena_in.trim();
  Serial.println(cadena_in);
  //Desmenuza el json que llega a la placa
  DynamicJsonBuffer jsonBuffer(JSON_ARRAY_SIZE(1000));
  JsonObject& json_in = jsonBuffer.parseObject(cadena_in);
  JsonObject& cabecera = json_in["cabecera"];
  JsonObject& cuerpo = json_in["cuerpo"];
  JsonObject& emisor = json_in["emisor"];
  String tx_routing_key = cabecera["routing_key"];
  String tx_codigo = cabecera["codigo"];
  String tx_version = cabecera["version"];
  if (tx_codigo == "abrir.cla.entrada") {
    Serial.print('Apertura web: porton entrada');
    Abrir(porton_entrada,id);

  } else if (tx_codigo == "abrir.cla.salida") {
    Serial.print('Apertura web: porton salida');
    Abrir(porton_salida,id);
  } else if (tx_codigo == "appbox.dispositivo.reiniciar") {
    ESP.restart();
  }
}
