/*
 * DEBER 5.2: MANEJO DE SLEEP
 * OBJETIVO: REALIZAR MODIFICACION Y CORRECCION DEL CODIGO 23
 * NOMBRE: RAFAEL FARINANGO
 */

#include<avr/power.h>
#include<avr/sleep.h>
#include<avr/wdt.h>
#include <MsTimer2.h>
#include<EEPROM.h>

int dato;
int on=0;
void(* resetFunc) (void) = 0; 


void setup() {
  wdt_disable();        //deshabilita el perro para empezar desde cero
  dato=EEPROM.read(0);  //lee la posicion 0 de la eeprom
  Serial.begin(9600);   //velocidad de la comunicacion serial
  Serial.println("COMENZAMOS"); //mensaje de aviso que comienza el programa
  //Serial.println(dato);       //algunos mensajes estan comentados porque me ayudaban a guiarme pero ya no los necesito
  if(dato==0){                  //si dato es igual a 0 
    set_sleep_mode(SLEEP_MODE_STANDBY);   //establece el modo sueño con modo standby
    sleep_enable();                       //habilita modo sueño
    EEPROM.write(0,1);                    //escribe en la posicion 0 de la eeprom el 1
  }
  else{                                   // caso contrario
    sleep_disable();                      //deshabilita el modo sueño
    EEPROM.write(0,0);                    //escribe 0 en la misma posicion
    MsTimer2::set(1000,tiempo);           //establece timer2 a razon de 1 segundo
    MsTimer2::start();                    //comienza
  }
  attachInterrupt(0,encender,LOW);        //establece una interrupcion con los parametros que se pueden ver
  wdt_enable(WDTO_8S);                    //habilita el perro guardian a razon de 8 segundos maximos de espera
}


void loop() {
  //Serial.println(EEPROM.read(0));
   
  if(dato == 0){                  //si dato es cero
    Serial.println("Sleep");      //imprime sleep
    delay(100);             
    sleep_mode();                 //activa modo sueño
    wdt_reset();                  //verifica con el perro guardian para que el modo sueo dure solo 8 seg
  }
  else{
    Serial.println("Despierto");  //mensaje
    delay(1000);
    sleep_disable();              //deshabilita modo sueño
  }

}


void tiempo(){
  int v = EEPROM.read(0);     //toma el valor de la posicion cero de la eeprom
  if(v == 0){                 //si es igual a cero 
    EEPROM.update(0,1);       //cambia su valor a uno
  }
  else{                       //si no
    EEPROM.update(0,0);       //cambia su valor a cero
  }
//  Serial.println(EEPROM.read(0));     comentarios porque estaba haiendo pruebas 
//  unsigned t = millis();
//  Serial.println(t);
//  if(dato==0){
//    EEPROM.update(0,1);  
//  }
}


void encender(){    //si aplastamos el boton se activa esta interrupcion
  resetFunc();      // y resetea el programa en cuanquier momento
}
