///////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones de lectura         //
// de sensores, encoders y botones que se usarán en el programa. //
///////////////////////////////////////////////////////////////////

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
