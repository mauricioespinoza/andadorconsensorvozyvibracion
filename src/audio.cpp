#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

#include "config.h"
#include "audio.h"

HardwareSerial dfSerial(2);
DFRobotDFPlayerMini dfplayer;

bool audioDisponible = false;

void iniciarAudio() {
    Serial.println("Iniciando DFPlayer...");

    dfSerial.begin(9600, SERIAL_8N1, DF_RX, DF_TX);
    delay(2000);

    if (!dfplayer.begin(dfSerial, false)) {
        Serial.println("ERROR: No se detecta DFPlayer");
        audioDisponible = false;
        return;
    }

    audioDisponible = true;

    Serial.println("DFPlayer OK");

    delay(1500);
    dfplayer.volume(30);
    delay(500);
}

void reproducirAudio(int track) {
    if (!audioDisponible) {
        Serial.println("Audio no disponible");
        return;
    }

    Serial.print("Play MP3 folder track: ");
    Serial.println(track);

    dfplayer.playMp3Folder(track);
}