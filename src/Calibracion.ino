/////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones para //
// la calibración de todos los sensores del robot. //
/////////////////////////////////////////////////////

/**
* Inicia una calibración completa de los sensores de pared
*/
void calibrate_all(){
  if(CALIBRADO_LATERAL){
    calibrate_sides();
  }
  delay(50);
  if(CALIBRADO_FRONTAL){
    calibrate_front();
  }
}

/**
* Realiza una calibración de los sensores frontales.
*
* NOTE: El robot debe de estar situado perpendicularmente a una pared frontal,
*       con el eje de las ruedas en la línea imaginaria de los postes de la celda.
*       Ej.: TODO: añadir link de foto de la wiki del apartado de calibración.
*/
void calibrate_front(){
  const float cm_calibracion = 5;
  int ticks_movimiento = cm_calibracion * TICKS_CM;
  int velI = 50;
  int velD = 50;
  int sensor_values[NUM_SENSORES_FRONTALES];
  ticksDerecho = 0;
  ticksIzquierdo = 0;
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
  stop();
}

/**
* Realiza una calibración automática de los sensores laterales.
*
* NOTE: El robot debe de estar situado perpendicularmente a una pared frontal,
*       con el eje de las ruedas en la línea imaginaria de los postes de la celda.
*       Ej.: TODO: añadir link de foto de la wiki del apartado de calibración.
*/
void calibrate_sides(){
  int sensor_values[NUM_SENSORES_LATERALES];
  float ticksMotores[2];
  calcular_arco_giro(-20, ticksMotores);
  ticksIzquierdo = 0;
  ticksDerecho = 0;
  short velBaseI = 50;
  do {

    read_sides_raw(sensor_values);
    for(byte sensor=0; sensor<NUM_SENSORES_LATERALES; sensor++){
      if (sensor_values[sensor] < minValoresLaterales[sensor]) {
        minValoresLaterales[sensor] = sensor_values[sensor];
      }
      if (sensor_values[sensor] > maxValoresLaterales[sensor]) {
        maxValoresLaterales[sensor] = sensor_values[sensor];
      }
    }

    if (ticksIzquierdo >= ticksMotores[0] && ticksMotores[0]>0) {
      if(velBaseI > 0){
        velBaseI = -velBaseI;
        ticksMotores[0] = 0;
      }
    }
    set_speed(velBaseI, 0);
  } while (velBaseI>0 || ticksIzquierdo > 0);
  stop(-50,0);

  calcular_arco_giro(20, ticksMotores);
  ticksIzquierdo = 0;
  ticksDerecho = 0;
  short velBaseD = 50;
  do {

    read_sides_raw(sensor_values);
    for(byte sensor=0; sensor<NUM_SENSORES_LATERALES; sensor++){
      if (sensor_values[sensor] < minValoresLaterales[sensor]) {
        minValoresLaterales[sensor] = sensor_values[sensor];
      }
      if (sensor_values[sensor] > maxValoresLaterales[sensor]) {
        maxValoresLaterales[sensor] = sensor_values[sensor];
      }
    }

    if (ticksDerecho >= ticksMotores[1] && ticksMotores[1]>0) {
      if(velBaseD > 0){
        velBaseD = -velBaseD;
        ticksMotores[1] = 0;
      }
    }
    set_speed(0, velBaseD);
  } while (velBaseD > 0 || ticksDerecho > 0);
  stop(0,-50);
}
