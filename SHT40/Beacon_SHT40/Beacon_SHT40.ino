/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Changed to a beacon scanner to report iBeacon, EddystoneURL and EddystoneTLM beacons by beegee-tokyo
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEEddystoneURL.h>
#include <BLEEddystoneTLM.h>
#include <BLEBeacon.h>
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

float TemperatureData,HumidityData;
float Temperature,Humidity;

//设置ESP32 5秒扫描一次蓝牙设备
int scanTime = 5; //In seconds
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
      if (advertisedDevice.haveName())
      {
        if(String(advertisedDevice.getName().c_str()) == "SHT40")//扫描是否有名为SHT40的蓝牙设备
        {
          Serial.print("Device name: ");
          Serial.println(advertisedDevice.getName().c_str());
          std::string strManufacturerData = advertisedDevice.getManufacturerData();
          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);
          Serial.printf("strManufacturerData: %d ", strManufacturerData.length());

          for (int i = 0; i < strManufacturerData.length(); i++)
          {
            Serial.printf("[%X]", cManufacturerData[i]);
          }
          
          //从SHT40获取原始数据
          TemperatureData = int(cManufacturerData[2]<<8 | cManufacturerData[3]);
          HumidityData = int(cManufacturerData[5]<<8 | cManufacturerData[6]);
          
          //将原始数据转化为温湿度数据
          Temperature = (175 * TemperatureData/65535) - 45;
          Humidity = (125 * HumidityData/65535) - 6;

          Serial.println();
          Serial.print("TemperatureData:");Serial.print(Temperature);Serial.println("℃");   
          Serial.print("HumidityData:");Serial.print(Humidity);Serial.println("%");      
          Serial.println("------------------");       
        }        
      }
    }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99); // less or equal setInterval value
}

void loop()
{
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
  delay(2000);
}