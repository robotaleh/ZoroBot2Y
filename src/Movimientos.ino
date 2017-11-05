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
	back_wall_reset = true;
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
	// delay(100);
	run_cm(100, 15, false);
	back_wall_reset = false;
}

/**
* Realiza un giro en arco, en lugar de hacerlo sobre si mismo, para ahorrar tiempo.
* Suele hacerse para giros de 90º en esquinas del recorrido.
* @param velBase Velocidad base a la que tomará el giro
* @param grados  Grados que describe el arco a realizar.
*/
void giro_arco(int velBase, int grados){
	float proporcion = 1.0f;
	float ticksMotores[2];
	proporcion = calcular_arco_giro(grados, ticksMotores);


	int velBaseI = velBase;
	int velBaseD = velBase;
	int vel_prop = ((float)velBase * (proporcion-1));

	if(grados>0){
		velBaseI += vel_prop;
	}else{
		velBaseD += vel_prop;
	}
	ticksIzquierdo = 0;
	ticksDerecho = 0;

	do {
		if(ticksIzquierdo >= ticksMotores[0] || ticksDerecho >= ticksMotores[1]){
			velI = 0;
			velD = 0;
		}
		set_speed(velBaseI, velBaseD);
	} while (ticksIzquierdo <= ticksMotores[0] && ticksDerecho <= ticksMotores[1]);
	stop(-velBase, -velBase);
}

/**
* Función para desplazar longitudinalmente el robot tantos cm como se indiquen.
* @param velBase Velocidad del movimiento
* @param cm      Centímetros que se recorrerán
* @param pausa   Indica si se llamará a la función "stop" después de recorrer los cm indicados.
*/
void run_cm(int velBase, float cm, bool pausa){
	int ticks_movimiento = cm * TICKS_CM;
	ticksDerecho = 0;
	ticksIzquierdo = 0;
	int velI = velBase;
	int velD = velBase;
	do{
		if(ticksIzquierdo >= ticks_movimiento || ticksDerecho >= ticks_movimiento){
			velI = 0;
			velD = 0;
		}
		loop_functions();
		PID();
		set_speed(velI, velD);
	}while(ticksIzquierdo <= ticks_movimiento && ticksDerecho <= ticks_movimiento);
	if(pausa)
	stop(0,0);
}

/**
* Calcula los ticks que deben dar los motores y la proporción de velocidades para realizar el giro de los grados indicados
* @method calcular_arco_giro
* @param  grados             Cantidad de grados a girar
* @param  ticksMotores       Array donde se almacenarán los ticks.
* @return                    Proporción de velocidades para efectuar el giro
*/
float calcular_arco_giro(int grados, float ticksMotores[]) {
	float rad = (abs(grados) * 3.1416f / 180);
	float arco_interior = rad * (float)RADIO_ARCO_CM *1.1f;
	float arco_exterior = rad * (float)(RADIO_ARCO_CM + DISTANCIA_RUEDAS_CM) *0.98f;
	if(grados > 0){
		ticksMotores[0] = (float)TICKS_CM * arco_exterior;
		ticksMotores[1] = (float)TICKS_CM * arco_interior;
	}else{
		ticksMotores[0] = (float)TICKS_CM * arco_interior;
		ticksMotores[1] = (float)TICKS_CM * arco_exterior;
	}
	return (arco_exterior / arco_interior);
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
	motor_derecho_direccion_adelante = true;
	motor_izquierdo_direccion_adelante = true;
	sumar_correcciones(suma_correcciones);
	int velI = velBaseI + suma_correcciones[0];
	int velD = velBaseD + suma_correcciones[1];

	// Limitar velocidad del motor derecho y selecciona la dirección.
	if (velD > 255) {
		velD = 255;
		pinD = MOTOR_DERECHO_ADELANTE;
		motor_derecho_direccion_adelante = true;
	} else if (velD < 0) {
		velD = abs(velD);
		if (velD > 255) {
			velD = 255;
		}
		pinD = MOTOR_DERECHO_ATRAS;
		motor_derecho_direccion_adelante = false;
	}

	// Limitar velocidad del motor izquierdo y selecciona la dirección.
	if (velI > 255) {
		velI = 255;
		pinI = MOTOR_IZQUIERDO_ADELANTE;
		motor_izquierdo_direccion_adelante = true;
	} else if (velI < 0) {
		velI = abs(velI);
		if (velI > 255) {
			velI = 255;
		}
		pinI = MOTOR_IZQUIERDO_ATRAS;
		motor_izquierdo_direccion_adelante = false;
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
	reset_correcciones();

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

/**
* Resetea los ticks restantes de casilla cuando pierde uno de las paredes laterales para minimizar el error
*/
void reset_ticks_wall(){
	if(back_wall_reset){
		ticksDerecho = TICKS_CM*CM_CASILLA-TICKS_CM*8.0f;
		ticksIzquierdo = TICKS_CM*CM_CASILLA-TICKS_CM*8.0f;
	}else{
		ticksDerecho = TICKS_CM*CM_CASILLA-TICKS_CM*3.8f;
		ticksIzquierdo = TICKS_CM*CM_CASILLA-TICKS_CM*3.8f;
	}
}
