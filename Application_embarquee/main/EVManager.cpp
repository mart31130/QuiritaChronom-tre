#include <arduino.h>
#include "EVManager.h"

// plus besoin de l'affichage Debug dans ce fichier cpp
#undef DEBUG
#define DEBUG false


//---------------- Gestion des évènements ------------------------
/* tampon circulaire des évènements
    sommet pointe sur la prochaine case vide => on écrit et ensuite on incrémente
    base pointe sous le prochain évènement à distribuer => on incrémente et ensuite on lit
   */
EV eventQueue[MAXEVENTQUEUE] = { EV::None };
uint32_t eventParam[MAXEVENTQUEUE] = { 0 };
uint16_t eventCount = 0, sommet = 1, base = 0;

//-- Ajoute un évènement dans la liste FIFO
bool Push(EV event, int32_t param) {
  if (eventCount >= MAXEVENTQUEUE) return false;  // le tampon est plein
  eventQueue[sommet] = event;                     //on le place à sommet car sommet pointe toujours la prochaine case libre
  eventParam[sommet] = param;
  __disable_irq();
  sommet = (1 + sommet) & (MAXEVENTQUEUE - 1);  // sommet pointe à présent la prochaine case libre
  eventCount++;
  __enable_irq();
  Debug << "[" << sommet << "] {" << (uint16_t)event << "," << param << "} EVcnt=" << eventCount << "\n";
  return true;
}  //Push()

//-- Retire un évènement de la liste FIFO
bool Pull(EV* event, int32_t* param) {
  if (eventCount == 0) return false;  // le tampon est vide
  __disable_irq();
  base = (1 + base) & ((1 << logMAXEVENTQUEUE) - 1);  // base monte à la première case occupée par un évènement
  eventCount--;
  __enable_irq();
  *event = eventQueue[base];
  *param = eventParam[base];
  Debug << "[" << base << "] {" << (uint16_t)eventQueue[base] << "," << eventParam[base] << "} EVcnt=" << eventCount << "\n";
  return true;
}  //Pull()

//-----------------Gestion des objets EVClasse ----------------------------------
// Tableau de pointeurs sur des objets EVClasse et dérivés
EVClasse* fsmlist[1 + MAXLISTENERS];
uint16_t fsmCount = 0;
uint32_t newObject = 0;
void NewObjectCounter() {
  newObject++;
}

// ajoute un objet recevant les évènements
bool AddListerner(EVClasse* fsm) {
  if (fsmCount > MAXLISTENERS) return false;
  fsmlist[fsmCount] = fsm;
  fsmCount++;  // fsmCount pointe la prochaine case libre
  Debug << "fsmCount=" << fsmCount << " *=" << (uint32_t)fsmlist[fsmCount - 1] << '\n';
  return true;
}  //END AddListerner()

//distribue les évènement à {tous les listener, ou jusqu'à ce qu'un listener le consomme}
void SendAll() {
  EV event;
  int32_t param;
  while (Pull(&event, &param))
    for (auto i = 0; i < fsmCount; i++)
      if (fsmlist[i]->Event(event, param)) break;  // exit for si un listener a consommé l'évènement
  // la pile est vide, on envoit l'événement "plus dévénements"
  for (auto i = 0; i < fsmCount; i++) fsmlist[i]->Event(EV::NoMore, 0);
}  //END sendAll()


/*-----------------------------
	Définition de la classe mère dont dérivent les objets gérés par EVmanager
*/
EVClasse::EVClasse(/* args */) {}
EVClasse::~EVClasse() {}
bool EVClasse::Event(EV event, int32_t param) {
  Debug << '{' << (uint16_t)event << ',' << param << "} in EVClasse\n";
  return ContinuerEVDistribution;
}