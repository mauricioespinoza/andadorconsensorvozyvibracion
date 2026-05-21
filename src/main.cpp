#include <Arduino.h>

#include "sensores.h"
#include "alertas.h"
#include "audio.h"
#include "motores.h"

// ===============================
// CONFIGURACIÓN GENERAL
// ===============================

const unsigned long BLOQUEO_AUDIO_MS = 5000;

const unsigned long DURACION_VIBRACION_MS = 3000;
const unsigned long BLOQUEO_VIBRACION_MS = 5000;

// Potencia baja para pruebas
const int POTENCIA_MOTOR_TEST = 70;

// ===============================
// CONTROL AUDIO
// ===============================

TipoAlerta audioActual = SIN_ALERTA;
unsigned long inicioAudioActual = 0;

bool audioEnVentanaActiva() {
    return audioActual != SIN_ALERTA &&
           millis() - inicioAudioActual < BLOQUEO_AUDIO_MS;
}

bool puedeReproducirAudio(TipoAlerta nuevaAlerta) {
    if (nuevaAlerta == SIN_ALERTA) {
        return false;
    }

    if (nuevaAlerta == EMERGENCIA && audioActual != EMERGENCIA) {
        return true;
    }

    if (audioEnVentanaActiva()) {
        return false;
    }

    return true;
}

void reproducirAudioControlado(TipoAlerta alerta) {
    if (!puedeReproducirAudio(alerta)) {
        return;
    }

    audioActual = alerta;
    inicioAudioActual = millis();

    Serial.print("Audio controlado -> alerta: ");
    Serial.println(alerta);

    reproducirAudio(alerta);
}

void resetAudioSiNoHayAlerta(TipoAlerta alerta) {
    if (alerta == SIN_ALERTA) {
        audioActual = SIN_ALERTA;
        inicioAudioActual = 0;
    }
}

// ===============================
// CONTROL MOTORES
// ===============================

TipoAlerta alertaMotorActual = SIN_ALERTA;
unsigned long inicioVibracion = 0;
unsigned long ultimaVibracion = 0;
bool vibrando = false;

bool esAlertaFuerte(TipoAlerta alerta) {
    return alerta == FRENTE_FUERTE ||
           alerta == IZQUIERDA_FUERTE ||
           alerta == DERECHA_FUERTE ||
           alerta == EMERGENCIA;
}

void activarMotoresPorAlerta(TipoAlerta alerta) {
    apagarMotores();

    switch (alerta) {
        case FRENTE_FUERTE:
        case EMERGENCIA:
            ambosMotores(POTENCIA_MOTOR_TEST);
            break;

        case IZQUIERDA_FUERTE:
            motorIzquierdo(POTENCIA_MOTOR_TEST);
            break;

        case DERECHA_FUERTE:
            motorDerecho(POTENCIA_MOTOR_TEST);
            break;

        default:
            apagarMotores();
            break;
    }
}

void ejecutarMotores(TipoAlerta alerta) {
    unsigned long ahora = millis();

    if (!esAlertaFuerte(alerta)) {
        apagarMotores();
        alertaMotorActual = SIN_ALERTA;
        vibrando = false;
        return;
    }

    bool puedeIniciarVibracion =
        !vibrando &&
        (
            alerta != alertaMotorActual ||
            ahora - ultimaVibracion >= BLOQUEO_VIBRACION_MS ||
            alerta == EMERGENCIA
        );

    if (puedeIniciarVibracion) {
        alertaMotorActual = alerta;
        inicioVibracion = ahora;
        ultimaVibracion = ahora;
        vibrando = true;

        Serial.print("Vibracion controlada -> alerta: ");
        Serial.println(alerta);
    }

    if (vibrando) {
        if (ahora - inicioVibracion <= DURACION_VIBRACION_MS) {
            activarMotoresPorAlerta(alertaMotorActual);
        } else {
            apagarMotores();
            vibrando = false;
        }
    } else {
        apagarMotores();
    }
}

// ===============================
// SETUP
// ===============================

void setup() {
    Serial.begin(115200);
    delay(1500);

    Serial.println("==============================");
    Serial.println("ANDADOR ASISTIDO");
    Serial.println("AUDIO CONTROLADO");
    Serial.println("MOTORES SOLO ALERTA FUERTE");
    Serial.println("==============================");

    iniciarSensores();
    iniciarMotores();
    iniciarAudio();

    apagarMotores();

    delay(2000);

    Serial.println("Sistema iniciado");
}

// ===============================
// LOOP
// ===============================

void loop() {
    LecturasSensores lecturas = leerSensores();

    imprimirLecturas(lecturas);

    TipoAlerta alerta = determinarAlerta(lecturas);

    Serial.print("Alerta dominante: ");
    Serial.println(alerta);

    reproducirAudioControlado(alerta);

    ejecutarMotores(alerta);

    resetAudioSiNoHayAlerta(alerta);

    delay(500);
}