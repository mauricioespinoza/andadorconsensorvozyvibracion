#ifndef CONFIG_H
#define CONFIG_H

//audio
#define DF_RX 16 // ESP32 recibe desde TX del DFPlayer
#define DF_TX 17 // ESP32 transmite hacia RX del DFPlayer

// Motores
#define MOTOR_L 26
#define MOTOR_R 27

// Sensor frontal
#define TRIG_F 18
#define ECHO_F 19

// Sensor izquierdo
#define TRIG_L 4
#define ECHO_L 5

// Sensor derecho
#define TRIG_R 21
#define ECHO_R 22

// Sensor infrarrojo frontal
#define FC51 25

// Umbrales
const int UMBRAL_LEVE = 20;
const int UMBRAL_MEDIA = 10;
const int UMBRAL_FUERTE = 5;

#endif