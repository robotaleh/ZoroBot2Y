/////////////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones referentes al movimiento //
// del robot, así como cálculos de ticks para los arcos y giros.       //
/////////////////////////////////////////////////////////////////////////


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
* Detiene completamente el robot
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
