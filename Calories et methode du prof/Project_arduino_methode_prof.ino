#include <SoftwareSerial.h>  //inclue le module bluetooth
#include <Wire.h>            //inclue le bus i2c
SoftwareSerial btSerial(6, 7);  //port du bluetooth

const byte capteur=3;
volatile long temps=0;
volatile long temps_prece=0;
long calories;
float vitesse;
float diametre=0.62; // 0.62m = 62,2 cm
int nb_aimants=2;
float distance= ((2*3.14)/nb_aimants)*(diametre/2) ;// distance entre deux aimant= rayon * (angle=2pi/nombre d'aimants)
float vitesse_km;
float masse= 70;
void gere_int1() // Gestion de l'interruption 
{ 
    temps_prece= temps;
    temps= millis();
}

void setup() {
  // put your setup code here, to run once:
 calories=0;
 pinMode(capteur,INPUT); 

 attachInterrupt(1,gere_int1,RISING); // Appelle la fontion gere_int1 à chaque fois que la broche 3 passe de l'etat BAS vers HAUT
                                      // appelle la foction gere_int si l'interruption(ou changement) 1 se produit c'est à dire sur la broche 3 se le mode RISING
                                      // (quand passe de l'etat BAS vers HAUT) selon la sytaxe attachInterrupt(Interruption,fonction,mode);

setupBlueToothConnection();  //fonction initialisation bluetooth

}

void loop() {
  // put your main code here, to run repeatedly:
vitesse = distance/temps_prece;
vitesse_km=vitesse*3.6;
btSerial.print(vitesse_km); 

float depense;
depense=(0.86*(0.26*vitesse*vitesse*vitesse+ 0.1*vitesse*masse))*(temps_prece/3600); 
calories= calories + depense; 
btSerial.print(calories); 

}
void setupBlueToothConnection()    //initialisation bluetooth
  {
  btSerial.begin(38400);
  btSerial.print("\r\n+STWMOD=0\r\n"); 
  btSerial.print("\r\n+STNA=WOODBIKE\r\n"); 
  btSerial.print("\r\n+STPIN=1234\r\n");
  btSerial.print("\r\n+STOAUT=1\r\n"); 
  btSerial.print("\r\n+STAUTO=0\r\n");
  delay(2000); 
  btSerial.print("\r\n+INQ=1\r\n"); 
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000);
 btSerial.flush();
  }
