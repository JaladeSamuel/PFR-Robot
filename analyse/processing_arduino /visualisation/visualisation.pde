import processing.serial.*;

// Serial
Serial myPort;  // créer l'objet sérial 
String inString ="";  // Input string from serial port:
int lf = 36;      // ASCII saut de ligne

String[] items = new String[2];
float [] valeur_recu_temporaire = new float[2];

float position_robot_x = 400;
float position_robot_y = 400;

// =======================================================
void setup() {
  size(800,800);
  background(0);

   // Serial Port
  String portName = "COM5";  //Serial.list()[0]; // choose the right port!
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil(lf);
  
}


void draw() {
  // dark Background
  background(255);
  conversion_serial_en_float();
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
  valeur_recu_temporaire[0] = float(tmp_tableau[0]);
  valeur_recu_temporaire[1] = float(tmp_tableau[1]);
  print("\n"+valeur_recu_temporaire[0]+" "+valeur_recu_temporaire[1]);
}
