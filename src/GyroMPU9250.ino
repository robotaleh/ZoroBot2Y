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
* Calcula el ángulo real en el que se encuentra el robot
*/
void process_Z_angle(){
  if(last_angle_micros == 0){
    last_angle_micros = micros();
  }
  float rad_s = read_gyro_z();
  angle -= 1.05 * rad_s * (micros()-last_angle_micros) / 1000000.0f;
  last_angle_micros = micros();
}

/**
* Obtiene el ángulo ya calculado anteriormente
* @return Angulo real del robot en grados.
*/
float get_z_angle(){
  return angle;
}

/**
 * Establece un ángulo objetivo para el robot y rota hasta llegar a el y mantenerse
 * durante 200ms para evitar que se haya pasado de largo.
 * @param angle_dest Ángulo destino en grados
 */
void set_z_angle(float angle_dest){
  float error = angle_dest - angle;
  float p;
  float i;
  long millis_error_ideal = 0;
  while(abs(error) > 1 && (millis_error_ideal == 0 || (millis()-millis_error_ideal)<200)){
    process_Z_angle();
    p = kp_gyro * error;
    if(abs(i)<25){
      i += error*0.5f;
    }
    set_speed(p+i, -(p+i));

    error = angle_dest - angle;
    if(abs(error)<=1){
      if(millis_error_ideal == 0){
        millis_error_ideal = millis();
      }
    }else{
      millis_error_ideal = 0;
    }
  }
  stop();
}

/**
* Resetea el ángulo para eliminar el error.
* NOTE: Esta función se debe de llamar antes de realizar un giro,
*       para eliminar el error que podría tener acumulado el giroscopio.
*/
void reset_z_angle(){
  angle = 0;
}

/**
* Calcula el ruido normal del giroscopio
* para excluirlo de las lecturas reales.
*/
void gyro_calibrate_deadband(){
  float noise = 0;
  for(int i = 0; i < 50;i++){
    noise += read_gyro_z();
    delay(10);
  }
  deadband_z = noise/50.0f;
}

/**
* Lectura directa del eje Z del giroscopio
* @return rad/s actuales
*/
float read_gyro_z(){
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int z = ((Buf[12] << 8 | Buf[13]) - deadband_z)/16.4;
  return (abs(z) > 1)? z: 0;
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
