String trameEntrante = "";              // Un string pour récupérer la trame NMEA entrante
String trameCherche = "$GPGGA";         // Définit la trame recherchée (Ici GGA)
boolean trameComplete = false;          // Si le string est complet
String latitude ="";
String longitude ="";

void setup() 
{
  Serial.begin(4800);
  trameEntrante.reserve(200);           // Reserve 200 bytes pour la trame NMEA entrante.
}

void loop() 
{
  analyseTrame();
}

//____________________________________________________________________________________________________________________________
//RECUPERATION DE LA TRAME JUSQU'A UNE NOUVELLE LIGNE
//____________________________________________________________________________________________________________________________
void serialEvent()
{
  while (Serial.available()>20)            // Récupère un nouveau Byte
  {
    char inChar = (char)Serial.read();     // L'ajoute à "trameEntrante"
    trameEntrante += inChar;               // Ajoute un nouveau caractère au string, sans en supprimer son contenu
    if (inChar == '\n')                    // Si il y a un retour à la ligne, définit la trame comme étant terminée
    {
      trameComplete = true;
    }
  }
}


void analyseTrame()
{
  if(trameComplete)                         // Affiche la trame quand une nouvelle trame arrive
  {
      if(trameEntrante.startsWith("$GPGGA"))
      {
        //Serial.println(trameEntrante);
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
        
        //________________________________________________________________________________________________________________________
        
            // Enregistrement du début de la trame en tant que Latitude
        latitude += trameEntrante.substring(0,2); 
        latitude += " ";
        trameEntrante = trameEntrante.substring(2,trameEntrante.length());
        
        latitude += trameEntrante.substring(0,trameEntrante.indexOf(','));
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
        latitude += " ";
            // Enregistrement du début de la trame en tant que Latitude
        latitude += trameEntrante.substring(0,trameEntrante.indexOf(',')); 
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
        //Serial.print("Latitude= ");
        //Serial.println(latitude);
        
        //__________________________________________________________________________________________________________________________

            // Enregistrement du début de la trame en tant que longitude
        longitude += trameEntrante.substring(0,3); 
        longitude += " ";
        trameEntrante = trameEntrante.substring(3,trameEntrante.length());

        longitude += trameEntrante.substring(0,trameEntrante.indexOf(',')); 
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
        longitude += " ";
            // Enregistrement du début de la trame en tant que Latitude
        longitude += trameEntrante.substring(0,trameEntrante.indexOf(',')); 
            //Suppression du début de la trame jusqu'à la prochaine virgule
        trameEntrante = trameEntrante.substring(trameEntrante.indexOf(',')+1,trameEntrante.length()); 
        //Serial.print("Longitude= ");
        //Serial.println(longitude);
        //Serial.println(" ");
        convert();
      }
      

      //_____________________________________________________________________________________________________________________________
      
      trameEntrante = ""; // Efface la trame
      latitude = "";      // Efface la trame
      longitude = "";     // Efface la trame
      trameComplete = false;
  }
}

//_________________________________________________________________________________________________________________

void convert()
{
  String deg = longitude.substring(0,trameEntrante.indexOf(' '));               // Enregistrement du début de la trame en tant que degrés
  longitude = longitude.substring(longitude.indexOf(' ')+1,longitude.length()); // Suppression du début de la trame jusqu'au prochain espace
  
  String minu = longitude.substring(0,3);                                       // Enregistrement du début de la trame en tant que minutes décimales
  longitude = longitude.substring(longitude.indexOf('.'),longitude.length());   // Suppression du début de la trame jusqu'au prochain point

  String dec = longitude.substring(0,longitude.length());                       // Enregistrement du début de la trame en tant que secondes décimales
  longitude = longitude.substring(longitude.indexOf('.')+1,longitude.length()); // Suppression du début de la trame jusqu'au prochain point
  
  float degf= deg.toFloat();      // Passage du String en Float (degrees)
  float minuf=minu.toFloat();     // Passage du String en Float (Minutes)
  float decf=dec.toFloat();       // Passage du String en Float (Secondes décimales)
  float sec;                      // Secondes Base(60)
  float degD;                     // Decimal degrees
  float latf;                     // Latitude en décimal (Float)
  float longf;                    // Longitude en décimal (Float)

  sec = decf*60;
  //___________________________________
  if (degf >= 0)
  {
    degD =(sec/3600) + (minuf/60);
  }
  else                                    // Conversion 
  {
    degD = -(sec/3600) - (minuf/60);
  }
  longf = degf + degD;
  //___________________________________
  
  Serial.print("Longf=");
  Serial.println(longf,7); // Affichage de la longitude avec une précision de 7 chiffres après la virgule
  
  Serial.println("________________________________________"); /*
  Serial.print("degf=");
  Serial.println(degf,7);
  Serial.print("minuf=");
  Serial.println(minuf,7);
  Serial.print("dec=");  
  Serial.println(decf,7);
  Serial.print("sec=");
  Serial.println(sec,7);
  Serial.println("________________________________________"); */

  deg = latitude.substring(0,trameEntrante.indexOf(' '));                       // Enregistrement du début de la trame en tant que degrés
  latitude = latitude.substring(latitude.indexOf(' ')+1,latitude.length());     // Suppression du début de la trame jusqu'au prochain espace
  
  minu = latitude.substring(0,2);                                               // Enregistrement du début de la trame en tant que minutes décimales
  latitude = latitude.substring(latitude.indexOf('.'),latitude.length());       // Suppression du début de la trame jusqu'au prochain point

  dec = latitude.substring(0,latitude.length());                                // Enregistrement du début de la trame en tant que secondes décimales
  latitude = latitude.substring(latitude.indexOf('.')+1,latitude.length());     // Suppression du début de la trame jusqu'au prochain point

  degf= deg.toFloat();    //Passage du String en Float (degrees)
  minuf=minu.toFloat();   //Passage du String en Float (Minutes)
  decf=dec.toFloat();     //Passage du String en Float (Secondes décimales)
  
  sec = decf*60;

  //___________________________________
  if (degf >= 0)
  {
    degD =(sec/3600) + (minuf/60);
  }
  else                                    // Conversion 
  {
    degD = -(sec/3600) - (minuf/60);
  }
  latf = degf + degD;
  //___________________________________
  
  Serial.print("latf=");
  Serial.println(latf,7);   // Affichage de la latitude avec une précision de 7 chiffres après la virgule
  
  
  /* Serial.println("________________________________________");
  Serial.print("degf=");
  Serial.println(degf,7);
  Serial.print("minuf=");
  Serial.println(minuf,7);
  Serial.print("dec=");  
  Serial.println(decf,7);
  Serial.print("sec=");
  Serial.println(sec,7); */
  Serial.println("________________________________________"); 
}


