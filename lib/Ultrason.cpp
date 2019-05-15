#include "../../lib/Ultrason.h"

/* Constantes pour le timeout */
unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s

/* Vitesse du son dans l'air en mm/us */
const float SOUND_SPEED = 340.0 / 1000;

/** Fonction setup() */
void initSerialSon() {
  /* Initialise le port série */
  Serial.begin(9600);
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN_ULTRASON, OUTPUT);
  digitalWrite(TRIGGER_PIN_ULTRASON, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN_ULTRASON, INPUT);
}
 
/** Fonction distance mm */
float distance_mm_ultrason() {
  
  /* 1. Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER */
  digitalWrite(TRIGGER_PIN_ULTRASON, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN_ULTRASON, LOW);
  
  /* 2. Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe) */
  long measure = pulseIn(ECHO_PIN_ULTRASON, HIGH, MEASURE_TIMEOUT);
   
  /* 3. Calcul la distance à partir du temps mesuré */
  float distance_mm = measure / 2.0 * SOUND_SPEED;
  return distance_mm;
}
