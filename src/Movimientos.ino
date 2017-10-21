/////////////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones referentes al movimiento //
// del robot, así como cálculos de ticks para los arcos y giros.       //
/////////////////////////////////////////////////////////////////////////

/**
* Función para realizar un giro de los grados indicados
*/
void rotate(int velBase, int grados){
	float ticksMotores[2];
	calcular_rotacion(grados, ticksMotores);
	ticksIzquierdo = 0;
	ticksDerecho = 0;
	short velBaseI = velBase;
	short velBaseD = velBase;
	if(grados > 0){
		velBaseD = -velBaseD;
	}else{
		velBaseI = -velBaseI;
	}
	do {
		if(abs(ticksIzquierdo) >= ticksMotores[0]){
			velBaseI = 0;
		}
		if(abs(ticksDerecho) >= ticksMotores[1]){
			velBaseD = 0;
		}
		set_speed(velBaseI, velBaseD);
	}while(abs(ticksDerecho) < ticksMotores[1] || abs(ticksIzquierdo) < ticksMotores[0]);
	if(grados>0){
		stop(-(velBase/2), (velBase/2));
	}else{
		stop((velBase/2), -(velBase/2));
	}
}

/**
* Función para resetear error del robot yendo marcha atrás hacia una pared y
* posicionándose en el centro de la casilla.
*
* NOTE: Para usarlo, es recomendable que sea el final de un callejón sin salida.
*/
void wall_reset(){
	int ticks_objetivoI = 0;
	int ticks_objetivoD = 0;
	do {
		set_speed(-50, -50);
		ticksDerecho = 0;
		ticksIzquierdo = 0;
		delay(100);
		if(!ticks_objetivoI && !ticks_objetivoD){
			ticks_objetivoI = abs(ticksIzquierdo);
			ticks_objetivoD = abs(ticksDerecho);
		}
	}while(abs(ticksIzquierdo) > (ticks_objetivoI/2) || abs(ticksDerecho) > (ticks_objetivoD/2));
	stop();
	delay(100);
	run_cm(50, 5.5f);
}

/**
 * Función para desplazar longitudinalmente el robot tantos cm como se indiquen.
 * @param velBase Velocidad del movimiento
 * @param cm      Centímetros que se recorrerán
 */
void run_cm(int velBase, float cm){
		int ticks_movimiento = cm * TICKS_CM;
	  ticksDerecho = 0;
	  ticksIzquierdo = 0;
	  int velI = velBase;
	  int velD = velBase;
	  do{
	    if(ticksIzquierdo >= ticks_movimiento){
	      velI = 0;
	    }
	    if(ticksDerecho >= ticks_movimiento){
	      velD = 0;
	    }
			PID();
	    set_speed(velI, velD);
	  }while(ticksDerecho < ticks_movimiento || ticksIzquierdo < ticks_movimiento);
		stop();
}

/**
* Calcula los ticks que deben dar los motores y la proporción de velocidades para realizar el giro de los grados indicados
* @method calcular_arco_giro
* @param  grados             Cantidad de grados a girar
* @param  ticksMotores       Array donde se almacenarán los ticks.
* @return                    Proporción de velocidades para efectuar el giro
*/
float calcular_arco_giro(int grados, float ticksMotores[]) {
	float rad = (grados * 3.1416f / 180);
	float arco_interior = rad * (float)RADIO_ARCO_CM;
	float arco_exterior = rad * (float)(RADIO_ARCO_CM + DISTANCIA_RUEDAS_CM);
	if (grados > 0) {
		ticksMotores[0] = (float)TICKS_CM * arco_interior;
		ticksMotores[1] = (float)TICKS_CM * arco_exterior;
		return (arco_exterior / arco_interior);
	} else {
		ticksMotores[0] = (float)TICKS_CM * abs(arco_exterior);
		ticksMotores[1] = (float)TICKS_CM * abs(arco_interior);
		return -(arco_exterior / arco_interior);
	}
}

/**
* Calcula los ticks que deben dar los motores para rotar sobre si mismo los grados indicados
* @method calcular_arco_giro
* @param  grados             Cantidad de grados a girar
* @param  ticksMotores       Array donde se almacenarán los ticks.
*/
void calcular_rotacion(int grados, float ticksMotores[]) {
	float rad = (grados * 3.1416f / 180);
	float arco_interior = rad * (float)DISTANCIA_RUEDAS_CM/2.0f;
	float arco_exterior = rad * (float)DISTANCIA_RUEDAS_CM/2.0f;
	if (grados > 0) {
		ticksMotores[0] = (float)TICKS_CM * arco_interior;
		ticksMotores[1] = (float)TICKS_CM * arco_exterior;
	} else {
		ticksMotores[0] = (float)TICKS_CM * abs(arco_exterior);
		ticksMotores[1] = (float)TICKS_CM * abs(arco_interior);
	}
}


/**
* Establece la velocidad para ambos motores individualmente
* @param velI Velocidad para el motor izquierdo
* @param velD Velocidad para el motor derecho
*/
void set_speed(float velBaseI, float velBaseD) {
	int pinD = MOTOR_DERECHO_ADELANTE;
	int pinI = MOTOR_IZQUIERDO_ADELANTE;
	sumar_correcciones(suma_correcciones);
	int velI = velBaseI + suma_correcciones[0];
	int velD = velBaseD + suma_correcciones[1];

	// Limitar velocidad del motor derecho y selecciona la dirección.
	if (velD > 255) {
		velD = 255;
		pinD = MOTOR_DERECHO_ADELANTE;
	} else if (velD < 0) {
		velD = abs(velD);
		if (velD > 255) {
			velD = 255;
		}
		pinD = MOTOR_DERECHO_ATRAS;
	}

	// Limitar velocidad del motor izquierdo y selecciona la dirección.
	if (velI > 255) {
		velI = 255;
		pinI = MOTOR_IZQUIERDO_ADELANTE;
	} else if (velI < 0) {
		velI = abs(velI);
		if (velI > 255) {
			velI = 255;
		}
		pinI = MOTOR_IZQUIERDO_ATRAS;
	}

	if (!run) {
		run = true;
		digitalWrite(MOTOR_RUN, HIGH);
	}
	digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
	digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
	digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
	digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);

	digitalWrite(pinD, HIGH);
	digitalWrite(pinI, HIGH);

	analogWrite(MOTOR_DERECHO_PWM   , velD);
	analogWrite(MOTOR_IZQUIERDO_PWM , velI);
}

/**
* Detiene completamente el robot, indicando una contra-velocidad para corregir inercia
*/
void stop(int contra_vel_I, int contra_vel_D){
	correccion_lateral = 0;
	correccion_frontal[0] = 0;
	correccion_frontal[1] = 0;

	set_speed(contra_vel_I, contra_vel_D);
	delay(50);
	run = false;
	digitalWrite(MOTOR_RUN, HIGH);
	digitalWrite(MOTOR_DERECHO_PWM, LOW);
	digitalWrite(MOTOR_IZQUIERDO_PWM, LOW);
}

/**
 * Detiene completamente el robot.
 */
void stop(){
	stop(0,0);
}
