/////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones para //
// la calibración de todos los sensores del robot. //
/////////////////////////////////////////////////////

/**
* Realiza una calibración de los sensores frontales.
*
* NOTE: El robot debe de estar situado perpendicularmente a una pared frontal,
*       con el eje de las ruedas en la línea imaginaria de los postes de la celda.
*       Ej.: TODO: añadir link de foto de la wiki del apartado de calibración.
*/
void calibrate_front(){
  const float cm_calibracion = 7.5;
  int ticks_movimiento = cm_calibracion * TICKS_CM;
  ticksDerecho = 0;
  ticksIzquierdo = 0;
  int velI = 80;
  int velD = 80;
  int sensor_values[NUM_SENSORES_FRONTALES];
  do{
    if(ticksIzquierdo >= ticks_movimiento){
      velI = 0;
    }
    if(ticksDerecho >= ticks_movimiento){
      velD = 0;
    }
    set_speed(velI, velD);
    read_fronts_raw(sensor_values);
    for (int sensor = 0; sensor < NUM_SENSORES_FRONTALES; sensor++) {
      if(sensor_values[sensor] < minValoresFrontales[sensor]){
        minValoresFrontales[sensor] = sensor_values[sensor];
      }
      if(sensor_values[sensor] > maxValoresFrontales[sensor]){
        maxValoresFrontales[sensor] = sensor_values[sensor];
      }
    }
  }while(ticksDerecho < ticks_movimiento || ticksIzquierdo < ticks_movimiento);
  stop(-80,-80);
}

