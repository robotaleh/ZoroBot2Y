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
