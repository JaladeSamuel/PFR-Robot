import processing.serial.*;

// Serial
Serial myPort;  // créer l'objet sérial 
String inString ="";  // Input string from serial port:
int lf = 36;      // ASCII saut de ligne

String[] items = new String[2];
float[] valeur_recu_temporaire = new float[2];
float[] liste_position_obstacle = new float[0]; 
int nb_elem_liste_position_obstacle = 0; 


float position_robot_x = 400;
float position_robot_y = 400;
float position_obstacle_x;
float position_obstacle_y;
float position_tmp_x;
float position_tmp_y;
float pi = 3.1415926535;

// =======================================================
void setup() {
  size(800,800);
  background(0);
  //1 cm = 1 pixel
  
  // Serial Port
  String portName = "COM5";  //Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil(lf);
  
}


void draw() {
  background(255);
  conversion_serial_en_float();
  affiche_robot();
  ajoute_nouveau_obstacles();
  affiche_obstacles();
}



// =============== EVENTS ==================
void serialEvent(Serial p)
{
  try {
    inString = p.readString(); 
    //items = parseString(inString); // want to split String?  
  }
  catch (Exception e) {}
}

// ================= UTILS ========================
String[] parseString(String serialString)
{
  if (serialString != null)
  {
    serialString = serialString.replaceAll("(\\r|\\n)", "");
    String items[] = splitTokens(serialString, " =");
    return(items);
  }
  else return(null);  
}

void conversion_serial_en_float(){
  String[] tmp_tableau = split(inString,',');
  valeur_recu_temporaire[0] = float(tmp_tableau[0]); //distance
  valeur_recu_temporaire[1] = float(tmp_tableau[1]); //teta
}

void affiche_robot(){
  fill(100,0,0);
  circle(position_robot_x,position_robot_y,40);
}

void ajoute_nouveau_obstacles(){
  // convertir les valeurs polaires en cartésienne
  position_obstacle_x = position_robot_x + valeur_recu_temporaire[0]*cos(valeur_recu_temporaire[1]*(pi/180));
  position_obstacle_y = position_robot_y - valeur_recu_temporaire[0]*sin(valeur_recu_temporaire[1]*(pi/180));
  print("\nPostion x :"+position_obstacle_y+" Décalage "+valeur_recu_temporaire[0]*sin(valeur_recu_temporaire[1]*(pi*180)));
  
  liste_position_obstacle = splice(liste_position_obstacle, position_obstacle_x, nb_elem_liste_position_obstacle);
  nb_elem_liste_position_obstacle++;
  liste_position_obstacle = splice(liste_position_obstacle, position_obstacle_y, nb_elem_liste_position_obstacle);
  nb_elem_liste_position_obstacle++;
}

void affiche_obstacles(){
  for(int i=0;i<nb_elem_liste_position_obstacle-1;i+=2){
    fill(0,100,0);
    //point(liste_position_obstacle[i],liste_position_obstacle[i+1]);
    circle(liste_position_obstacle[i],liste_position_obstacle[i+1],4);
  }
}


