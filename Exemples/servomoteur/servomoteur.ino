#include <Servo.h>
Servo servo1;
int teta=0; 
void setup() {
  // put your setup code here, to run once:
servo1.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(teta=0;teta<=45;teta+=1){
   servo1.write(45);
   delay(15);
   }
    for(teta=45;teta<=90;teta+=1){
   servo1.write(90);
   delay(15);}

   for(teta=90;teta<=135;teta+=1){
   servo1.write(135);
   delay(15);}
   
   for(teta=135;teta<=180;teta+=1){
   servo1.write(180);
   delay(15);}

// le retour

    for(teta=180;teta>=135;teta-=1){
   servo1.write(135);
   delay(15);
   }
    for(teta=135;teta>=90;teta-=1){
   servo1.write(90);
   delay(15);}

   for(teta=90;teta>=45;teta-=1){
   servo1.write(45);
   delay(15);}
   
   for(teta=45;teta>=0;teta-=1){
   servo1.write(0);
   delay(15);}

}
