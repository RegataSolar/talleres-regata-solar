#include "esc.h"
#include "Ticker.h"
#include "Config.h"

//---------------DEBUG---------------
bool DEBUG = true;

//-------------------Ticker--------------
Ticker cicle_timer;
void PO_timer_callback();

//-------------------RC SIGNAL-----------------
static byte rcOld;
volatile unsigned long rcRises[4];
volatile unsigned long rcTimes[4];
volatile unsigned int rcChange =0;
void init_RC();

//-----------------ESC----------------------
int PPM1 = 0;    
int speed = 0;   // 50 para configuración bidireccional, 0 para unidireccional

//------------------------------------------



//###################################################################
// SETUP
//###################################################################
void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  
  init_ESC();
  delay(1000);
  init_RC();

  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  
  cicle_timer.setCallback(PO_timer_callback);
  cicle_timer.setInterval(CICLE_TIMER);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
  cicle_timer.start();

  delay(1000);

}

//###################################################################
// LOOP
//###################################################################
void loop() {
  unsigned long rcT[4];
  unsigned int rcN;

  if(rcChange){
    cli();
    rcN=rcChange;
    rcChange =0; //borramos la flag de nuevos cambios
    //leemos los registros de tiempos d cada puerto
    rcT[0] = rcTimes[0];
    rcT[1] = rcTimes[1];
    rcT[2] = rcTimes[2];
    rcT[3] = rcTimes[3];
    sei();

    PPM1 = map(rcT[0],1000,2000,0,100);

    if(PPM1 >100) PPM1=100;
    if(PPM1 <0) PPM1 = 0;
  }
  sei();
  cicle_timer.update();
}

//###################################################################
// TICKER
//###################################################################
void PO_timer_callback(){ 

         vel_ESC(PPM1); 

         if(DEBUG){
            print_telemetry();
         }
} //end callback



//###################################################################
// FUNCIÓNS VARIAS
//###################################################################

void print_telemetry(){
    Serial.println(PPM1);   

}


void init_RC(){
  rcOld =0;
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  PCMSK1 |= 0x0F; //pin-change interrupt en los 4 primeros pines del PORTC (A0-A3)
  PCIFR |= 0x02; //clear pin-change interrupt
  PCICR |= 0x02; //pin-change interrupt enable
}


ISR (PCINT1_vect){
  byte rcNew = PINC & 15; //PINC & 1111 pilla el estado de los pines A0-A3
  byte changes = rcNew^rcOld; //saca el pin que ha cambiado comparando el estado de los 4 anteriores cn s 4 nuevos
  byte channel =0;
  unsigned long now=micros();
  while(changes){ // de 0000 a 1111
    if((changes &1)){ //Ha cambiado este canal ?
      if((rcNew & (1<<channel))){ //si es flanco subida
        rcRises[channel] =  now;  //t0=ini
      }else{                      //si es flanco bajada
        rcTimes[channel] = now -rcRises[channel];//t1=nw-t0;
      }
    }
    changes >>=1;
    ++channel;
    ++rcChange; //activamos la flag de nuevos cambios para el loop
  }
  rcOld = rcNew;
}
