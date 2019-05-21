import processing.serial.*;

// pour le port serial
Serial myPort;  // créer l'objet sérial 
String inString ="";  // Input string from serial port:
int lf = 36;      // ASCII de dollar 

String[] items = new String[2];  // liste des items utile pour la capture des informations sur le port serie.
float[] valeur_recu_temporaire = {0,0};  //liste des valeurs recu temporaires

float[] liste_position_obstacle = new float[0];  //cette liste contient l'ensemble des positions (x,y) des obstacles dectecte
int nb_elem_liste_position_obstacle = 0;  //le nombre d'element de la liste


float position_robot_x = 400;  // position inialise au centre de la salle (on suppose que la salle fait 800 cm x 800 cm. 
float position_robot_y = 400;

//position x et y d'obstacles contenues dans la liste_position_obstacle  
float position_obstacle_x;  
float position_obstacle_y;  

float position_tmp_x;  // position temporaire 
float position_tmp_y;

float pi = 3.1415926535;  //redefinition de pi

int compte_teta= 0;  // c'est un compteur qui compte le nombre de fois que teta change. Exemple si teta vaut 50 et compte_teta 2, quand teta vaut 55 compte_teta vaudra 3.
int robot_immobile = 0;  // compteur qui s'incremente quand le robot de bouge pas. 
float x1_ligne,x2_ligne,y1_ligne,y2_ligne = 0;  // Une ligne va etre creer lors de la regression lineaire. Cette ligne est initialise a (0,0,0,0) au debut. 

float sauveur=0;  // permet de sauver la valeur d'une variable pour la comparer par la suite. Ici on l'utilise pour comparer la valeur recu et celle recu precedement. 

// fonction setup de processing
void setup() {
  size(800,800);  //taille de la salle 8m x 8m
  background(0);  
  //1 cm = 1 pixel
  
  // port serial 
  String portName = "COM3";  //on utilise COM3
  myPort = new Serial(this, portName, 9600);  
  myPort.bufferUntil(lf);  // lf = $
  
}


void draw() {
  background(255);

  // ici on pourra mettre une fonction qui cree de nouveau objet ligne quand on aura la position du robot. Pour avoir autant de ligne qu'il y a d'analyse de l'environnement. 
  line(x1_ligne,x2_ligne,y1_ligne,y2_ligne); //dessin de la ligne

  conversion_serial_en_float(); //fonction qui filtre puis convertie la valeur du port serial et renvoie le resultat en float.
  regression_lineaire();   // fonction qui calcul la regression lineaire a partir d'un nuage de point
  affiche_robot();  //affiche le robot
  ajoute_nouveau_obstacles();  // ajouter un nouvel obstacle dans la liste des obstacles
  affiche_obstacles();  // affiche les obstacles 
}



// Fonction de gestion des evenement  
void serialEvent(Serial p){
  try {  // try.. catch pour eviter les erreurs lors de la connexion au port serie. 
    inString = p.readString(); 
  }
  catch (Exception e) {}
}

// fonction parseString
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

  String[] tmp_tableau = split(inString,','); //on lit le port serie et on enregistre les resultats dans tmp_tableau

  if(valeur_recu_temporaire[0] != float(tmp_tableau[0])){ // pour eviter les doublons
    if(float(tmp_tableau[0])>384 || float(tmp_tableau[0]) <20){} //d'apres nos tests dans ]20,384[ les valeurs sont corrects
      // valeur non fiable car hors limite
    else{
      valeur_recu_temporaire[0] = float(tmp_tableau[0]); // distance 
    }
  }

  if(valeur_recu_temporaire[1] != float(tmp_tableau[1])){ // pour eviter les doublons
    
    valeur_recu_temporaire[1] = float(tmp_tableau[1]); //teta
    compte_teta++; // on a un nouveau teta donc compte teta augmente. 
  }
  
  //si robot bouge on met robot_immobile à 0. On remet compte teta à 0. 
}

void affiche_robot(){  // affichage du robot 
  fill(100,0,0);
  robot_immobile++;  //ce n'est pas fini mais plus tard si le robot bouge, on remet ce compteur a 0. Cela permet de savoir depuis combien de "temps" il est a cette position. 
  circle(position_robot_x,position_robot_y,40);  //trace un cercle qui correspond a la position du robot. 
}

void ajoute_nouveau_obstacles(){
  // convertir les valeurs polaires en cartésienne
  if(valeur_recu_temporaire[1] != sauveur){
    sauveur= valeur_recu_temporaire[1];
    // convertir les valeurs polaires en cartésienne
    position_obstacle_x = position_robot_x + valeur_recu_temporaire[0]*cos(valeur_recu_temporaire[1]*(pi/180)); 
    position_obstacle_y = position_robot_y - valeur_recu_temporaire[0]*sin(valeur_recu_temporaire[1]*(pi/180));
    //print("\nPostion x :"+position_obstacle_y+" Décalage "+valeur_recu_temporaire[0]*sin(valeur_recu_temporaire[1]*(pi*180))); // decommenter cette partie pour voir les positions. 
    
    // on ajoute dans la liste les nouvelles positions d'obstacles selon x et selon y. 
    liste_position_obstacle = splice(liste_position_obstacle, position_obstacle_x, nb_elem_liste_position_obstacle);
    nb_elem_liste_position_obstacle++;
    liste_position_obstacle = splice(liste_position_obstacle, position_obstacle_y, nb_elem_liste_position_obstacle);
    nb_elem_liste_position_obstacle++;
  }
}

void affiche_obstacles(){
  // parcourir la liste des obstacles et affiche un petit cercle pour chacun d'eux
  for(int i=0;i<nb_elem_liste_position_obstacle-1;i+=2){
    fill(0,100,0);
    //point(liste_position_obstacle[i],liste_position_obstacle[i+1]);
    circle(liste_position_obstacle[i],liste_position_obstacle[i+1],4);
  }
}

void regression_lineaire(){  
  // la regression lineaire est possible si le robot est immobile depuis un moment. 
  // plus tard la regression lineaire se fera des que le robot commmence a se deplacer et qu'on a assez de valeur. 
  if( compte_teta%17 == 0 ){ //robot_immobile == 0 && compte_teta%17 == 0
    float[] x = new float[compte_teta]; //creation d'un tableau de compte_teta valeur representant l'ensemble x. 
    float[] y = new float[compte_teta]; //pareil pour y 

    for(int i=0;i<compte_teta-1;i++){
      // remplir y
      y[i]=liste_position_obstacle[(nb_elem_liste_position_obstacle-(2*i)-1)];
      // remplir x 
      x[i]=liste_position_obstacle[nb_elem_liste_position_obstacle-(2*i)-2];
    }

    int nombre_element_x = compte_teta;
    float somme_x = 0;
    // calcul de la somme de x c'est a dire l'ensemble de tous les elements de la liste de x. 
    for(int i=0;i<nombre_element_x;i++){
      somme_x = somme_x + x[i];
    }

    float somme_y = 0;
    // calcul de la somme de y
    for(int i=0;i<nombre_element_x;i++){
      somme_y = somme_y + y[i];
    }

    float somme_x2 = 0;
    // calcul de la somme de x au carre
    for(int i=0;i<nombre_element_x;i++){
      somme_x2 = somme_x2 + x[i]*x[i];
    }

    float somme_xy = 0;
    // calcul de la somme de x*y
    for(int i=0;i<nombre_element_x;i++){
      somme_xy = somme_xy + x[i]*y[i];
    }
    // Equation de forme Ax = B donc x = A^-1 + B 
    // A doit être vu comme une matrice 2x2 mais comme c'est un tableau ici c'est plat. A = [a b c d]
    float[] A = {somme_x, nombre_element_x, somme_x2, somme_x};
    float[] A_inverse = {0,0,0,0};
    //inverse de A = 1 /(ad-bc) x [d -b -c a]. Si ad-bc == 0 aucune solution
    if(somme_x * somme_x - nombre_element_x * somme_x2 != 0){
      //inverse A dans chaque composante
      float tmp = 1/((somme_x * somme_x) - (nombre_element_x * somme_x2));
      A_inverse[0]= tmp * A[3]; 
      A_inverse[1]= tmp * (-A[1]);
      A_inverse[2]= tmp * (-A[2]);
      A_inverse[3]= tmp * A[0];

      float[] B = {somme_y,somme_xy}; //a voir comme une matrice colonne
      // resolution du systeme pour trouver les deux solutions
      float solution1 = A_inverse[0]*B[0] + A_inverse[1]*B[1];
      float solution2 = A_inverse[2]*B[0] + A_inverse[3]*B[1];
      //print("\n"+solution1); //decommenter pour afficher les valeurs 
      //print("\n"+solution2);
      
      //creation de y_droite qui represente les nouveaux points de la droite 
      float[] y_droite = new float[0];
      for(int i=0;i<nombre_element_x;i++){  //remplir la liste
        y_droite = splice(y_droite,(x[i]*solution1+solution2),i);
        //print("\n ici "+y_droite[i]);
      }

      // la ligne creer au debut recevra ces valeurs 
      x1_ligne = x[0];
      x2_ligne = y_droite[0];
      y1_ligne = x[nombre_element_x-2];
      y2_ligne = y_droite[nombre_element_x-2];
      
      // print("\nLigne : " + x[0] +"  "+ y_droite[0] +"  "+ x[nombre_element_x-1] +"  "+ y_droite[nombre_element_x-1]);
    }
  }
}
