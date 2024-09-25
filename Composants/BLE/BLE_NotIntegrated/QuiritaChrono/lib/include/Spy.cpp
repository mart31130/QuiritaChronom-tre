/*
 * Classe Spy dérive de EVClasse 
 * 
 * Cette classe permet d'afficher les événements traités par EVmanager
 * 
 * !! Singleton !!
 *    utilise des variables externes à l'objet mais internes à l'unité de compilation
 * créé le 25 fev 2022 par
 * modifié le --- par ---
 */


#include "Spy.h"

// plus besoin de l'affichage Debug dans ce fichier
#undef  DEBUG
#define DEBUG false

// Spy peut filtrer les évènements qu'on ne veux pas affficher    
#define MAXIGNORELIST 32
EV ignoreList[MAXIGNORELIST+1]={EV::Dummy};
int ignoreListLenght=0;
bool spyONOFF=true;


//---- Hex(data) --------- imprime data en hexadécimal sur le moniteur série ---------
// template = truc de paresseux. En regardant ses usages effectifs le compilateur va créer 
// autant de versions que nécessaire avec le même code source :
// une version 16 bits et une version 32bits.
template <typename T> void Hex(T data){
    int i=sizeof(data)*2; // le nombre de demi-octets dans data
    char tmp[1+i]; //<- n'est pas un tableau dynamique car à cet instant précis le compilateur connait la valeur exacte de i
    cout.print("0x"); //TODO injecter ce 0x en tête du tampon tmp pour économiser un appel à cout.
    tmp[i]=0; // le zéro de fin de chaine ASCII en C
    while(i>0){
      char b=data&0x0F; // un demi-octet
      tmp[--i]=(b<10)?('0'+b):('A'+b-10); // conversion demi-octet vers ASCII
      data>>=4; // prochain demi-octet
    }
    cout.print(tmp);
}//END Hex()


//----- Méthodes de la classe Spy ---------------------------------------

Spy::Spy(int x){
  ignoreList[1]={EV::Dummy};
  ignoreListLenght=1;
  NewObjectCounter(); // compteur global d'instanciations , pour vérifier si pas de fuite de mémoire
}//END constructeur Spy::Spy()

bool Spy::Event( EV event, int32_t param ){
    Debug << "{" << (uint16_t)event << "," << param << "} in Spy\n";
    // Spy écoute certains évènements pour se paramétrer
    switch(event){
      case EV::ColdReset: ignoreListLenght=0; spyONOFF=false; break;
      case EV::Spy_ON_OFF: spyONOFF = param; break;
      case EV::SpyIgnoreEven: if(ignoreListLenght<MAXIGNORELIST) ignoreList[++ignoreListLenght]=(EV)param; break;
    }//switch(event)
    // Affichage sur la console des {évènements,message} qui passent dans la machinerie
    if( spyONOFF ){
        // Spy filtre les évènements qu'on ne veux pas voir  
        int i=ignoreListLenght; 
        while(i>0) if( event==ignoreList[i--] ) return ContinuerEVDistribution;
        cout.print("EV ");
        Hex((uint16_t)event);
        cout.print(" ");
        Hex(param);
        cout.println(); // TODO on devrait pouvoir économiser tous les précédents appels à cout avec un tampon de 21 octets
    }//if(spyONOFF)
    return ContinuerEVDistribution;
}//END Spy::Event()
