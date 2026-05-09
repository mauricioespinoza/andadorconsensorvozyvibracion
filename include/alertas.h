#ifndef ALERTAS_H
#define ALERTAS_H

#include "sensores.h"

enum TipoAlerta {
    SIN_ALERTA = 0,
    FRENTE_LEVE = 1,
    FRENTE_MEDIA = 2,
    FRENTE_FUERTE = 3,
    IZQUIERDA_LEVE = 4,
    IZQUIERDA_MEDIA = 5,
    IZQUIERDA_FUERTE = 6,
    DERECHA_LEVE = 7,
    DERECHA_MEDIA = 8,
    DERECHA_FUERTE = 9,
    EMERGENCIA = 10
};

TipoAlerta determinarAlerta(const LecturasSensores& lecturas);

#endif