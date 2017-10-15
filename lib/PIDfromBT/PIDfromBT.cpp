
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <PIDfromBT.h>
PIDfromBT::PIDfromBT(float* inKp, float* inKi, float* inKd, int* inVel, float* inIdeal, int inMinIdeal, int inMaxIdeal, bool* inacelerar, float* inaumentoP, float* inaumentoD, float* inmaxAumento, float* invelIncremento, float* inmaxError, bool debug)
{
  kp = inKp; // P
  ki = inKi; // I
  kd = inKd; // D
  vel = inVel; // V

  acelerar  = inacelerar; // a
  aumentoP = inaumentoP; // p
  aumentoD = inaumentoD; // d
  maxAumento = inmaxAumento; // m
  velIncremento = invelIncremento; // i
  maxError = inmaxError; // e

  ideal = inIdeal; // V
  maxIdeal = inMaxIdeal; // V
  minIdeal = inMinIdeal; // V

  print = debug;
  valor = "";
  letra = ' ';
  last_millis = 0;
}
PIDfromBT::PIDfromBT(float* inKp, float* inKi, float* inKd, int* inVel, bool* inacelerar, float* inaumentoP, float* inaumentoD, float* inmaxAumento, float* invelIncremento, float* inmaxError, bool debug)
{
  kp = inKp; // P
  ki = inKi; // I
  kd = inKd; // D
  vel = inVel; // V

  acelerar  = inacelerar; // a
  aumentoP = inaumentoP; // p
  aumentoD = inaumentoD; // d
  maxAumento = inmaxAumento; // m
  velIncremento = invelIncremento; // i
  maxError = inmaxError; // e

  print = debug;
  valor = "";
  letra = ' ';
  last_millis = 0;
}

PIDfromBT::PIDfromBT(float* inKp, float* inKi, float* inKd, int* inVel, bool debug)
{
  kp = inKp; // P
  ki = inKi; // I
  kd = inKd; // D
  vel = inVel; // V

  print = debug;
  valor = "";
  letra = ' ';
  last_millis = 0;
}

PIDfromBT::PIDfromBT(float* inKp, float* inKi, float* inKd, int* inVel, float* inIdeal, int inMinIdeal, int inMaxIdeal, bool debug)
{
  kp = inKp; // P
  ki = inKi; // I
  kd = inKd; // D
  vel = inVel; // V

  ideal = inIdeal; // V
  maxIdeal = inMaxIdeal; // V
  minIdeal = inMinIdeal; // V

  print = debug;
  valor = "";
  letra = ' ';
  last_millis = 0;
}

bool PIDfromBT::update()
{
  if (Serial.available()) {
    last_millis = millis();
    byte readByte = Serial.read();
    if ((isDigit((char)readByte) || (char)readByte == '.' || (char)readByte == '-')) {
      valor += (char)readByte;

    } else {
      if (letra == ' ') {
        letra = (char)readByte;
      } else {
        switch (letra) {
          case 'C':
            if (valor.toInt() > 0) {
              if (*carrilObjetivo < maxCarriles-1) {
                *carrilObjetivo = *carrilObjetivo + 1;
              }
            } else {
              if (*carrilObjetivo > 0) {
                *carrilObjetivo = *carrilObjetivo - 1;
              }
            }
            if (print) {
              Serial.println("Carril: " + (String)(*carrilObjetivo + 1));
            }
            break;
          case 'P':
            *kp = valor.toFloat();
            if (print) {
              Serial.println("Kp: " + valor);
            }
            break;
          case 'I':
            *ki = valor.toFloat();
            if (print) {
              Serial.println("Ki: " + valor);
            }
            break;
          case 'D':
            *kd = valor.toFloat();
            if (print) {
              Serial.println("Kd: " + valor);
            }
            break;
          case 'V':
            *vel = valor.toInt();
            if (print && (last_millis_vel == 0 || (millis() - last_millis_vel) > 500)) {
              Serial.println("Vel: " + valor);
              last_millis_vel = millis();
            }
            break;
          case 'L':
            *ideal = map(valor.toInt(), -500, 500, minIdeal, maxIdeal);;
            if (print && (last_millis_ideal == 0 || (millis() - last_millis_ideal) > 500)) {
              Serial.println("Ideal: " + String(*ideal));
              last_millis_ideal = millis();
            }
            break;
          case 'a':
            acc_activada = valor.toInt();
            *acelerar = (acc_activada == 1) ? true : false;
            if (print) {
              // Serial.println("Acc: " + (acc_activada == 1) ? "ON" : "OFF");
            }
            break;
          case 'p':
            *aumentoP = valor.toFloat();
            if (print) {
              Serial.println("Acc. Kp: " + valor);
            }
            break;
          case 'd':
            *aumentoD = valor.toFloat();
            if (print) {
              Serial.println("Acc. Kd: " + valor);
            }
            break;
          case 'm':
            *maxAumento = valor.toFloat();
            if (print) {
              Serial.println("Máx. Vel.: " + valor);
            }
            break;
          case 'i':
            *velIncremento = valor.toFloat();
            if (print) {
              Serial.println("Vel. Aum.: " + valor);
            }
            break;
          case 'e':
            *maxError = valor.toFloat();
            if (print) {
              Serial.println("Máx. Error: " + valor);
            }
            break;
        }
        letra = (char)readByte;
        valor = "";
      }
    }
  } else if ((millis() - last_millis) > 5) {
    if (letra != ' ' && valor != "") {
      switch (letra) {
        case 'C':
          if (valor.toInt() > 0) {
            if (*carrilObjetivo < maxCarriles-1) {
              *carrilObjetivo = *carrilObjetivo + 1;
            }
          } else {
            if (*carrilObjetivo > 0) {
              *carrilObjetivo = *carrilObjetivo - 1;
            }
          }
          if (print) {
            Serial.println("Carril: " + (String)(*carrilObjetivo + 1));
          }
          break;
        case 'P':
          *kp = valor.toFloat();
          if (print) {
            Serial.println("Kp: " + valor);
          }
          break;
        case 'I':
          *ki = valor.toFloat();
          if (print) {
            Serial.println("Ki: " + valor);
          }
          break;
        case 'D':
          *kd = valor.toFloat();
          if (print) {
            Serial.println("Kd: " + valor);
          }
          break;
        case 'V':
          *vel = valor.toInt();
          if (print && (last_millis_vel == 0 || (millis() - last_millis_vel) > 500)) {
            Serial.println("Vel: " + valor);
            last_millis_vel = millis();
          }
          break;
        case 'L':
          *ideal = map(valor.toInt(), -500, 500, minIdeal, maxIdeal);;
          if (print && (last_millis_ideal == 0 || (millis() - last_millis_ideal) > 500)) {
            Serial.println("Ideal: " + String(*ideal));
            last_millis_ideal = millis();
          }
          break;
        case 'a':
          acc_activada = valor.toInt();
          *acelerar = (acc_activada == 1) ? true : false;
          if (print) {
            if (acc_activada == 1)
              Serial.println("Acc: ON");
            else
              Serial.println("Acc: OFF");
          }
          break;
        case 'p':
          *aumentoP = valor.toFloat();
          if (print) {
            Serial.println("Acc. Kp: " + valor);
          }
          break;
        case 'd':
          *aumentoD = valor.toFloat();
          if (print) {
            Serial.println("Acc. Kd: " + valor);
          }
          break;
        case 'm':
          *maxAumento = valor.toFloat();
          if (print) {
            Serial.println("Máx. Vel.: " + valor);
          }
          break;
        case 'i':
          *velIncremento = valor.toFloat();
          if (print) {
            Serial.println("Vel. Aum.: " + valor);
          }
          break;
        case 'e':
          *maxError = valor.toFloat();
          if (print) {
            Serial.println("Máx. Error: " + valor);
          }
          break;
      }
      letra = ' ';
      valor = "";
    }
  }
}

void PIDfromBT::setMinIdeal(int inMinIdeal) {
  minIdeal = inMinIdeal;
}
void PIDfromBT::setMaxIdeal(int inMaxIdeal) {
  maxIdeal = inMaxIdeal;
}
void PIDfromBT::setCarriles(bool inCarrilesEnabled) {
  carrilesEnabled = inCarrilesEnabled;
}
void PIDfromBT::setCarriles(short* inCarrilObjetivo, short inMaxCarriles) {
  carrilObjetivo = inCarrilObjetivo;
  maxCarriles = inMaxCarriles;
}
