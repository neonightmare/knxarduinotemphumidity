

// Sketch for the KNX Temperature sensor
// Hardware by Matthias Freudenreich
// Software by Franck Marini
// Visit http://liwan.fr/KnxWithArduino/ (post on on KNX Temperature sensor) to get all the infos!

// Modification and Test by neonightmare for Testing on Arduino MEGA
// for Uno: Change the Serial-Ports!!

#include <KnxDevice.h> // get the library at https://github.com/franckmarini/KnxDevice (use V0.3 or later)

//#include <DHT22.h> // get the library at http://playground.arduino.cc/Main/DHTLib
#include <dht.h>


int DHTpin = 2;    // DHT22(2) sensor pin

// DHT22 sensor -> blau:GND(DHTPin_4); violett +5V (DHTPin_1); grün: Signal DHTPin_2
dht DHT;

#define DHT22_PIN 2
struct
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};

// Definition of the Communication Objects attached to the device
KnxComObject KnxDevice::_comObjectsList[] =
{
 /* Index 0 = TEMPERATURE VALUE */    KnxComObject(G_ADDR(2,0,0), KNX_DPT_9_001 /* 2 byte float DPT */ , COM_OBJ_SENSOR  /* Sensor Output */ ) ,
 /* Index 1 = HUMIDITY VALUE    */    KnxComObject(G_ADDR(2,0,1), KNX_DPT_9_001 /* 2 byte float DPT */ , COM_OBJ_SENSOR  /* Sensor Output */ ) ,
};
const byte KnxDevice::_comObjectsNb = sizeof(_comObjectsList) / sizeof(KnxComObject); // do no change this code


//Verbindung zu BCU/TPUART:
//
//   o o o o o  -> 10 9 8 7 6
//   o o o o o  -> 1 2 3 4 5
//   1 -> GND // 2 RxD-> to Arduino TX // 4 TxD -> to Arduino RX // 5 -> +5V


// Callback function to handle com objects updates
void knxEvents(byte index) {};


unsigned long      lastTime     = 0;
unsigned long      elapsedTime  = 0;
unsigned long      waitTime     = 10000; // 10s

void setup(){
   Serial.begin(115200); //Debug-Kommunikation ->SerialMonitor
   Serial.println();
   Serial.print("Setup begins");
   Serial.println();
   // Open serial communications and wait for port to open:
   pinMode(DHTpin, OUTPUT);
   digitalWrite(DHTpin, HIGH);
   Knx.begin(Serial3, P_ADDR(1,1,30)); // start a KnxDevice session with physical address 1.1.30 on Serial UART
   Serial.print("Setup finished");
   Serial.println();
}


void loop(){
  Knx.task();
  Serial.print("loop: ");
  Serial.println();

  if (elapsedTime >= waitTime)
  { // Every waitTime we read humidity and temperature and send values to the KNX bus

    int chk = DHT.read22(DHT22_PIN);
    Knx.write(0,DHT.temperature);
    Knx.write(1,DHT.humidity);
    Serial.print("Temperatur: ");
    Serial.print(DHT.temperature);
    Serial.println();
    Serial.print("Feuchtigkeit: ");
    Serial.print(DHT.humidity);
    Serial.println();
    lastTime = millis();
  }
  elapsedTime = (millis() - lastTime);
}
