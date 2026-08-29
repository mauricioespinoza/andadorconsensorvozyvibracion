#ifndef CONFIG_H
#define CONFIG_H

// =====================================================
// AUDIO - DFPLAYER MINI
// Sector derecho superior
// =====================================================

// ESP32 recibe desde TX del DFPlayer
#define DF_RX 23

// ESP32 transmite hacia RX del DFPlayer
// Lleva resistencia de 1 kΩ en serie
#define DF_TX 22


// =====================================================
// MOTORES VIBRADORES
// =====================================================

// Motor izquierdo del andador
// El cable llega al lado derecho de la placa
#define MOTOR_L 18

// Motor derecho del andador
#define MOTOR_R 25


// =====================================================
// SENSOR ULTRASÓNICO FRONTAL - HC-SR04
// =====================================================

#define TRIG_F 14
#define ECHO_F 13


// =====================================================
// SENSOR ULTRASÓNICO IZQUIERDO - HC-SR04
// El cable llega al lado derecho de la placa
// =====================================================

#define TRIG_L 21
#define ECHO_L 19


// =====================================================
// SENSOR ULTRASÓNICO DERECHO - HC-SR04
// Pendiente de confirmar uno a uno
// =====================================================

#define TRIG_R 32
#define ECHO_R 33


// =====================================================
// SENSOR INFRARROJO FRONTAL - FC-51
// =====================================================

#define FC51 27


// =====================================================
// MONITOREO DE BATERÍA
// Divisor resistivo 100 kΩ / 100 kΩ
// Punto medio conectado al ADC
// =====================================================

#define BATTERY_ADC 35


// =====================================================
// UMBRALES DE DISTANCIA (cm)
// =====================================================

const int UMBRAL_LEVE = 100;
const int UMBRAL_MEDIA = 50;
const int UMBRAL_FUERTE = 15;


#endif