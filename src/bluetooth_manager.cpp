#include <Arduino.h>
#include "BluetoothSerial.h"
#include "bluetooth_manager.h"

BluetoothSerial SerialBT;

const char* nombreAlerta(TipoAlerta alerta) {
    switch (alerta) {
        case SIN_ALERTA: return "SIN_ALERTA";

        case FRENTE_LEVE: return "FRENTE_LEVE";
        case FRENTE_MEDIA: return "FRENTE_MEDIA";
        case FRENTE_FUERTE: return "FRENTE_FUERTE";

        case IZQUIERDA_LEVE: return "IZQUIERDA_LEVE";
        case IZQUIERDA_MEDIA: return "IZQUIERDA_MEDIA";
        case IZQUIERDA_FUERTE: return "IZQUIERDA_FUERTE";

        case DERECHA_LEVE: return "DERECHA_LEVE";
        case DERECHA_MEDIA: return "DERECHA_MEDIA";
        case DERECHA_FUERTE: return "DERECHA_FUERTE";

        case EMERGENCIA: return "EMERGENCIA";

        default: return "DESCONOCIDA";
    }
}

void iniciarBluetooth() {
    SerialBT.begin("ANDADOR_BT");

    Serial.println("==============================");
    Serial.println("BLUETOOTH INICIADO");
    Serial.println("Nombre: ANDADOR_BT");
    Serial.println("==============================");
}

void enviarLecturasBluetooth(const LecturasSensores& lecturas, TipoAlerta alerta) {

    if (!SerialBT.hasClient()) {
        return;
    }

    SerialBT.println("====================");
    SerialBT.println("ANDADOR ASISTIDO");

    SerialBT.print("Frente: ");
    SerialBT.print(lecturas.frente);
    SerialBT.println(" cm");

    SerialBT.print("Izquierda: ");
    SerialBT.print(lecturas.izquierda);
    SerialBT.println(" cm");

    SerialBT.print("Derecha: ");
    SerialBT.print(lecturas.derecha);
    SerialBT.println(" cm");

    SerialBT.print("FC51: ");
    SerialBT.println(
        lecturas.fc51Detecta ? "OBSTACULO" : "LIBRE"
    );

    SerialBT.print("Bateria: ");
    SerialBT.print(lecturas.voltajeBateria, 2);
    SerialBT.println(" V");

    SerialBT.print("Alerta: ");
    SerialBT.print((int)alerta);
    SerialBT.print(" - ");
    SerialBT.println(nombreAlerta(alerta));

    SerialBT.println();
}