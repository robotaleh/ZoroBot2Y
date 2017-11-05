///////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones de lectura         //
// de sensores, encoders y botones que se usarán en el programa. //
///////////////////////////////////////////////////////////////////

/**
* Lectura directa de los sensores frontales
* @param lateralValues Array donde se almacenarán los valores leídos
*/
void read_fronts_raw(short frontalValues[]){
  for(byte sensor = 0; sensor<NUM_SENSORES_FRONTALES;sensor++){
    frontalValues[sensor] = analogRead(sensores_frontales[sensor]);
  }
}

/**
* Lee los sensores frontales obteniendo los valores mapeados.
* @param filtrado  Indica si obtiene el valor filtrado
*                   o si mapea directamente el leído en ese momento.
*/
void read_fronts(bool filtrado){
  if(delay_sensores_frontales.check()){
    sumaValoresFrontales[0] = 0;
    sumaValoresFrontales[1] = 0;
    for(byte lectura = 1; lectura < NUM_HISTORIAL_SENSORES; lectura++){
      for(int sensor = 0; sensor<NUM_SENSORES_FRONTALES;sensor++){
        // Mueve los valores del indice actual al anterior
        filtrado_sensores_frontales[sensor][lectura -1] = filtrado_sensores_frontales[sensor][lectura];
        // Suma el índice anterior con los demás.
        sumaValoresFrontales[sensor] += filtrado_sensores_frontales[sensor][lectura-1];
      }
    }

    for(int sensor = 0; sensor<NUM_SENSORES_FRONTALES;sensor++){
      // Añade al final del array la lectura actual.
      filtrado_sensores_frontales[sensor][NUM_HISTORIAL_SENSORES - 1] = analogRead(sensores_frontales[sensor]);
      // Suma la última lectura
      sumaValoresFrontales[sensor] += filtrado_sensores_frontales[sensor][NUM_HISTORIAL_SENSORES - 1];
    }
    delay_sensores_frontales.reset();
  }

  for(int sensor = 0; sensor<NUM_SENSORES_FRONTALES;sensor++){
    if(filtrado){
      // Calcula la media de los valores sumados y los mapea a 0-255
      valores_sensores_frontales[sensor] = map((sumaValoresFrontales[sensor] / (float)NUM_HISTORIAL_SENSORES), minValoresFrontales[sensor],maxValoresFrontales[sensor], 0,255);
    }else{
      // Mapea la lectura directa de cada sensor
      valores_sensores_frontales[sensor] = map(analogRead(sensores_frontales[sensor]), minValoresFrontales[sensor],maxValoresFrontales[sensor], 0,255);
    }
  }
}

/**
* Función sobrecargada para simplificar la lectura filtrada de los sensores frontales.
*/
void read_fronts(){
  read_fronts(true);
}

/**
* Lectura directa de los sensores laterales
* @param lateralValues Array donde se almacenarán los valores leídos
*/
void read_sides_raw(int lateralValues[]){
  for(byte sensor = 0; sensor<NUM_SENSORES_LATERALES;sensor++){
    lateralValues[sensor] = analogRead(sensores_laterales[sensor]);
  }
}

/**
* Lee los sensores laterales obteniendo los valores mapeados.
* @param filtrado  Indica si obtiene el valor filtrado
*                   o si mapea directamente el leído en ese momento.
*/
void read_sides(bool filtrado){
  if(delay_sensores_laterales.check()){
    sumaValoresLaterales[0] = 0;
    sumaValoresLaterales[1] = 0;
    for(byte lectura = 1; lectura < NUM_HISTORIAL_SENSORES; lectura++){
      for(int sensor = 0; sensor<NUM_SENSORES_LATERALES;sensor++){
        // Mueve los valores del indice actual al anterior
        filtrado_sensores_laterales[sensor][lectura -1] = filtrado_sensores_laterales[sensor][lectura];
        // Suma el índice anterior con los demás.
        sumaValoresLaterales[sensor] += filtrado_sensores_laterales[sensor][lectura-1];
      }
    }

    for(int sensor = 0; sensor<NUM_SENSORES_LATERALES;sensor++){
      // Añade al final del array la lectura actual.
      filtrado_sensores_laterales[sensor][NUM_HISTORIAL_SENSORES - 1] = analogRead(sensores_laterales[sensor]);
      // Suma la última lectura
      sumaValoresLaterales[sensor] += filtrado_sensores_laterales[sensor][NUM_HISTORIAL_SENSORES - 1];
    }
    delay_sensores_laterales.reset();
  }

  for(int sensor = 0; sensor<NUM_SENSORES_LATERALES;sensor++){
    if(filtrado){
      // Calcula la media de los valores sumados y los mapea a 0-255
      valores_sensores_laterales[sensor] = map((sumaValoresLaterales[sensor] / (float)NUM_HISTORIAL_SENSORES), minValoresLaterales[sensor],maxValoresLaterales[sensor], 0,255);
    }else{
      // Mapea la lectura directa de cada sensor
      valores_sensores_laterales[sensor] = map(analogRead(sensores_laterales[sensor]), minValoresLaterales[sensor],maxValoresLaterales[sensor], 0,255);
    }
  }
}

/**
* Función sobrecargada para simplificar la lectura filtrada de los sensores laterales.
*/
void read_sides(){
  read_sides(true);
}

/**
* Función a ejecutar con cada tick del Canal B del motor derecho
*/
void tick_encoder_derecho_B(){
  if(motor_derecho_direccion_adelante){
    ticksDerecho++;
  }else{
    ticksDerecho--;
  }
}

/**
* Función a ejecutar con cada tick del Canal B del motor izquierdo
*/
void tick_encoder_izquierdo_B(){
  if(motor_izquierdo_direccion_adelante){
    ticksIzquierdo++;
  }else{
    ticksIzquierdo--;
  }
}
