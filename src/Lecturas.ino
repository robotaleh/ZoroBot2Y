///////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones de lectura         //
// de sensores, encoders y botones que se usarán en el programa. //
///////////////////////////////////////////////////////////////////

/**
 * Lectura directa de los sensores frontales
 * @param lateralValues Array donde se almacenarán los valores leídos
 */
void lectura_frontal_raw(short frontalValues[]){
  for(byte sensor = 0; sensor<NUM_SENSORES_FRONTALES;sensor++){
    frontalValues[sensor] = analogRead(sensores_frontales[sensor]);
  }
}

/**
 * Lectura directa de los sensores laterales
 * @param lateralValues Array donde se almacenarán los valores leídos
 */
void lectura_lateral_raw(int lateralValues[]){
  for(byte sensor = 0; sensor<NUM_SENSORES_LATERALES;sensor++){
    lateralValues[sensor] = analogRead(sensores_laterales[sensor]);
  }
}


/**
* Función a ejecutar con cada tick del Canal A del motor derecho
*/
void tick_encoder_derecho_A(){
  estado_encoder_derecho_A = !estado_encoder_derecho_A;
  if(!estado_encoder_derecho_A){
    if(estado_encoder_derecho_B){
      ticksDerecho--;
    }else{
      ticksDerecho++;
    }
  }else{
    if(!estado_encoder_derecho_B){
      ticksDerecho--;
    }else{
      ticksDerecho++;
    }
  }
}

/**
* Función a ejecutar con cada tick del Canal B del motor derecho
*/
void tick_encoder_derecho_B(){
  estado_encoder_derecho_B = !estado_encoder_derecho_B;
  if(!estado_encoder_derecho_B){
    if(!estado_encoder_derecho_A){
      ticksDerecho--;
    }else{
      ticksDerecho++;
    }
  }else{
    if(estado_encoder_derecho_A){
      ticksDerecho--;
    }else{
      ticksDerecho++;
    }
  }
}

/**
* Función a ejecutar con cada tick del Canal A del motor izquierdo
*/
void tick_encoder_izquierdo_A(){
  estado_encoder_izquierdo_A = !estado_encoder_izquierdo_A;
  if(!estado_encoder_izquierdo_A){
    if(estado_encoder_izquierdo_B){
      ticksIzquierdo++;
    }else{
      ticksIzquierdo--;
    }
  }else{
    if(!estado_encoder_izquierdo_B){
      ticksIzquierdo++;
    }else{
      ticksIzquierdo--;
    }
  }
}

/**
* Función a ejecutar con cada tick del Canal B del motor izquierdo
*/
void tick_encoder_izquierdo_B(){
  estado_encoder_izquierdo_B = !estado_encoder_izquierdo_B;
  if(!estado_encoder_izquierdo_B){
    if(!estado_encoder_izquierdo_A){
      ticksIzquierdo++;
    }else{
      ticksIzquierdo--;
    }
  }else{
    if(estado_encoder_izquierdo_A){
      ticksIzquierdo++;
    }else{
      ticksIzquierdo--;
    }
  }
}
