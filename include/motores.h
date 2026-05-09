#ifndef MOTORES_H
#define MOTORES_H

void iniciarMotores();
void apagarMotores();

void motorIzquierdo(int potencia);
void motorDerecho(int potencia);
void ambosMotores(int potencia);

#endif