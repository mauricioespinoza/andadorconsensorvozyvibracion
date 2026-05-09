#include <Arduino.h>
#include "config.h"
#include "alertas.h"

bool obstaculo(long distancia) {
    return distancia > 0 && distancia < UMBRAL_LEVE;
}

TipoAlerta alertaPorFrente(long distancia) {
    if (distancia < UMBRAL_FUERTE) return FRENTE_FUERTE;
    if (distancia < UMBRAL_MEDIA) return FRENTE_MEDIA;
    return FRENTE_LEVE;
}

TipoAlerta alertaPorIzquierda(long distancia) {
    if (distancia < UMBRAL_FUERTE) return IZQUIERDA_FUERTE;
    if (distancia < UMBRAL_MEDIA) return IZQUIERDA_MEDIA;
    return IZQUIERDA_LEVE;
}

TipoAlerta alertaPorDerecha(long distancia) {
    if (distancia < UMBRAL_FUERTE) return DERECHA_FUERTE;
    if (distancia < UMBRAL_MEDIA) return DERECHA_MEDIA;
    return DERECHA_LEVE;
}

TipoAlerta determinarAlerta(const LecturasSensores& lecturas) {
    if (lecturas.fc51Detecta) {
        return EMERGENCIA;
    }

    bool obsF = obstaculo(lecturas.frente);
    bool obsL = obstaculo(lecturas.izquierda);
    bool obsR = obstaculo(lecturas.derecha);

    if (obsF) {
        return alertaPorFrente(lecturas.frente);
    }

    if (obsL && obsR) {
        long dMin = min(lecturas.izquierda, lecturas.derecha);

        if (dMin < UMBRAL_FUERTE) return EMERGENCIA;
        if (dMin < UMBRAL_MEDIA) return IZQUIERDA_MEDIA;
        return IZQUIERDA_LEVE;
    }

    if (obsL) {
        return alertaPorIzquierda(lecturas.izquierda);
    }

    if (obsR) {
        return alertaPorDerecha(lecturas.derecha);
    }

    return SIN_ALERTA;
}