#include <Wire.h>

#define SHT40_ADDRESS 0x44 // 传感器的 I2C 地址，此处的SHT40为0x44
int l = 5;// 读字节长度

void setup(){
  Serial.begin(115200);
  Wire.begin();
}

void loop() {

  Wire.beginTransmission(SHT40_ADDRESS);
  Wire.write(0xFD);//i2ctx:3
  Wire.endTransmission();//i2c null
  
  delay(10); // 程序等待10ms，让SHT40做好准备

  Wire.requestFrom(SHT40_ADDRESS, 6);
  if (Wire.available() >= l) {
    byte data[l];
    for (int i = 0; i < l; i++) {//读取I2C传感器输出的数据
      data[i] = Wire.read();
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
  Serial.println();
  }
  Wire.endTransmission();//i2c null
  delay(1000); 
}
