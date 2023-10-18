//---------------------------------- LIBRERIAS -------------------------------------
#include <WiFi.h>
#include "time.h"
//------------------------ FECHA ----------------------------------------
const char* ntpServer = "cl.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;
int timeEpoch, Minutes; //tiempo epoch para json y minutos para heartbeat
void LocalTime() {
  struct tm timeinfo;
  time_t Epoch;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Minutes = timeinfo.tm_min;
  time(&Epoch);
  timeEpoch = int(Epoch);
}
//----------------------- WIFI -----------------------------------------
const char* SSID = "POCO F3";
const char* PASS = "kioto123";
String wifiSignal = "";
void setup_wifi() {
  int RST_ESP = 0;
  Serial.print("WF:: Iniciando wifi... ");
  WiFi.begin(SSID, PASS);
  //Caso de desconexion
  while (WiFi.status() != WL_CONNECTED) {
    //Espera la conexion durante 20 segundos y luego reinicia
    if (RST_ESP == 100) {
      Serial.println("Reiniciando...");
      ESP.restart();
      //break;
    } else {
      RST_ESP++;
      Serial.println("Intento de conexion: " + String(RST_ESP));
    }
    delay(200);
  }
  //Caso de conexion
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Conectando a WiFi: ");
    Serial.println(SSID);
    wifiSignal = String(WiFi.RSSI());
    Serial.println("Intensidad: " + wifiSignal + " dBi");
  }
}
