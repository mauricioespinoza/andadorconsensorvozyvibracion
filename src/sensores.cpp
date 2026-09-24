#include <Arduino.h>

#include "config.h"
#include "sensores.h"

// =====================================================
// INICIALIZACIÓN DE SENSORES
// =====================================================

void iniciarSensores() {

    // HC-SR04 frontal
    pinMode(TRIG_F, OUTPUT);
    pinMode(ECHO_F, INPUT);

    // HC-SR04 izquierdo
    pinMode(TRIG_L, OUTPUT);
    pinMode(ECHO_L, INPUT);

    // HC-SR04 derecho
    pinMode(TRIG_R, OUTPUT);
    pinMode(ECHO_R, INPUT);

    // Sensor infrarrojo FC-51
    pinMode(FC51, INPUT);

    // Entrada analógica para monitoreo de batería
    pinMode(BATTERY_ADC, INPUT);
}


// =====================================================
// MEDICIÓN HC-SR04
// =====================================================

long medirDistancia(int trigPin, int echoPin) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    long duracion = pulseIn(echoPin, HIGH, 30000);

    // Si no se recibe eco
    if (duracion == 0) {
        return -1;
    }

    // Conversión tiempo -> distancia en centímetros
    return duracion * 0.034 / 2;
}


// =====================================================
// MEDICIÓN DE BATERÍA
// Divisor resistivo: 100 kΩ / 100 kΩ
// GPIO35 recibe aproximadamente la mitad del voltaje
// =====================================================

float leerVoltajeBateria() {

    int lecturaADC = analogRead(BATTERY_ADC);

    // ESP32: ADC de 12 bits
    // 0 - 4095 representa aproximadamente 0 - 3.3 V
    float voltajeADC = (lecturaADC / 4095.0) * 3.3;

    // Divisor 100k / 100k:
    // Vbateria = Vadc * 2
    float voltajeBateria = voltajeADC * 2.0;

    return voltajeBateria;
}


// =====================================================
// LECTURA GENERAL DE SENSORES
// =====================================================

LecturasSensores leerSensores() {

    LecturasSensores lecturas;

    // FC-51
    lecturas.fc51Detecta = digitalRead(FC51) == LOW;

    // HC-SR04 frontal
    lecturas.frente = medirDistancia(TRIG_F, ECHO_F);
    delay(40);

    // HC-SR04 izquierdo
    lecturas.izquierda = medirDistancia(TRIG_L, ECHO_L);
    delay(40);

    // HC-SR04 derecho
    lecturas.derecha = medirDistancia(TRIG_R, ECHO_R);
    delay(40);

    // Voltaje de batería
    lecturas.voltajeBateria = leerVoltajeBateria();

    return lecturas;
}


// =====================================================
// MONITOR SERIAL
// =====================================================

void imprimirLecturas(const LecturasSensores& lecturas) {

    Serial.print("Frente: ");
    Serial.print(lecturas.frente);

    Serial.print(" cm | Izquierda: ");
    Serial.print(lecturas.izquierda);

    Serial.print(" cm | Derecha: ");
    Serial.print(lecturas.derecha);

    Serial.print(" cm | FC51: ");
    Serial.print(lecturas.fc51Detecta ? "DETECTA" : "LIBRE");

    Serial.print(" | Bateria: ");
    Serial.print(lecturas.voltajeBateria, 2);
    Serial.println(" V");
}