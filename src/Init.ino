//////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones que se            //
// ejecutarán al inicio del programa, como inicializaciones de  //
// componentes o del puerto Serial.                             //
//////////////////////////////////////////////////////////////////


/**
* Inicializa todos los pines y componentes
*/
void init_all(){
  init_serial();
  init_motores();
  init_encoders();
  init_sensores();
  init_btn_led();
  init_gyro();
}

/**
 * Inicializa el puerto Serial
 */
void init_serial(){
  Serial.begin(9600);
  while(!Serial){}
}

/**
* Inicializa los pines de los motores y los para.
*/
void init_motores(){
  // Inicialización de pines
  pinMode(MOTOR_DERECHO_ADELANTE, OUTPUT);
  pinMode(MOTOR_DERECHO_ATRAS, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ADELANTE, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ATRAS, OUTPUT);
  pinMode(MOTOR_DERECHO_PWM, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_PWM, OUTPUT);
  pinMode(MOTOR_RUN, OUTPUT);

  // Estado inicial; parado
  digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
  digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);
  digitalWrite(MOTOR_RUN, HIGH);
  digitalWrite(MOTOR_DERECHO_PWM, LOW);
  digitalWrite(MOTOR_IZQUIERDO_PWM, LOW);
}

/**
* Inicializa los encoders en cuadratura
* usando la librería "PinChangeInt.h" para
* los pines fuera de interrupciones nativas
* y obtiene sus estados iniciales.
*/
void init_encoders(){
  // Inicializa los encoders.
  attachInterrupt(digitalPinToInterrupt(ENCODER_DERECHO_B), tick_encoder_derecho_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_IZQUIERDO_B), tick_encoder_izquierdo_B, CHANGE);
}

/**
 * Inicializa todos los sensores
 */
void init_sensores(){
  for(byte sensor = 0;sensor<NUM_SENSORES_FRONTALES;sensor++){
    pinMode(sensores_frontales[sensor], INPUT);
  }

  for(byte sensor = 0;sensor<NUM_SENSORES_LATERALES;sensor++){
    pinMode(sensores_laterales[sensor], INPUT);
  }
}

/**
 * Inicializa leds y botones
 */
void init_btn_led(){
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
}
