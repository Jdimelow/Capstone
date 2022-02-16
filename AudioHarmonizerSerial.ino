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
    while (index < 4 && (str = strtok_r(p, ";", &p)) != NULL ){
      introInput[index]=str;
      index++;
      //Serial.println(str);
    }
    Serial.println(p);
    if(introInput[3] == "Automatic"){
      Serial.println("In automatic mode");
      String autoInput[5];
      int autoIndex = 0;
      char *str;
      while ((str = strtok_r(p, ";", &p)) != NULL){
        autoInput[autoIndex]=str;
        autoIndex++;
      }
      for(int i=0; i<5;i++)
      {
        Serial.println(autoInput[i]);
      }
      //Serial.println(str); 
    } 
    else{
      Serial.println("In manual mode");
      String mlength = String(strtok_r(p, ";", &p));
      String harmonies = String(strtok_r(p, ";", &p));
      Serial.println(harmonies);
      String noteNumber = String(strtok_r(p, ";", &p));
      String manualInput[mlength.toInt()];
      int manualIndex=0;
      char *str;
      Serial.println("about to parse");
      while ((str = strtok_r(p, ";", &p)) != NULL){
        manualInput[manualIndex]=str;
        manualIndex++;
        //Serial.println(manualIndex);
      }

      
      for(int i=0;i<mlength.toInt();i++)
      {
        Serial.println(manualInput[i]);
      }
    }
     
  }
  int batteryLevel = 49;
  Serial.println(batteryLevel);
  int oldBatteryLevel = 90;
  batteryMonitor(batteryLevel, oldBatteryLevel);
  delay(10000);
  batteryLevel = 39;
  Serial.println(batteryLevel);
  batteryMonitor(batteryLevel, oldBatteryLevel);
  delay(10000);
  batteryLevel = 29;
  Serial.println(batteryLevel);
  batteryMonitor(batteryLevel, oldBatteryLevel);
  delay(10000);
  batteryLevel = 19;
  Serial.println(batteryLevel);
  batteryMonitor(batteryLevel, oldBatteryLevel);
  delay(10000);
  batteryLevel = 9;
  Serial.println(batteryLevel);
  batteryMonitor(batteryLevel, oldBatteryLevel);
  delay(10000);
  
  // testing code for battery fuel guage 
//  int batteryLevel = 57;
//  int oldBatteryLevel = 0;
//  for(int i = 0; i < 1; i++) {
//    oldBatteryLevel = batteryLevel;
//    batteryLevel = random(0, 100);
//    batteryMonitor(batteryLevel, oldBatteryLevel);
//    //Serial.println(oldBatteryLevel);
//    Serial.println(batteryLevel);
//  }

  delay(500);
}

 void batteryMonitor(int batteryLevel, int oldBatteryLEvel) {
   if(batteryLevel != oldBatteryLEvel){
    int splitArray[3] = {0, 0, 0};
    splitInteger(splitArray, batteryLevel);
    convertAscii(splitArray);
  }
}

void splitInteger(int splitArray[3], int batteryLevel){
  for(int i=2; i>=0; i--){
    splitArray[i] = (batteryLevel%10);
    batteryLevel /= 10;
  }
}

void convertAscii(int splitArray[3]){
  int batteryLevel[3];
  // convert to decimal
  for(int i=0; i<3; i++){
    if(splitArray[i] == 0){
      batteryLevel[i] = 48;
    }else if(splitArray[i] == 1){
      batteryLevel[i] = 49;
    }else if(splitArray[i] == 2){
      batteryLevel[i] = 50;
    }else if(splitArray[i] == 3){
      batteryLevel[i] = 51;
    }else if(splitArray[i] == 4){
      batteryLevel[i] = 52;
    }else if(splitArray[i] == 5){
      batteryLevel[i] = 53;
    }else if(splitArray[i] == 6){
      batteryLevel[i] = 54;
    }else if(splitArray[i] == 7){
      batteryLevel[i] = 55;
    }else if(splitArray[i] == 8){
      batteryLevel[i] = 56;
    }else if(splitArray[i] == 9){
      batteryLevel[i] = 57;
    }
  }

  
  //write to terminal
  //SerialBT.write('Battery Level:');
  for(int i=0; i<3; i++){
    SerialBT.write(batteryLevel[i]);
    
    delay(500);
    
  }
  SerialBT.write(100);
}
