#ifndef INC_EVManager
#define INC_EVManager
/*------------------------------------------------------------------------
Un évènement est défini dans l'énumération EV déclarée dans quiri.h
Le gestionnaire d'évènement accepte des évènements par sa fonction Push()
Ces évènements sont enregistrés dans une pile FIFO
Les évènements sont distribués par SendAll() à tous les listeners enregistrés par AddListerner() 
jusqu'à ce que la pile FIFO soit vide.
------------------------------------------------------------------------*/

#include "arduino.h"
#include "EVentList.h"

//---- Helper pour débogguer avec le nom de la fonction englobante. note: On commence par un '.' les affichages de debug.
#define Debug \
  if (DEBUG) cout << '.' << __func__ << " "
#define debug(a) \
  if (DEBUG) cout << '.' << __func__ << "() " << #a << "=" << (a) << '\n'
//usage debug(myVar);
//usage debug(myVar1) << " autreVar" << autreVar << '\n' ;

//--- Affichage sur le PC avec la liaison USB -----------------
#define cout Serial2
template<class T> inline Print& operator<<(Print& obj, T arg) {
  obj.print(arg);
  return obj;
}

//---- BEGIN MOFIFIABLE BY USER
#define DEBUG true
#define MAXLISTENERS 15
#define logMAXEVENTQUEUE 10  // 2^6 soit 64 évènements dans la FIFO. TODO ajuster à la consommation effective (10 => liste de 1024)
//---- END MOFIFIABLE BY USER
//---- END MOFIFIABLE BY USER

#define MAXEVENTQUEUE (1 << logMAXEVENTQUEUE)  // Cela permet le rollup de l'anneau FIFO avec un simple ET binaire.

// classe mère pour les objets gérés par EVmanager (pas forcément construits comme des fsm)
class EVClasse {
public:
  enum { ContinuerEVDistribution = false,
         CesserEVDistribution = true };
  EVClasse(/* args */);
  ~EVClasse();
  virtual bool Event(EV event, int32_t param);
};  //END EVClasse

void NewObjectCounter();

// ajoute un objet recevant des évènements
bool AddListerner(EVClasse* fsm);

//accepte un nouvel évènement dans la liste FIFO
bool Push(EV event, int32_t param);

//-- Retire un évènement de la liste FIFO
bool Pull(EV* event, int32_t* param);

// distribue tous les évènement à [presque] tous les listener
// note : [presque] car un listener peut consommer un évènement particulier, dans ce cas celui-ci ne sera pas distribué plus loin
void SendAll();

#endif
