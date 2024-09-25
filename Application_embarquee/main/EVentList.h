/*!
* Nom du projet: QUIRITACHRONOMETRE
* @file <EVentList.h>
* @brief <ennum des evenements du QUIRITA-CHRONOMETRE> 
* @date < 07/03/2024> 
----------------------------------------
 *   Fichier de l'eventManager à modifier
*/
#ifndef INC_EVentList
#define INC_EVentList

enum class EV : int16_t {
  //---- évènements généraux
  None = 0,      // nonévénement. cet événement doit être ignoré par tout le monde.
  ColdReset,     // démarrage à froid, tout doit être initialisé et les valeurs paramétrées doivent être relues depuis l'EEPROM
  HotReset,      // démarrage à chaud.
  Dummy,         // Evénement qui ne sert à rien
  Systic_10MIN,  // séquencement du système. le paramètre informe du nombre de Systics restant avant la consolidation des données dans la carte SD
  Systic_100MS,

  //---- BEGIN USER CODE ---------------------------------
  // conseil: initialisez chaque premier type d'évènement pour l'identifier plus facilement en mode debug affichage hexa

  //--- événements batterie
  LowBatt = 0x0100,  // param tension en mV
  BatterieLevel_MV,     // mesure de la tension batterie
  BatterieLevel_PC,

  //---- événements d'interaction IHM
  ShellActif = 0x0110,  // le paramètre rappelle la commande en ASCII sur 4 lettres (list = 6C 69 73 74 )

  //---- Bouton IHM
  ShortPress = 0x0300,  // Appuit court détecté. Param = durée de l'appuit en ms
  LongPress,            // Appuit long détecté.  Param = durée de l'appuit en ms

  //---- cafeteur Spy
  SpyIgnoreEven = 0x0400,  // l'événement à filter est dans le paramètre
  Spy_ON_OFF,              // autorise ou non l'affichage par Spy
  SpyPullIgnoreList,       // l'événement à retirer de la liste est dans le paramètre
  SpyPushEvent,            // l'événement qui doit être ajouter

  //---- Calibrage
  Calibrage = 0x1900,

  //---- Capteur de Température
  Temperature = 0x2100,                // Valeur de température disponible | Unité : Déci °C (210 = 21.0°C)

  //---- Capteur d'humidité
  Humidite = 0x2200,                    // Valeur de température disponible | Unité : Déci % (454 = 45,4%)

  //---- Capteur d'Acceleration
  SeuilAtteintAccelerometre = 0x2300,  // Le seuil d'acceleration est atteint | Généré par interruption
  Accelerometre,                       // Valeur de l'accelerometre disponible (Module des axes X Y Z)
  AccelerometreX,                      // Valeur X de l'accelerometre
  AccelerometreY,                      // Valeur Y de l'accelerometre
  AccelerometreZ,                      // Valeur Z de l'accelerometre
  ModifierSeuilAcce,                   // Nouvelle valeur du seuil
  BesoinSeuilAcce,                     // Demande du seuil
  ValeurSeuilAcce,                     // Valeur du seuil en réponse à une demande

  //---- Capteur Lux
  Lux = 0x2500,                       // Valeur du luxmétre disponible | Unité : Lux

  //---- Capteur Microphone
  SeuilAtteintMicro = 0x2600,  // Le seuil du Microphone est atteint
  Micro,                       // Valeur du microphone disponible | Unité : Valeur Analogique (~mV)
  ModifierSeuilMic,            //Nouvelle valeur du seuil
  BesoinSeuilMic,              // Demande du seuil
  ValeurSeuilMic,              // Valeur du seuil en réponse à une demande

  //---- Carte SD
  SDLvl = 0x2800,
  //---- END USER CODE -------------------------------------

  NoMore = 0x7FFF  // l'évènement "plus d'évènements à traiter" doit être le tout dernier de cette liste
};                 //END enum class EV
#endif