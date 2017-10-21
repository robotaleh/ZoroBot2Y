///////////////////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones referentes al algoritmo de PID //
///////////////////////////////////////////////////////////////////////////////
void PID(){
  if(velReal == 0 && started){
    PID_frontal();
  }else{
    PID_lateral();
  }
}

void PID_frontal() {
  read_fronts();

  float p[] = {0, 0};
  double i[] = {0, 0};
  float d[] = {0, 0};
  float error[] = {objetivo_frontal[0] - valores_sensores_frontales[0], objetivo_frontal[1] - valores_sensores_frontales[1]};

  for (int sensor = 0; sensor < 2; sensor++) {
    if ((valores_sensores_frontales[0] > 0 || valores_sensores_frontales[1] > 0)) {
      p[sensor] = kp_frontal * error[sensor];
      d[sensor] = kd_frontal * ((error[sensor] - ultimoError_frontal[sensor]) / (millis() - ultimoMillis_frontal[sensor]));
      ultimoMillis_frontal[sensor] = millis();
      ultimoError_frontal[sensor] = error[sensor];
    } else {
      ultimoMillis_frontal[sensor] = millis();
    }
    correccion_frontal[sensor] = p[sensor] + i[sensor] + d[sensor];
  }

}

void PID_lateral() {
  read_sides();

  if(valores_sensores_laterales[0] > 0 || valores_sensores_laterales[1] > 0){
    float p = 0;
    double i = 0;
    double d = 0;
    short error = 0;

    if(valores_sensores_laterales[0] > 0 && valores_sensores_laterales[1] > 0){
      objetivo_lateral[0] = 0;
      objetivo_lateral[1] = 0;
      error = valores_sensores_laterales[1] - valores_sensores_laterales[0];
      digitalWrite(LED_PIN, LOW);
    }else{
      digitalWrite(LED_PIN, HIGH);
      if(valores_sensores_laterales[0] > 0){
        if(objetivo_lateral[0] == 0){
          objetivo_lateral[0] = valores_sensores_laterales[0];
        }
        error = objetivo_lateral[0] - valores_sensores_laterales[0];
      }else if(valores_sensores_laterales[1] > 0){
        if(objetivo_lateral[1] == 0){
          objetivo_lateral[1] = valores_sensores_laterales[1];
        }
        error = -(objetivo_lateral[1] - valores_sensores_laterales[1]);
      }
    }

    p = kp_lateral * error;
    d = kd_lateral * (error - ultimoError_lateral);
    ultimoMillis_lateral = millis();

    if(abs(sum_error_lateral) < 5){
      sum_error_lateral += error;
    }
    if(velBase == 0){
      sum_error_lateral = 0;
    }
    i = ki_lateral * sum_error_lateral;

    ultimoError_lateral = error;
    correccion_lateral = p + i + d;
  }
}

/**
* Función para agrupar todas las correcciones derivadas del PID
* @param correcciones Array que contiene las correcciones a aplicar a cada motor
*/
void sumar_correcciones(float correcciones[]){
  correcciones[0] = correccion_frontal[0] - correccion_lateral;
  correcciones[1] = correccion_frontal[1] + correccion_lateral;
}

/**
 * Resetea todas las correcciones a 0 para deshacer los ajustes sobre el PID
 */
void reset_correcciones(){
  correccion_lateral = 0;
  correccion_frontal[0] = 0;
  correccion_frontal[1] = 0;
}
