/*
 * Classe ButtonIHM dérive de EVClasse 
 * 
 * Cette classe gère le capteur "bouton poussoir"
 * 
 * !! Singleton !!
 *    utilise une interruption matérielle
 *    Entrée : 
 *       N° de broche à surveiller        
 *    Sortie : 
 *       émission des évènements ShortPress et LongPress
 *    
 * créé le 25 fev 2022 par
 * modifié le --- par ---
 */
//--- WARNING : cette classe ne devrait jamais être instanciée en plusieurs exemplaires à cause de l'ISR interne
//--> TODO en faire un singleton


#include "EVManager.h"
#include "ButtonIHM.h"
// plus besoin de l'affichage Debug dans ce fichier
#undef  DEBUG
#define DEBUG false


enum state_t:uint16_t {sUndef, sInit, sRun};
enum {SHORTPRESS_MS=100, LONGPRESS_MS=1000};

// prototypes des fonctions et variables privées
// note : non déclaré private car _MyISR ne peut pas être une instance (interruption matérielle forcément unique)
state_t _sEtat = state_t::sUndef;
int  _pin;
bool _Init( EV event, int32_t param ); // initialise les variables internes de la FSM
bool _Run ( EV event, int32_t param ); // initialise les variables internes de la FSM
void _MyISR (void);

/*-------------------------------------
 *    Constructeur
 */
ButtonIHM::ButtonIHM(int pin){
    _pin = pin;
    _sEtat = state_t::sInit;
    NewObjectCounter();
}

/*---------------------------------------
 *    Point d'entrée
*/
bool ButtonIHM::Event( EV event, int32_t param ){
    Debug << '{' << (uint16_t)event << ',' << param << "} in ButtonIHM\n"; 
    switch( _sEtat ){   // passer l'évènement à l'état actuel pour traitement
    case state_t::sRun:   return _Run(event,param); 
    case state_t::sUndef: return _Init(event,param ); 
    case state_t::sInit:  return _Init(event,param ); 
    default: ; // cet état est inconnu 
    }//switch
    return ContinuerEVDistribution;
}//END event()


//--------------------- fonctions privées -------------------

bool _Init( EV event, int32_t param){
    pinMode(_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(_pin), _MyISR, CHANGE);
    _sEtat = sRun;
    return ButtonIHM::ContinuerEVDistribution;
}//END _Init()


bool _Run( EV event, int32_t param){
    // ne sert à rien pour cet objet car il est géré par interruption
    return ButtonIHM::ContinuerEVDistribution;
}//END _Run()



void _MyISR (void){
    static int previousState=HIGH; // bouton est relaché
    static uint32_t previousTicks=HAL_GetTick();
    int actualState = digitalRead(_pin);
    uint32_t actualTicks=HAL_GetTick(); // équivalent millis() des arduinos, bas niveau de l'api STM32
    if( actualState==HIGH && previousState==LOW){
        uint32_t elapsed=actualTicks-previousTicks;
        if( elapsed > LONGPRESS_MS ){
            Push(EV::LongPress, elapsed );
        }else if( elapsed > SHORTPRESS_MS ){
            Push(EV::ShortPress, elapsed );
        }
    }
    previousState=actualState;
    previousTicks=actualTicks;
}//_MyISR()
