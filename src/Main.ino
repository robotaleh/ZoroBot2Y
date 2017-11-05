//////////////
//LIBRERÍAS //
//////////////
#include <ATDelay.h>
#include <PinChangeInt.h>
#include <PIDfromBT.h> // Calibracion de BT desde App Android PIDfromBT

///////////////////
// PINES MOTORES //
///////////////////
#define MOTOR_DERECHO_ADELANTE      9
#define MOTOR_DERECHO_ATRAS         10
#define MOTOR_DERECHO_PWM           11
#define MOTOR_IZQUIERDO_ADELANTE    6
#define MOTOR_IZQUIERDO_ATRAS       7
#define MOTOR_IZQUIERDO_PWM         5
#define MOTOR_RUN                   8

////////////////////
// PINES SENSORES //
////////////////////
#define SENSOR_LATERAL_IZQUIERDO    A7
#define SENSOR_FRONTAL_IZQUIERDO    A6
#define SENSOR_FRONTAL_DERECHO      A1
#define SENSOR_LATERAL_DERECHO      A0

////////////////////
// PINES ENCODERS //
////////////////////
#define ENCODER_IZQUIERDO_A         4
#define ENCODER_IZQUIERDO_B         2
#define ENCODER_DERECHO_A           12
#define ENCODER_DERECHO_B           3



//////////////////////
//VARIABLES NEUTRAS //
//////////////////////
#define NUM_HISTORIAL_SENSORES      10
#define DELAY_FILTRO_SENSORES       5
#define LED_PIN                     13
#define BTN_1                       A3
#define BTN_2                       A2
int velBase     = 0;
int velMax      = 255;
float velI      = 0;
float velD      = 0;
bool run        = false;
bool led_state  = -1;


////////////////////////////////
//VARIABLES CALIBRADO FRONTAL //
////////////////////////////////
#define NUM_SENSORES_FRONTALES      2
#define TIEMPO_CALIBRADO_FRONTAL    5000
#define CALIBRADO_FRONTAL           1
int sensores_frontales[]            = {SENSOR_FRONTAL_IZQUIERDO, SENSOR_FRONTAL_DERECHO};
int valores_sensores_frontales[]    = {0, 0};
int minValoresFrontales[]           = {1023, 1023};
int maxValoresFrontales[]           = {0, 0};
int sumaValoresFrontales[]          = {0,0};
// int minValoresFrontales[]           = {92, 106};
// int maxValoresFrontales[]           = {598, 594};
int filtrado_sensores_frontales[2][NUM_HISTORIAL_SENSORES];

////////////////////////////////
//VARIABLES CALIBRADO LATERAL //
////////////////////////////////
#define NUM_SENSORES_LATERALES      2
#define TIEMPO_CALIBRADO_LATERAL    5000
#define CALIBRADO_LATERAL           1
int sensores_laterales[]            = {SENSOR_LATERAL_IZQUIERDO, SENSOR_LATERAL_DERECHO};
int valores_sensores_laterales[]    = {0, 0};
int minValoresLaterales[]           = {1023, 1023};
int maxValoresLaterales[]           = {0, 0};
int sumaValoresLaterales[]          = {0,0};
// int minValoresLaterales[]           = {194, 167};
// int maxValoresLaterales[]           = {597, 596};
int filtrado_sensores_laterales[2][NUM_HISTORIAL_SENSORES];


/////////////////////////////
//VARIABLES DE ACELERACIÓN //
/////////////////////////////
short velReal                 = 0;
short vel_ini                 = 80;
long millis_inicial_accel     = 0;
float m                       = 0;
bool cambiando_vel            = false;


//////////////////////////
//VARIABLES DE ENCODERS //
//////////////////////////
volatile bool estado_encoder_derecho_A    = false;
volatile bool estado_encoder_izquierdo_A  = false;
volatile bool estado_encoder_derecho_B    = false;
volatile bool estado_encoder_izquierdo_B  = false;
volatile long ticksIzquierdo              = 0;
volatile long ticksDerecho                = 0;

const float RADIO_ARCO_CM                 = 4.2f;
const float DISTANCIA_RUEDAS_CM           = 6.7f;
const float TICKS_CM                      = 49;
const float CM_CASILLA                    = 15.5f;
bool motor_derecho_direccion_adelante = true;
bool motor_izquierdo_direccion_adelante = true;


//////////////////////////
//VARIABLES PID FRONTAL //
//////////////////////////
float kp_frontal                = 1;
float ki_frontal                = 0;
float kd_frontal                = 0;
short ultimoError_frontal[]     = {0, 0};
long  ultimoMillis_frontal[]    = {0, 0};
float correccion_frontal[]      = {0, 0};
short objetivo_frontal[]        = {125, 125};
short umbral_deteccion_frontal  = -75;

//////////////////////////
//VARIABLES PID LATERAL //
//////////////////////////
float kp_lateral                = 0.1;
float ki_lateral                = 0.5;
float kd_lateral                = 100;
int sum_error_lateral           = 0;
short ultimoError_lateral       = 0;
long  ultimoMillis_lateral      = 0;
float correccion_lateral        = 0;
short objetivo_lateral[]        = {0, 0};
short umbral_deteccion_lateral  = 0;
float suma_correcciones[2];

bool started          = false;
bool back_wall_reset  = false;



PIDfromBT pid_calibrate(&kp_lateral, &ki_lateral, &kd_lateral, &velBase, DEBUG);
ATDelay delayPIDlateral(5);
ATDelay delay_sensores_frontales(DELAY_FILTRO_SENSORES);
ATDelay delay_sensores_laterales(DELAY_FILTRO_SENSORES);


void setup() {
  delay(1000);
  init_all();
  delay(100);
  calibrate_all();
  delay(100);
  rotate(50,180);
  delay(100);
  wall_reset();
}

void loop() {
    // put your main code here, to run repeatedly:
}
