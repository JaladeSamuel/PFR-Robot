/* PARTIE SERVO 
#include <Servo.h>
Servo servo1;
int teta=0;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(teta=0;teta<=180;teta+=1){
    servo1.write(teta); //angle final visé
    delay(45);
  }  
  for(teta=180;teta>=0;teta-=1){
    servo1.write(teta);
    delay(45);
  }
}

///////////////////////////////////
*/

#include <Servo.h>
Servo servo1;
int teta=0;
/* Constantes pour les broches */
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO
 
/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/** Fonction setup() */
void setup() {
  
  servo1.attach(9);
   
  /* Initialise le port série */
  Serial.begin(9600);
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
}
 
/** Fonction loop() */
void loop() {
  for(teta=40;teta<=120;teta+=5){
    servo1.write(teta); //angle final visé
    delay(50);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    delay(50);
    digitalWrite(TRIGGER_PIN, LOW);
    
    /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
    long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
     
    /* 3. Calcul la distance à partir du temps mesuré */
    float distance_mm = measure / 2.0 * SOUND_SPEED;
     
    /* Affiche les résultats en mm, cm et m */
    /*
    Serial.print(F("Distance: "));
    Serial.print(distance_mm);
    Serial.print(F("mm ("));
    Serial.print(distance_mm / 10.0, 2);
    Serial.print(F("cm, "));
    Serial.print(distance_mm / 1000.0, 2);
    Serial.println(F("m)"));
    */
    Serial.print(distance_mm / 10.0, 2);
    Serial.print(F(","));
    Serial.println(teta);
    Serial.print(F(","));
    Serial.println("$"); 
    /* Délai d'attente pour éviter d'afficher trop de résultats à la seconde */
    delay(100);
  }
  servo1.write(40);
  delay(45);
}

/*
  Read Value 
*/
/*
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input from analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.print("Value=");
  Serial.println(sensorValue);
  delay(10);        // delay in between reads for stability
}
*/
