#include <Arduino.h>
#include "BluetoothSerial.h"
#include "bluetooth_manager.h"

BluetoothSerial SerialBT;

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
    SerialBT.println((int)alerta);

    SerialBT.println();
}