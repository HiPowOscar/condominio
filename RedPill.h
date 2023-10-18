//----------------------------- Variables de estado --------------------------------
unsigned long tiempo = 0;
String deviceState = "";
boolean stateIN = false, stateOUT = false;
String idUser = "", estadoSensores = "";
byte Entrada = 1, Salida = 1, infraCount = 5, estadoEntrada = 0, estadoSalida = 0;  //Veces que las barras del porton cruzan el infrarrojo para estar completamente abierto
unsigned long periodo23 = 1000, periodo14 = 250, tBloqueo = 50000;                  //Tiempo de delay de infrarrojos 2-3 y 1-4 respectivamente
unsigned long periodoFC1 = 0, periodoFC2 = 0, periodoFC3 = 0, periodoFC4 = 0, tiempoSTART = 0, tiempoLOCK = 0;
//-------------Pines Generales-------------------------------------------------------
//Apertura-Cierre Porton Entrada
#define START_1 27
//Apertura-Cierre Porton Salida
#define START_2 26
//Bloqueo BrazoHidraulico_1
#define STOP_1 25
//Bloqueo BrazoHidraulico_2
#define STOP_2 33
//Sensor Corte Entrada
#define Fotocelda_1 18
//Sensor Entrada
#define Fotocelda_2 19
//Sensor Salida
#define Fotocelda_3 21
//Sensor Corte Salida
#define Fotocelda_4 22
//Apertura por RTU
#define puertoRTU 23
//------------Apertura----------------------------------
void Open(byte porton) {
  if (porton == 0) {
    stateIN = true;              //Activacion logica
    digitalWrite(START_1, LOW);  //Activacion fisica
  } else if (porton == 1) {
    stateOUT = true;             //Activacion logica
    digitalWrite(START_1, LOW);  //Activacion fisica
  }
}
//------------BLOQUEO--------------------------------
void Block(byte porton) {
  if (porton == 0) {
    stateIN = true;              //Activacion logica
    digitalWrite(START_1, LOW);  //Activacion fisica
  } else if (porton == 1) {
    stateOUT = true;             //Activacion logica
    digitalWrite(START_1, LOW);  //Activacion fisica
  }
}
//---------FOTOCELDAS-------------------------------
char orden[4];
byte countIn = 0, countOut = 0;
void Fotoceldas() {
  //Inicia la lectura de los infrarrojos
  int fc1 = digitalRead(Fotocelda_1);
  int fc2 = digitalRead(Fotocelda_2);
  int fc3 = digitalRead(Fotocelda_3);
  int fc4 = digitalRead(Fotocelda_4);
  //Revisa los tiempos de apertura y conteo de barras de los portones
  if (fc1 == HIGH) {
    Serial.print("Fotocelda 1 activada\n");
    if (tiempo - periodoFC1 >= periodo14) {
      orden[0] = '1';
    }
  } else if (fc1 == LOW) {
    periodoFC1 = tiempo;
    orden[0] = '0';
  }
  if (fc2 == HIGH) {
    Serial.print("Fotocelda 2 activada\n");
    countIn += 1;  //Cuenta las veces que el porton tapa el Infrarrojo
    if (countIn >= infraCount) {
      estadoEntrada = 1;
    }
    if (tiempo - periodoFC2 >= periodo23) {
      orden[1] = '1';
    }
  } else if (fc2 == LOW) {
    periodoFC2 = tiempo;
    orden[1] = '0';
  }
  if (fc3 == HIGH) {
    Serial.print("Fotocelda 3 activada\n");
    countOut += 1;  //Cuenta las veces que el porton tapa el Infrarrojo
    if (countOut >= infraCount) {
      estadoSalida = 1;
    }
    if (tiempo - periodoFC3 >= periodo23) {
      orden[2] = '1';
    }
  } else if (fc3 == LOW) {
    periodoFC3 = tiempo;
    orden[2] = '0';
  }
  if (fc4 == HIGH) {
    Serial.print("Fotocelda 4 activada\n");
    if (tiempo - periodoFC4 >= periodo14) {
      orden[3] = '1';
    }
  } else if (fc4 == LOW) {
    periodoFC4 = tiempo;
    orden[3] = '0';
  }
}
//----------------------------LOGICA DE SENSORES-----------------------------------
byte portonSalida = 0, portonEntrada = 0;
void logicaSensores() {
  if (orden == "1000") {
    estadoSensores = "Saliendo";
  } else if (orden == "1100") {
    estadoSensores = "Saliendo";
  } else if (orden == "1110") {
    estadoSensores = "Obstaculizado";
  } else if (orden == "1111") {
    estadoSensores = "Obstaculizado";
  } else if (orden == "0111") {
    estadoSensores = "Obstaculizado";
  } else if (orden == "0011") {
    estadoSensores = "Entrando";
  } else if (orden == "0001") {
    estadoSensores = "Entrando";
  } else if (orden == "0000") {
    estadoSensores = "Sin Movimiento";
  } else if (orden == "1001") {
    estadoSensores = "Saliendo y Entrando";
  } else if (orden == "1101") {
    estadoSensores = "Saliendo y Entrando";
  } else if (orden == "1011") {
    estadoSensores = "Saliendo y Entrando";
  } else if (orden == "0110") {
    estadoSensores = "Obstaculizado";
  } else if (orden == "0101") {
    estadoSensores = "Entrando";
  } else if (orden == "1010") {
    estadoSensores = "Saliendo";
  } else if (orden == "0100") {
    estadoSensores = "Porton entrada";
  } else if (orden == "0010") {
    estadoSensores = "Porton salida";
  }
}
//---------------------------LOOP----------------------------------
void redPill_loop() {
  Fotoceldas();
  logicaSensores();
  if (estadoSensores == "Obstaculizado") {
    Serial.println(estadoSensores);
    if (estadoEntrada == 1) {
      if (tiempo - tiempoLOCK >= tBloqueo)
        Block(estadoEntrada);
      Serial.println('Porton de entrada bloqueado');
    } else if (estadoSalida == 1) {
      if (tiempo - tiempoLOCK >= tBloqueo) {
        Block(estadoSalida);
        Serial.print('Porton de salida bloqueado');
      }
    }
  }
}
