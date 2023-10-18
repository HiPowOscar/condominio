#include <EEPROM.h>
#define EEPROM_SIZE 512

//-----------INTEGERS------------------------
void writeIntIntoEEPROM(int address, int number)
{
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

//-----------ARRAY-INTEGERS-------------------
void writeIntArrayIntoEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    EEPROM.write(addressIndex, numbers[i] >> 8);
    EEPROM.write(addressIndex + 1, numbers[i] & 0xFF);
    addressIndex += 2;
  }
}
void readIntArrayFromEEPROM(int address, int numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = (EEPROM.read(addressIndex) << 8) + EEPROM.read(addressIndex + 1);
    addressIndex += 2;
  }
}

//--------LONG-------------------------
void writeLongIntoEEPROM(int address, long number)
{
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, (number >> 8) & 0xFF);
  EEPROM.write(address + 3, number & 0xFF);
}
long readLongFromEEPROM(int address)
{
  return ((long)EEPROM.read(address) << 24) +
         ((long)EEPROM.read(address + 1) << 16) +
         ((long)EEPROM.read(address + 2) << 8) +
         (long)EEPROM.read(address + 3);
}

//------------LONG ARRAY-------------------------------
void writeLongArrayIntoEEPROM(int address, long numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    EEPROM.write(addressIndex, (numbers[i] >> 24) & 0xFF);
    EEPROM.write(addressIndex + 1, (numbers[i] >> 16) & 0xFF);
    EEPROM.write(addressIndex + 2, (numbers[i] >> 8) & 0xFF);
    EEPROM.write(addressIndex + 3, numbers[i] & 0xFF);
    addressIndex += 4;
  }
}
void readLongArrayFromEEPROM(int address, long numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = ((long)EEPROM.read(addressIndex) << 24) +
                 ((long)EEPROM.read(addressIndex + 1) << 16) +
                 ((long)EEPROM.read(addressIndex + 2) << 8) +
                 (long)EEPROM.read(addressIndex + 3);
    addressIndex += 4;
  }
}

//-----------UNSIGNED LONG -----------------------------------
void writeUnsignedLongIntoEEPROM(int address, unsigned long number)
{
  EEPROM.write(address, (number >> 24) & 0xFF);
  EEPROM.write(address + 1, (number >> 16) & 0xFF);
  EEPROM.write(address + 2, number >> 8);
  EEPROM.write(address + 3, number & 0xFF);
}
unsigned long readUnsignedLongFromEEPROM(int address)
{
  return ((unsigned long)EEPROM.read(address) << 24) +
         ((unsigned long)EEPROM.read(address + 1) << 16) +
         ((unsigned long)EEPROM.read(address + 2) << 8) +
         (unsigned long)EEPROM.read(address + 3);
}

//------------UNSIGNED LONG ARRAY-------------------------
void writeUnsignedLongArrayIntoEEPROM(int address, unsigned long numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    EEPROM.write(addressIndex, (numbers[i] >> 24) & 0xFF);
    EEPROM.write(addressIndex + 1, (numbers[i] >> 16) & 0xFF);
    EEPROM.write(addressIndex + 2, (numbers[i] >> 8) & 0xFF);
    EEPROM.write(addressIndex + 3, numbers[i] & 0xFF);
    addressIndex += 4;
  }
}
void readUnsignedLongArrayFromEEPROM(int address, unsigned long numbers[], int arraySize)
{
  int addressIndex = address;
  for (int i = 0; i < arraySize; i++)
  {
    numbers[i] = ((unsigned long)EEPROM.read(addressIndex) << 24) +
                 ((unsigned long)EEPROM.read(addressIndex + 1) << 16) +
                 ((unsigned long)EEPROM.read(addressIndex + 2) << 8) +
                 (unsigned long)EEPROM.read(addressIndex + 3);
    addressIndex += 4;
  }
}

//-------------COMMIT---------------------
void eepromCommit() {
  if (EEPROM.commit()) {
    Serial.println("Configuracion almacenada en la EEPROM!");
  } else {
    Serial.println("ERROR! fallo la escritura en la EEPROM");
  }
}

//-----------------EEPROM--------------------------------
void clearEEPROM() {
  for (int E = 0; E < EEPROM.length(); E++)
  {
    EEPROM.write(E, 0);   // Itera por toda la EEPROM y reemplaza cualquier valor que esté en algún byte por cero
  }
}
