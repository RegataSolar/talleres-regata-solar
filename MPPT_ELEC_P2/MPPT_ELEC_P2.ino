#include "Ticker.h"
#include "current.h"
#include "Config.h"

//---------------DEBUG---------------
bool DEBUG = true;

//-------------------Ticker Variables--------------
Ticker cicle_timer;
float v = 0;
float i = 0;
float p = 0;
void PO_timer_callback();


//-----------------INA219---------------
#define Ki 270    // coeficiente de axuste de corrente, pola modificación da resistencia Shunt

//###################################################################
// SETUP
//###################################################################

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  
  delay(1000);

  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  
  init_INA219(0);

  cicle_timer.setCallback(PO_timer_callback);
  cicle_timer.setInterval(CICLE_TIMER);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
  cicle_timer.start();

  delay(1000);

}

//###################################################################
// LOOP
//###################################################################

void loop() {
  
  cicle_timer.update();
}

//###################################################################
// TICKER
//###################################################################

void PO_timer_callback(){ 

         v = get_LoadVolts();
         i = get_LoadCurrent()/Ki;
         p = v*i;

         if(DEBUG){
            print_telemetry();
         }
} //end callback


//###################################################################
// FUNCIONS VARIAS
//###################################################################

void print_telemetry(){
    Serial.print(v);
    Serial.print("V,");
    Serial.print(i);
    Serial.print("A,");
    Serial.print(p);
    Serial.println("W");
}
