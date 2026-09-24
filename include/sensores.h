#ifndef SENSORES_H
#define SENSORES_H

struct LecturasSensores {
    long frente;
    long izquierda;
    long derecha;
    bool fc51Detecta;

    // Monitoreo de batería
    float voltajeBateria;
};

void iniciarSensores();

long medirDistancia(int trigPin, int echoPin);

// Lectura del voltaje de batería
float leerVoltajeBateria();

LecturasSensores leerSensores();

void imprimirLecturas(const LecturasSensores& lecturas);

#endif