///////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones de control y   //
// calibración del giroscopio MPU9250 integrado en el robot. //
///////////////////////////////////////////////////////////////
#include <Wire.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

int deadband_z = 0;
float angle = 0;
long last_angle_micros = 0;

float kp_gyro = 1;

/**
* Inicializa el gyro, calculando el ruido.
*/
void init_gyro(){
  Wire.begin();
  I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);

  gyro_calibrate_deadband();
}
/**
* Funcion auxiliar lectura
* @param Address  Dirección del MPU9250.
* @param Register Registro donde se escribirá.
* @param Nbytes   Cantidad de bytes que se leerán.
* @param Data     Variable de salida con los datos de lectura.
*/
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
  Data[index++] = Wire.read();
}

/**
* Funcion auxiliar de escritura
* @param Address  Dirección del MPU9250.
* @param Register Registro donde se escribirá.
* @param Data     Contenido de la escritura.
*/
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}
