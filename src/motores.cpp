#include <Arduino.h>

#include "config.h"
#include "motores.h"

const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

const int CANAL_MOTOR_L = 0;
const int CANAL_MOTOR_R = 1;

void iniciarMotores() {
    ledcSetup(CANAL_MOTOR_L, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(CANAL_MOTOR_R, PWM_FREQ, PWM_RESOLUTION);

    ledcAttachPin(MOTOR_L, CANAL_MOTOR_L);
    ledcAttachPin(MOTOR_R, CANAL_MOTOR_R);

    apagarMotores();
}

void apagarMotores() {
    ledcWrite(CANAL_MOTOR_L, 0);
    ledcWrite(CANAL_MOTOR_R, 0);
}

void motorIzquierdo(int potencia) {
    ledcWrite(CANAL_MOTOR_L, potencia);
}

void motorDerecho(int potencia) {
    ledcWrite(CANAL_MOTOR_R, potencia);
}

void ambosMotores(int potencia) {
    ledcWrite(CANAL_MOTOR_L, potencia);
    ledcWrite(CANAL_MOTOR_R, potencia);
}