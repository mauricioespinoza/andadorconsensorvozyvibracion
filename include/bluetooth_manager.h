#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include "sensores.h"
#include "alertas.h"

void iniciarBluetooth();
void enviarLecturasBluetooth(const LecturasSensores& lecturas, TipoAlerta alerta);

#endif