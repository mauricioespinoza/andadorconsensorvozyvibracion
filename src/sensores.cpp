#include <Arduino.h>

#include "config.h"
#include "sensores.h"

void iniciarSensores() {
    pinMode(TRIG_F, OUTPUT);
    pinMode(ECHO_F, INPUT);

    pinMode(TRIG_L, OUTPUT);
    pinMode(ECHO_L, INPUT);

    pinMode(TRIG_R, OUTPUT);
    pinMode(ECHO_R, INPUT);

    pinMode(FC51, INPUT);
}

long medirDistancia(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    long duracion = pulseIn(echoPin, HIGH, 30000);

    if (duracion == 0) {
        return -1;
    }

    return duracion * 0.034 / 2;
}

LecturasSensores leerSensores() {
    LecturasSensores lecturas;

    lecturas.fc51Detecta = digitalRead(FC51) == LOW;

    lecturas.frente = medirDistancia(TRIG_F, ECHO_F);
    delay(40);

    lecturas.izquierda = medirDistancia(TRIG_L, ECHO_L);
    delay(40);

    lecturas.derecha = medirDistancia(TRIG_R, ECHO_R);
    delay(40);

    return lecturas;
}

void imprimirLecturas(const LecturasSensores& lecturas) {
    Serial.print("Frente: ");
    Serial.print(lecturas.frente);

    Serial.print(" cm | Izquierda: ");
    Serial.print(lecturas.izquierda);

    Serial.print(" cm | Derecha: ");
    Serial.print(lecturas.derecha);

    Serial.print(" cm | FC51: ");
    Serial.println(lecturas.fc51Detecta ? "DETECTA" : "LIBRE");
}