
/*----------------------------------------
 *   Fichier de l'eventManager à modifier
*/
#ifndef INC_EVentList
#define INC_EVentList

enum class EV : int16_t {	
	//---- évènements généraux
	None=0,    // nonévénement. cet événement doit être ignoré par tout le monde.
	ColdReset, // démarrage à froid, tout doit être initialisé et les valeurs paramétrées doivent être relues depuis l'EEPROM
	HotReset,  // démarrage à chaud.
	Dummy,     // Evénement qui ne sert à rien
	Systic,	   // séquencement du système. le paramètre informe du nombre de Systics restant avant la consolidation des données dans la carte SD


//---- BEGIN USER CODE ---------------------------------
  	// conseil: initialisez chaque premier type d'évènement pour l'identifier plus facilement en mode debug affichage hexa
    
  	//--- événements batterie
  	LowBatt=0x0100, // param tension en mV
  	
  	//---- évènement microphone
  	SeuilAtteint=0x2000, // Le seuil de bruit est atteint
  	
  	//---- Bouton IHM
  	ShortPress=0x0300, // Appuit court détecté. Param = durée de l'appuit en ms
  	LongPress,	       // Appuit long détecté.  Param = durée de l'appuit en ms
  
    //---- cafeteur Spy 
    SpyIgnoreEven=0x0400, // l'évènement à filter est dans le paramètre
    Spy_ON_OFF,           // autorise ou non l'affichage par Spy

    //---- Capteur de Température
    SeuilAtteintTemp=0x2100, // Le seuil de température est atteint
    Temperature, // Valeur de température disponible

    //---- Capteur d'humidité
    SeuilAtteintHumid=0x2200, // Le seuil d'humidité est atteint
    Humidite, // Valeur de température disponible

     //---- Capteur d'Acceleration
     SeuilAtteintAccelerometre=0x2300, // Le seuil d'acceleration est atteint
     Accelerometre, // Valeur de l'accelerometre disponible

     //---- Capteur Gyroscope
     SeuilAtteintGyroscope=0x2400, // Le seuil du Gyroscope est atteint
     Gyroscope, // Valeur du Gyroscope disponible

    //---- Capteur Lux
     SeuilAtteintLux=0x2500, // Le seuil de luminosité est atteint
     Lux, // Valeur du Gyroscope disponible
//---- END USER CODE -------------------------------------
	
	  NoMore=0x7FFF // l'évènement "plus d'évènements à traiter" doit être le tout dernier de cette liste
}; //END enum class EV
	
#endif
