#include <Servo.h>
#include "esc.h"
#include "Arduino.h"

Servo ESC; //Crear un objeto de clase servo
 
int vel = initpulse;

void init_ESC(){
    ESC.attach(ESC_pin);
    delay(10);
    //ESC.writeMicroseconds(initpulse);
    //delay(5000); //Esperar 5 segundos para hacer la activacion
}

void vel_ESC(char percent){
    int ms = map(percent,0,100,1000,2000);
    ESC.writeMicroseconds(ms);
}
