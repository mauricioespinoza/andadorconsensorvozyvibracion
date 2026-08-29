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

    // Prioridad máxima: FC-51
    if (lecturas.fc51Detecta) {
        return EMERGENCIA;
    }

    bool obsF = obstaculo(lecturas.frente);
    bool obsL = obstaculo(lecturas.izquierda);
    bool obsR = obstaculo(lecturas.derecha);

    // =========================
    // PRIORIDAD 1: FUERTE
    // =========================
    if (obsF && lecturas.frente < UMBRAL_FUERTE) {
        return FRENTE_FUERTE;
    }

    if (obsL && lecturas.izquierda < UMBRAL_FUERTE) {
        return IZQUIERDA_FUERTE;
    }

    if (obsR && lecturas.derecha < UMBRAL_FUERTE) {
        return DERECHA_FUERTE;
    }

    // =========================
    // PRIORIDAD 2: MEDIA
    // =========================
    if (obsF && lecturas.frente < UMBRAL_MEDIA) {
        return FRENTE_MEDIA;
    }

    if (obsL && lecturas.izquierda < UMBRAL_MEDIA) {
        return IZQUIERDA_MEDIA;
    }

    if (obsR && lecturas.derecha < UMBRAL_MEDIA) {
        return DERECHA_MEDIA;
    }

    // =========================
    // PRIORIDAD 3: LEVE
    // =========================
    if (obsF) {
        return FRENTE_LEVE;
    }

    if (obsL) {
        return IZQUIERDA_LEVE;
    }

    if (obsR) {
        return DERECHA_LEVE;
    }

    return SIN_ALERTA;
}