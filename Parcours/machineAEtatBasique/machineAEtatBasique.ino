#include <Servo.h>
#include <SharpIR.h>
#define ir A0
#define model 1080
Servo myServoD, myServoG;


/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/*CONSTANTE ULTRASON ROUGE*/
/**************************/
/* Constantes pour les broches */
const byte TRIGGER_PIN_ULTRASON_ROUGE = 10; // Broche TRIGGER
const byte ECHO_PIN_ULTRASON_ROUGE = 3;    // Broche ECHO
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT_ROUGE = 25000UL; // 25ms = ~8m à 340m/s
/**************************/

/*FONCTION ULTRASON ROUGE*/
/**************************/
void initUltrasonRouge() {
   /* Initialise les broches */
  pinMode(TRIGGER_PIN_ULTRASON_ROUGE, OUTPUT);
  digitalWrite(TRIGGER_PIN_ULTRASON_ROUGE, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN_ULTRASON_ROUGE, INPUT);
}

float getDistanceUltrasonRouge() {
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN_ULTRASON_ROUGE, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_ULTRASON_ROUGE, LOW);
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN_ULTRASON_ROUGE, HIGH, MEASURE_TIMEOUT_ROUGE);
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm;
}
/**************************/


/*FONCTION INFRA ROUGE*/
/**************************/
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)
SharpIR SharpIR(ir, model);
/*FONCTION IR*/
long getDistanceInfrarouge(){ 
  long dis = SharpIR.distance();  // this returns the distance to the object you're measuring
  return dis;
}
/**************************/

/*FONCTION MOTEUR*/
/**************************/
void initMoteur() {
  myServoD.attach(5);
  myServoG.attach(6);
}

void rotationGauche(){
  myServoG.write(120);
  myServoD.write(60);
}

void rotationDroite(){
  myServoG.write(70);
  myServoD.write(130);
}

void trajectoireGauche(){
  myServoG.write(125);
  myServoD.write(115);
}

void trajectoireDroite(){
  myServoG.write(115);
  myServoD.write(125);
}

void avancer(){
  myServoG.write(115);
  myServoD.write(115);
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
int DISTANCE_MM_ROUGE = 0;
int DISTANCE_MM_INFRAROUGE=0;
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
  initUltrasonRouge();
  Serial.begin(9600);
}


void loop() {
  DISTANCE_MM_INFRAROUGE = getDistanceInfrarouge()*10;
  DISTANCE_MM_ROUGE = getDistanceUltrasonRouge();
  if(DISTANCE_MM_ROUGE < 1) {
    DISTANCE_MM_ROUGE = 8000;
  }
  Serial.println(DISTANCE_MM_INFRAROUGE);

  switch(ETAT_PRESENT) {
    case 0 : 
       arreter();
       if(DISTANCE_MM_ROUGE > 500 && DISTANCE_MM_INFRAROUGE > 300) {
        ETAT_SUIVANT = 3;
       } else if(DISTANCE_MM_ROUGE > 500 && DISTANCE_MM_INFRAROUGE < 200){
        ETAT_SUIVANT = 5;
       } else if(DISTANCE_MM_ROUGE > 500 ) {
        ETAT_SUIVANT = 1;
       } else {
        ETAT_SUIVANT = 0;
       }
       Serial.println(" Etat arreter");
       break;
    case 1 : 
      avancer();
      if(DISTANCE_MM_ROUGE < 500){
        ETAT_SUIVANT = 4;
      } else if (DISTANCE_MM_INFRAROUGE > 300) {
        ETAT_SUIVANT = 3; 
      } else if (DISTANCE_MM_INFRAROUGE < 200) {
        ETAT_SUIVANT = 5; 
      } else {
        ETAT_SUIVANT = 1;
      }
      Serial.println(" Etat avancer");
      break;
    case 2 : 
      reculer();
      Serial.println(" Etat reculer");
      break;
    case 3 : 
      trajectoireDroite();
      if(DISTANCE_MM_ROUGE < 500 ) {
        ETAT_SUIVANT = 4;
      } else if(DISTANCE_MM_INFRAROUGE < 300){
        ETAT_SUIVANT = 5;
      } else {
        ETAT_SUIVANT = 3;
      }
      Serial.println(" Etat trajectoireDroite");
      break;
    case 4 : 
       rotationGauche();
       if(DISTANCE_MM_ROUGE > 500){
        ETAT_SUIVANT = 1; 
       } else {
        ETAT_SUIVANT = 4;
       }
       Serial.println(" Etat rotationGauche");
       break;
    case 5 : 
       trajectoireGauche();
       if(DISTANCE_MM_ROUGE < 500){
        ETAT_SUIVANT = 4; 
       } else if(DISTANCE_MM_INFRAROUGE > 200){
        ETAT_SUIVANT = 1;
       } else {
        ETAT_SUIVANT = 5;
       }
       Serial.println(" Etat trajectoireGauche");
       break;
  }
  
  ETAT_PRESENT = ETAT_SUIVANT;
  delay(200);
}
