# Andador Asistido ESP32 - Rama Testing

Este branch contiene una versión de pruebas del proyecto **Andador Asistido con ESP32**, sensores ultrasónicos, sensor infrarrojo, módulo DFPlayer Mini y motores vibradores.

La finalidad de esta rama es validar el comportamiento del sistema antes de preparar una versión más estable/productiva en `main`.

## Estado actual

Versión funcional de testing con:

- Lectura de sensores ultrasónicos HC-SR04:
  - Frontal
  - Izquierdo
  - Derecho
- Lectura de sensor infrarrojo FC-51
- Reproducción de alertas de audio mediante DFPlayer Mini
- Audio desde carpeta `/MP3` de la microSD
- Control de motores vibradores por PWM
- Lógica de alerta dominante
- Bloqueo de audio para evitar superposición
- Vibración solo en alertas fuertes o emergencia
- Duración limitada de vibración

## Hardware utilizado

- ESP32 Dev Module
- 3 sensores HC-SR04
- 1 sensor infrarrojo FC-51
- DFPlayer Mini
- MicroSD con audios MP3
- 2 motores vibradores
- Transistores para control de motores
- Fuente actual de pruebas: USB 3.0

> Nota: En esta versión de pruebas, el sistema aún se alimenta desde USB. La alimentación definitiva con batería/regulador queda pendiente para montaje final.

## Estructura del proyecto

Proyecto desarrollado con **PlatformIO** usando framework Arduino.

```text
include/
├── config.h
├── sensores.h
├── alertas.h
├── audio.h
└── motores.h

src/
├── main.cpp
├── sensores.cpp
├── alertas.cpp
├── audio.cpp
└── motores.cpp