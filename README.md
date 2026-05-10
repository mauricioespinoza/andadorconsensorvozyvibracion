# Andador Asistido ESP32

Proyecto de andador asistido basado en ESP32, sensores ultrasónicos, sensor infrarrojo, alertas de audio mediante DFPlayer Mini y motores vibradores.

Esta versión corresponde al código preparado para pruebas de montaje real sobre el andador.

## Estado actual

Versión funcional de testing con:

- Lectura de sensores ultrasónicos HC-SR04:
  - Frontal
  - Izquierdo
  - Derecho
- Lectura de sensor infrarrojo FC-51
- Reproducción de alertas de audio mediante DFPlayer Mini y PAM8403
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
- Amplificador con potenciometro (regulador volumen) PAM8403
- 2 motores vibradores
- Transistores 2N2222 y diodos 1N4007 para control de motores
- Fuente: 
    Bateria 18650 + 
    TP4056 (modulo carga)+ 
    Interruptor ON/OFF + 
    Mt3608 (modulo elevador de voltaje >A 5 V con pico maximo 1A)


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

## Configuración PlatformIO

Archivo platformio.ini esperado:

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

monitor_speed = 115200
upload_speed = 115200

lib_deps =
    dfrobot/DFRobotDFPlayerMini

## Pines principales

Revisar include/config.h para la configuración vigente.

Configuración utilizada

#define TRIG_F 18
#define ECHO_F 19

#define TRIG_L 4
#define ECHO_L 5

#define TRIG_R 21
#define ECHO_R 22

#define FC51 25

#define MOTOR_L 26
#define MOTOR_R 27

#define DF_RX 16
#define DF_TX 17
Audios DFPlayer

Los audios deben estar en la microSD en la siguiente estructura:

/MP3/0001.mp3
/MP3/0002.mp3
/MP3/0003.mp3
...
/MP3/0010.mp3

El código usa:

dfplayer.playMp3Folder(track);

Por eso los nombres deben respetar el formato 0001.mp3, 0002.mp3, etc.

## Lógica de alertas

La lógica actual trabaja con una alerta dominante.

Prioridad general:

FC-51 / emergencia
Frente fuerte
Frente medio o leve
Ambos laterales
Lateral más cercano

El audio tiene una ventana de bloqueo para evitar superposición:

Mientras un audio está activo, no se reproduce otro durante algunos segundos,
salvo que se detecte emergencia.

## Motores vibradores

Los motores están controlados por PWM.

Reglas actuales:

Alerta leve/media: solo audio
Frente fuerte: vibran ambos motores
Izquierda fuerte: vibra motor izquierdo
Derecha fuerte: vibra motor derecho
Emergencia: vibran ambos motores
Vibración limitada a una ventana de tiempo
Pausa entre vibraciones para evitar consumo excesivo y vibración constante


## Ejecución

> Compilar:

pio run

> Subir firmware:

pio run -t upload

> Subir y abrir monitor serial:

pio run -t upload -t monitor

> Monitor serial:

115200 baud

## Consideraciones

Durante las pruebas se observaron los siguientes puntos:

Los motores pueden afectar el audio si la alimentación no es suficiente.
El DFPlayer Mini puede requerir algunos segundos después del arranque antes de reproducir.
El sensor FC-51 puede moverse con las vibraciones, por lo que debe quedar bien fijado.
Los HC-SR04 laterales pueden detectar la estructura o la carcasa si quedan muy cerca.
Para uso, se recomienda mantener un entorno despejado de transito, disminuir volumen si se considera invasivo.
