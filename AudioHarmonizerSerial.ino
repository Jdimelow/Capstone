//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include <BluetoothSerial.h>
//#include <BTAddress.h>
//#include <BTAdvertisedDevice.h>
//#include <BTScan.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32Test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
    //Serial.println(SerialBT.readString());
    //Serial.println("");
    
    String input = SerialBT.readString();
    //Serial.println(input);
    char buf[input.length()];
    input.toCharArray(buf, sizeof(buf));
    //Serial.println(buf);
    char *p = buf;
    char *str;
    int index=0;
    String introInput[4];
    while (index < 4 && (str = strtok_r(p, ".", &p)) != NULL ){
      introInput[index]=str;
      index++;
      //Serial.println(str);
    }
    Serial.println(p);
    if(introInput[3] == "Automatic")
    {
      Serial.println("In automatic mode");
      String autoInput[5];
      int autoIndex = 0;
      char *str;
      while ((str = strtok_r(p, ".", &p)) != NULL){
        autoInput[autoIndex]=str;
        autoIndex++;
      }
      for(int i=0; i<5;i++)
      {
        Serial.println(autoInput[i]);
      }
      //Serial.println(str); 
    }
    else
    {
      Serial.println("In manual mode");
      String mlength = String(strtok_r(p, ".", &p));
      String harmonies = String(strtok_r(p, ".", &p));
      Serial.println(harmonies);
      String noteNumber = String(strtok_r(p, ".", &p));
      String manualInput[mlength.toInt()];
      int manualIndex=0;
      char *str;
      Serial.println("about to parse");
      while ((str = strtok_r(p, ".", &p)) != NULL){
        manualInput[manualIndex]=str;
        manualIndex++;
        Serial.println(manualIndex);
      }

      
//      for(int i=0;i<mlength.toInt();i++)
//      {
//        Serial.println(manualInput[i]);
//      }
    }
     
  }
  delay(20);
}
