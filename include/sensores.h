#ifndef SENSORES_H
#define SENSORES_H

struct LecturasSensores {
    long frente;
    long izquierda;
    long derecha;
    bool fc51Detecta;
};

void iniciarSensores();

long medirDistancia(int trigPin, int echoPin);

LecturasSensores leerSensores();

void imprimirLecturas(const LecturasSensores& lecturas);

#endif