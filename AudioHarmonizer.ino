
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
unsigned int txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCallBacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("===Start=Receive===");
      Serial.println("Recieved Value: ");

      for (int i = 0; i < rxValue.length(); i++){
        Serial.print(rxValue[i]);
      }
      
      Serial.println();

      // Activate maual or automic mode ---> LED connected to GPIO 25
      int OpMode = 25;
      //digitalWrite(OpMode, LOW); // Set to automatic by default (LOW'0')
      if(rxValue.find("1") != -1) {
        Serial.println("Manual Mode Set!");
        digitalWrite(OpMode, HIGH);
      }
      else if(rxValue.find("0") != -1) {
        Serial.println("Automatic Mode Set!");
        digitalWrite(OpMode, LOW);
      }

      Serial.println(); 
      Serial.println("===End=Receive===");
    }
    
  }
};

//BluetoothSerial SerialBT;
const int OpMode = 25; // manual or automatic mode pin (for testing a led)

void setup() {
  Serial.begin(115200);
  //Set pinmode for the output of which mode
  pinMode(OpMode, OUTPUT);
  
  //Bluetooth device name
  BLEDevice::init("ESP32test"); 
  
  // BLE server
  BLEServer *pServer = BLEDevice::createServer(); 
  pServer->setCallbacks(new MyServerCallbacks());
  
  //BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);  
  
  // BLE characteristic
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY); 
                    
  //notify BLE2902
  pCharacteristic->addDescriptor(new BLE2902());
  
  // characteristic for receiving from app
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,
                    BLECharacteristic::PROPERTY_WRITE);
                     
  //start service 
  pService->start();
  
  //start advertising 
  pServer->getAdvertising()->start();
  Serial.println("Waiting to connect to client...");
  
  //pinMode(OpMode, OUTPUT); // operation mode manual or automatic(default to automatic)
}

void loop() {
  if (deviceConnected) {
    txValue = random(-10, 20); // tx value is the output going to the app
    //convert txValue to characteristic
    char txString[8];
    dtostrf(txValue, 1, 2, txString);
    //notifying the client(THING CONNECTED TO)
    pCharacteristic->notify();
    Serial.println("Sent Value: " + String(txString));
    delay(1000);
    
  }
  
}

//set up bluetooth connection with app

//set up gpio pins

//receive data from bluetooth app

//send data to dsp 
