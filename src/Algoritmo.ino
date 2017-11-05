/////////////////////////////////////////////////////////////////////////////
// Este fichero contendrá todas las funciones de manejo del algoritmo      //
// de resolución, así como gestión de orientaciones, posición de casillas, //
// mapeado, etc...                                                         //
/////////////////////////////////////////////////////////////////////////////

/**
 * Efectua una lectura no filtrada de los sensores y actualiza los indicadores de
 * pared de la celda actual.
 */
void actualiza_casilla_actual(){
  read_fronts(false);
  read_sides(false);
  casilla_actual[IZQUIERDA]     = valores_sensores_laterales[0] >= umbral_deteccion_lateral;
  casilla_actual[FRONTAL]       = valores_sensores_frontales[0] >= umbral_deteccion_frontal && valores_sensores_frontales[1] >= umbral_deteccion_frontal;
  casilla_actual[DERECHA]       = valores_sensores_laterales[1] >= umbral_deteccion_lateral;
}

/**
 * Actualiza los indicadores de pared de la celda actual y
 * cuenta las posibles salidas que existen. Cuantas "no paredes" hay.
 * @return Numero de salidas de la casilla (paredes faltantes)
 */
byte contar_salidas(){
  byte salidas = 0;
  actualiza_casilla_actual();
  for(byte pared = 0; pared<3; pared++){
    if(!casilla_actual[pared]){
      salidas++;
    }
  }
  return salidas;
}
