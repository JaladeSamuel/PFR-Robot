#include <SoftwareSerial.h>
#include <Servo.h>
#define ir A0
#define model 1080
Servo myServoD, myServoG;

SoftwareSerial mySerial(12, 11); // RX, TX  
// Connect HM10      Arduino Uno
//     TXD          Pin 7
//     RXD          Pin 8

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/*CONSTANTE ULTRASON AVANT*/
/**************************/
/* Constantes pour les broches */
const byte TRIGGER_PIN_ULTRASON_AVANT = 2; // Broche TRIGGER
const byte ECHO_PIN_ULTRASON_AVANT = 3;    // Broche ECHO
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT_AVANT = 25000UL; // 25ms = ~8m à 340m/s
/**************************/

/*FONCTION ULTRASON AVANT*/
/**************************/
void initUltrasonAvant() {
   /* Initialise les broches */
  pinMode(TRIGGER_PIN_ULTRASON_AVANT, OUTPUT);
  digitalWrite(TRIGGER_PIN_ULTRASON_AVANT, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN_ULTRASON_AVANT, INPUT);
}

float getDistanceUltrasonAvant() {
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN_ULTRASON_AVANT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_ULTRASON_AVANT, LOW);
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN_ULTRASON_AVANT, HIGH, MEASURE_TIMEOUT_AVANT);
  /* 3. Calcul la distance à partir du temps mesuré */
  int distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm;
}
/**************************/

/*CONSTANTE ULTRASON ROUGE*/
/**************************/
const int numOfReadings = 10;
int readings[numOfReadings];
int arrayIndex = 0;
int total = 0;

int echoPin = 7;
int initPin = 4;
int vitesse=0;

unsigned long pulseTime = 0; //stores the pulse in MicroSeconds
unsigned long distance = 0; //variable for storing the distance(cm)
/**************************/

/*FONCTION ULTRASON DROIT*/
/**************************/
void initUltrasonDroit() {
  pinMode(initPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int thisReading = 0; thisReading < numOfReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }
}

int getDistanceUltrasonDroit() {
  digitalWrite(initPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(initPin, LOW);
  
  pulseTime = pulseIn(echoPin, HIGH);
  distance = pulseTime / 58;
  /*total = total - readings[arrayIndex];
  readings[arrayIndex] = distance;
  total = total + readings[arrayIndex];
  arrayIndex = arrayIndex + 1;

  if (arrayIndex >= numOfReadings)
  {
    arrayIndex = 0;
  }*/

  return distance * 10;
}
/**************************/

/*FONCTION MOTEUR*/
/**************************/
void initMoteur() {
  myServoD.attach(5);
  myServoG.attach(6);
}

void rotationGauche(){
  myServoG.write(133);
  myServoD.write(50);
}

void rotationDroite(){
  myServoG.write(70);
  myServoD.write(130);
}

void trajectoireGauche(int vitesse){
  myServoG.write(vitesse);
  myServoD.write(vitesse-10);
}

void trajectoireDroite(int vitesse){
  myServoG.write(vitesse-10);
  myServoD.write(vitesse);
}

void avancer(int vitesse){
  myServoG.write(vitesse);
  myServoD.write(vitesse);
}

void reculer(){
  myServoG.write(70);
  myServoD.write(70);
}

void arreter(){
  myServoG.write(90);
  myServoD.write(90);
}
/**************************/


/********MAIN*********/
/*********************/
unsigned int ETAT_PRESENT = 1;
unsigned int ETAT_SUIVANT = 0;
int DISTANCE_MM_AVANT = 0;
int DISTANCE_MM_DROIT = 0;

/*
 * 0 = arreter()
 * 1 = avancer()
 * 2 = reculer()
 * 3 = rotationDroite()
 * 4 = rotationGauche()
 * 
 */ 
void setup() {
  initMoteur();
  initUltrasonAvant();
  initUltrasonDroit();
  Serial.begin(9600);
  mySerial.begin(9600);
}


void loop() {
  DISTANCE_MM_DROIT = getDistanceUltrasonDroit();
  DISTANCE_MM_AVANT = getDistanceUltrasonAvant();

  Serial.print("Distance Droite: ");
  Serial.print(DISTANCE_MM_DROIT);
  Serial.print(" | Distance Avant: ");
  Serial.print(DISTANCE_MM_AVANT);
  
  if (DISTANCE_MM_AVANT < 1) {
    DISTANCE_MM_AVANT = 8000;
  }
  if (DISTANCE_MM_DROIT < 1) {
    DISTANCE_MM_DROIT = 8000;
  }

  if (DISTANCE_MM_AVANT > 1500) {
    vitesse = 125;
  } else if (DISTANCE_MM_AVANT > 600 && DISTANCE_MM_AVANT < 1500) {
    vitesse = 120;
  }

  switch(ETAT_PRESENT) {
  int c;
  if (Serial.available()) {
    c = Serial.read();
    mySerial.print(c);
  }
  if (mySerial.available()) {
    c = mySerial.read();
    Serial.println(c);    
  }
  

    
    case 0: 
      arreter();
      
      if (DISTANCE_MM_AVANT > 600 && DISTANCE_MM_DROIT > 400 && c==1) {
        ETAT_SUIVANT = 3;
      } else if (DISTANCE_MM_AVANT > 600 && DISTANCE_MM_DROIT < 300){
        ETAT_SUIVANT = 5;
      } else if (DISTANCE_MM_AVANT > 1500 ) {
        ETAT_SUIVANT = 1;
      } else {
        ETAT_SUIVANT = 0;
      }
      
      Serial.println(" | Etat arreter");
      break;
      
    case 1: 
      avancer(vitesse);
      
      if(DISTANCE_MM_AVANT < 600){
        ETAT_SUIVANT = 4;
      } else if (DISTANCE_MM_DROIT > 400) {
        ETAT_SUIVANT = 3; 
      } else if (DISTANCE_MM_DROIT < 300) {
        ETAT_SUIVANT = 5;
      } else {
        ETAT_SUIVANT = 1;
      }
      
      Serial.println(" | Etat avancer");
      break;
      
    case 2 : 
      reculer();
      Serial.println(" | Etat reculer");
      break;
      
    case 3: 
      trajectoireDroite(vitesse);
      
      if(DISTANCE_MM_AVANT < 600 ) {
        ETAT_SUIVANT = 4;
      } else if(DISTANCE_MM_DROIT < 400){
        ETAT_SUIVANT = 5;
      } else {
        ETAT_SUIVANT = 3;
      }
      Serial.println(" | Etat trajectoireDroite");
      break;
      
    case 4: 
      rotationGauche();
      
      if (DISTANCE_MM_AVANT > 600) {
        ETAT_SUIVANT = 1;
      } else {
        ETAT_SUIVANT = 4;
      }
      
      Serial.println(" | Etat rotationGauche");
      break;
      
    case 5:
      trajectoireGauche(vitesse);

      if (DISTANCE_MM_AVANT < 600){
        ETAT_SUIVANT = 4; 
      } else if (DISTANCE_MM_DROIT > 300){
        ETAT_SUIVANT = 1;
      } else {
        ETAT_SUIVANT = 5;
      }
      
      Serial.println(" | Etat trajectoireGauche");
      break;

    case 6:

      break;
  }
  
  ETAT_PRESENT = ETAT_SUIVANT;
  delay(200);
}
