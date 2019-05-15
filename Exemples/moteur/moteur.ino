#include <Servo.h>

Servo myServoD, myServoG;
int pos;

void setup() {
  // put your setup code here, to run once:
myServoD.attach(5);
myServoG.attach(3);
}

void rotationDroite(){
 /* myServoD.write(50);
myServoG.write(50);
delay(1500);*/
myServoG.write(90);
myServoD.write(25);
delay(1000);
/*myServoD.write(50);
myServoG.write(50);*/
}

void rotationGauche(){
 /* myServoD.write(50);
myServoG.write(50);
delay(1500);*/
myServoG.write(30);
myServoD.write(90);
delay(1000);
/*myServoD.write(50);
myServoG.write(50);*/
}

void arriere()

void loop() {


  myServoD.write(50);
myServoG.write(50);
delay(1500);
rotationDroite();
myServoD.write(50);
myServoG.write(50);
rotationGauche();
myServoD.write(50);
myServoG.write(50);





/* CODE JUSTE


for(pos=90;pos>=24;pos-=1){
myServoD.write(pos);
myServoG.write(pos);
delay(500);
}

for(pos=24;pos<=90;pos+=1){
myServoD.write(pos);
myServoG.write(pos);
delay(500);
}

for(pos=90;pos<=161;pos+=1){
myServoD.write(pos);
myServoG.write(pos);
delay(500);
}

for(pos=161;pos>=90;pos-=1){
myServoD.write(pos);
myServoG.write(pos);
delay(500);
}
*/
}
