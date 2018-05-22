#include <SoftwareSerial.h> //inclue le module bluetooth
#include <Wire.h> //inclue le bus i2c

const byte PIN_LED = 3; 
SoftwareSerial btSerial(6, 7); //port du bluetooth
int tour=0; //variable en nb entier
int tour_precedent=0;

long duree_test = 3000; // test sur 3 secondes
long chrono_depart = 0; // valeur de départ du chrono
long chrono = 0; // valeur courante du chrono
long chrono_precedent=0;

float vitesse = 0; //variable decimale
float vitesse_moyenne;
float distance=0;
float perimetre;

volatile long nb_chgt = 0; // nb de changement etat Pin


void gere_int0() // Gestion de l'interruption 
{ 
nb_chgt = nb_chgt + 1 ; 
}

void setup ()
{
pinMode(PIN_LED, OUTPUT); 
setupBlueToothConnection(); //fonction initialisation bluetooth
perimetre=29*3.14; //pérmimètre de la roue en cmm 
chrono_depart = millis(); //chrono de départ
attachInterrupt(0,gere_int0,CHANGE); 
}

void loop() {

int nb_tour;
tour=nb_chgt/2; //initialisation du nombre de chgt capté (1 a chaque passage de l'aimant)
chrono = millis();

if (tour_precedent!=tour) 
{ 
nb_tour=tour-tour_precedent;
tour_precedent=tour;


distance=(distance+nb_tour*perimetre/100); // en m
vitesse=0.01*3600*(nb_tour*perimetre)/(chrono-chrono_precedent); //km par h
chrono_precedent=chrono;
vitesse_moyenne=0.01*3600*distance/(chrono-chrono_depart);
btSerial.print(vitesse); 
btSerial.print("&");
btSerial.print(distance); 
btSerial.print("&");
btSerial.print((chrono-chrono_depart)/1000);
btSerial.print("&");
btSerial.print(vitesse_moyenne);
btSerial.print("&");




} 
else if ((chrono-chrono_precedent>4000) and (vitesse!=0))
{

vitesse=0;
chrono_precedent=chrono; 
vitesse=0;
btSerial.print(vitesse);
btSerial.print("&");
btSerial.print(distance); 
btSerial.print("&");
btSerial.print((chrono-chrono_depart)/1000);
btSerial.print("&");
vitesse_moyenne=3600*distance/(chrono-chrono_depart);
btSerial.print(vitesse_moyenne);
btSerial.print("&");
} 
delay(1000);

}

void setupBlueToothConnection() //initialisation bluetooth
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
