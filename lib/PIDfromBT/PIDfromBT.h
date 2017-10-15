#ifndef PIDfromBT_h
#define PIDfromBT_h
#define LIBRARY_VERSION 1
#include <SoftwareSerial.h>
class PIDfromBT
{

#define DEBUG 1
#define NO_DEBUG 0

#define MIN_IDEAL 0
#define MAX_IDEAL 0

public:

  PIDfromBT(float* kp, float* kd, float* ki, int* vel, bool* acelerar, float* aumentoP, float* aumentoD, float* maxAumento, float* velIncremento, float* maxError, bool debug);
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, bool debug);
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, float* ideal, int minIdeal, int maxIdeal, bool debug);
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, float* ideal, int minIdeal, int maxIdeal, bool* acelerar, float* aumentoP, float* aumentoD, float* maxAumento, float* velIncremento, float* maxError, bool debug);

  void setMinIdeal(int minIdeal);
  void setMaxIdeal(int maxIdeal);
  void setCarriles(short* carrilObjetivo, short maxCarriles);
  void setCarriles(bool carrilesEnabled);
  bool update();

private:
  String valor;
  char letra;
  long last_millis;
  bool print;
  float *kp;
  float *ki;
  float *kd;
  int *vel;
  bool* acelerar;
  float* aumentoP;
  float* aumentoD;
  float* maxAumento;
  float* velIncremento;
  float* maxError;
  float* ideal;
  int maxIdeal;
  int minIdeal;
  short acc_activada;
  long last_millis_ideal;
  long last_millis_vel;
  bool carrilesEnabled = false;
  short *carrilObjetivo;
  short maxCarriles;
};
#endif

