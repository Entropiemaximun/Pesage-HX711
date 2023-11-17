//  programme  afficheur parola et HX711  avec  arduino Nano
// Bricbric & Piem 2023
 
 #include <stdint.h> 

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

const uint16_t WAIT_TIME = 100;


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
                                
#define MAX_DEVICES 4

#define CLK_PIN  11  // pinoche MOSI
#define DATA_PIN 13  // pinoche SCK
#define CS_PIN   10  // pinnoche SS

#define Tare 7
int Tare0=0;

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Calibrating the load cell
#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN  4   //   pinoche hx711
#define LOADCELL_SCK_PIN  5  //   pinoche hx711

HX711 scale;
  long reading =0;
//float poids =0;
int poids =0;


int facteur  =14698;    // a modifier
int vide=24000;    // a modifier

  //reading= valeur affichée par le peson sans rien
 // vide= valeur pour annuler la valeur lue (reading)
 // facteur= (reading + reading du poids étalonage)/par poids d'étalonage
 // facteur= (24000 + 168 000) / 80
 // facteur= (24000 - 1100) / 1.5
 // facteur= 2 275
 //poids= (reading-vide)/facteur; voici le calcul effectué par arduino plus bas

  



  //loop time variables in microseconds  
  const uint16_t LOOP_TIME = 400;  //2Hz    
  uint32_t lastTime = LOOP_TIME;
  uint32_t currentTime = LOOP_TIME;

void setup(void)
{

  pinMode(Tare, INPUT_PULLUP);

  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  P.begin();
  P.print("PESE");
  delay(1000);
  P.print("VEAU");
  delay(1000);
 
  Serial.begin(115200);
  
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);



 reading = scale.get_units(4);
  poids = (reading - vide) / facteur;
  Tare0 = poids;
  P.print("Tare");
  Serial.print("Tare");



}

void loop(void)
{



    currentTime = millis();
   
    if (currentTime - lastTime >= LOOP_TIME)
    {
      lastTime = currentTime;




  if (scale.is_ready()) {

    
    reading = scale.get_units(4);
   

      //poids= reading/facteur-vide;
     poids= (reading-vide)/facteur;
    //P.print(reading);
    if (!digitalRead(Tare)){
      Tare0=poids;
       P.print("Tare");
       Serial.print("Tare");
    }
    else{
      
    
    poids=poids-Tare0;
      
     Serial.print("Result: ");
    Serial.print(reading);
    Serial.print(" ");
       Serial.println(poids);
     P.print(poids);
    }
  } 
  else {
    Serial.println("HX711 not found.");
      P.print("no pesée");
  }



//calibration factor will be the (reading)/(known weight)
 
 
    }
 yield;
 
}
