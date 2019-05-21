/* 
______________________________________________________

 Lire les commentaire de visualisation en premier
 Je ne redis pas tout dans ce code
______________________________________________________

*/ 

import processing.serial.*; // pour le port serie

// Serial
Serial myPort;  // créer l'objet sérial 
String inString =""; 
int lf = 36;      // ASCII dollar 

String[] items = new String[2];
float[] valeur_recu_temporaire = {0,0,0};

float save_x = 123456;
float save_y = 123456;

// liste de x et liste de y pour l'initialisation de l'accelerometre
float[] liste_init_x = new float[0];
int compte_init_x = 0;
float[] liste_init_y = new float[0];
int compte_init_y = 0;
int bo = 2; //booléen 

//valeur de moyenne x et y. 
float moyenne_x = 0; 
float moyenne_y = 0;
int entree_permise;

float position_robot_x = 400;
float position_robot_y = 400;


void setup() {
  size(800,800);
  background(0);
  //1 cm = 1 pixel

  String portName = "COM6";  //port
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil(lf);
  
}


void draw() {
  background(255);
  //line(x1_ligne,x2_ligne,y1_ligne,y2_ligne);
  conversion_serial_en_float();
  //regression_lineaire();
  affiche_robot();
  //ajoute_nouveau_obstacles();
  //affiche_obstacles();
}

void serialEvent(Serial p){
  try {
    inString = p.readString(); 
  }
  catch (Exception e) {}
}


String[] parseString(String serialString){
  if (serialString != null)
  {
    serialString = serialString.replaceAll("(\\r|\\n)", "");
    String items[] = splitTokens(serialString, " =");
    return(items);
  }
  else return(null);  
}

void conversion_serial_en_float(){
  // version modifie de la fonction conversion_serial_en_float pour prendre en compte la position de l'axe x et y (et z)
  String[] tmp_tableau = split(inString,',');
  try{
    valeur_recu_temporaire[0] = float(tmp_tableau[0]); //x
    valeur_recu_temporaire[1] = float(tmp_tableau[1]); //y
    valeur_recu_temporaire[2] = float(tmp_tableau[2]); //z
  }catch (Exception e) {}
  //print("\nx = "+valeur_recu_temporaire[0]+"  y = "+valeur_recu_temporaire[1]+"  z = "+valeur_recu_temporaire[2]); //decommenter pour tester
  
  //print("\n AVANT :"+valeur_recu_temporaire[0]+" et "+save_x);
  entree_permise=0;
  // entree_permise = 1 si la valeur est conforme aux exigences et aux attentes. Sinon elle vaut 0. 
  if(valeur_recu_temporaire[0] != save_x && valeur_recu_temporaire[0]>0){ //valeur_recu_temporaire[0]>0 permet de filtrer les première valeurs d'initialisation qui sont fausses
    if(save_x != 123456.0 &&(valeur_recu_temporaire[0]-100 > save_x || valeur_recu_temporaire[0]+100 < save_x)){ //eviter les valeurs trop loin
      ; //mauvaise valeur à éliminer.
    }
    else{ //bonne valeur
      save_x = valeur_recu_temporaire[0];  //on va sauvegarder la valeur de x. 
      if(compte_init_x<20){ //pour l'initialisation de l'accelerometre on va creer un tableau. 
        liste_init_x = splice(liste_init_x,valeur_recu_temporaire[0],compte_init_x); 
        compte_init_x++;
      }  
      entree_permise=1; // la valeur est bonne donc l'entree vaut 1.
      //print("\n x"+compte_init_x+" "+valeur_recu_temporaire[0]);
    }
  }
  if(valeur_recu_temporaire[1] != save_y && valeur_recu_temporaire[1]>0){ //meme condition que pour x. 
    if(save_y != 123456.0 &&(valeur_recu_temporaire[1]-100 > save_y || valeur_recu_temporaire[0]+100 < save_y)){ //eviter les valeurs trop loin
      ; //mauvaise valeur à éliminer.
    }
    else{ //bonne valeur
      save_y = valeur_recu_temporaire[1];
      if(compte_init_y<20){
        liste_init_y = splice(liste_init_y,valeur_recu_temporaire[1],compte_init_y); 
        compte_init_y++;
      }
      entree_permise=1;
      //print("\n y"+compte_init_y);
    }
  }
  if(bo==1){ //pour remettre les valeurs à zéro
    compte_init_y=0;
    compte_init_x=0;
  }
  //print("\n",compte_init_x +" "+ compte_init_y+" "+bo); //pour les tests

  //calcul des moyennes pour initialiser les positions et donc avoir la condition initiale 1 (voir rapport). 
  if(compte_init_x==20 && compte_init_y==20 && bo==2){
    moyenne_x = 0;
    moyenne_y = 0;
    for(int i=0;i<20;i++){
      moyenne_x += liste_init_x[i];
      moyenne_y += liste_init_y[i];
    }
    moyenne_x = moyenne_x / 20.0;
    moyenne_y = moyenne_y / 20.0;
    //affiche les moyennes.
    print(" \n Moyenne x = "+moyenne_x+"    Moyenne y = "+moyenne_y);
    bo=1; // cette variable passe à 1 quand on a fini de calculer les moyennes pour l'initialisation. 
  }
  //si robot bouge on met robot_immobile à 0. On remet compte teta à 0. 
}

void affiche_robot(){
  fill(100,0,0); 
  //entree permise ==1  donc valeur conforme et bo ==1 donc initialisation termine
  if(entree_permise==1 && bo==1){
    if(abs(valeur_recu_temporaire[0]-moyenne_x) > 5 && abs(valeur_recu_temporaire[1]-moyenne_y) > 5 ){ //si il y a un leger changement de la position de x et de y alors on bouge
      //print("\nle robot bouge. x:" +(valeur_recu_temporaire[0]-moyenne_x)+" y="+(valeur_recu_temporaire[1]-moyenne_y)); 
      // si on integre 2 fois l'acceleration on a la position en fonction du temps. 
      position_robot_x = position_robot_x+(((valeur_recu_temporaire[0]-moyenne_x)/2)*0.4);
      position_robot_y = position_robot_y+(((valeur_recu_temporaire[1]-moyenne_y)/2)*0.2);
    }
  }
  //robot_immobile++;
  circle(position_robot_x,position_robot_y,40);
}
