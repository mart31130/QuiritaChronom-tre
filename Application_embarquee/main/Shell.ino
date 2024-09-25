/*!
* Nom du projet: QUIRITACHRONOMÈTRE 
* @file Shell.ino
* @version : 
* @brief : fichier source de l'application technicien 
* @author DECLIPPEL Anthony
* @date 13/03/2024 
* 20240505 DaK ajout des commandes d'enregistrement en EEPROM (DB)
* 20240604 Ajout de la possibilité d'interpréter l'une ou l'autre des liaisons séries (liaison USB pour le technicien, HC05 pour l'appli soignant)
*  remplacer tous les "cout << ..."  par "shell << ..." dans ce fichier uniquement est suffisant pour ajouter la fonctionnalité.
*  note :
*  shell.attach() peut être appelé plusieurs fois et le changement dans shell.cpp est pris en compte immédiatement.
*  l'objet shell étant lui-même un stream, il suffit de l'utiliser directement pour afficher !
*/

// Commandes de l'application technicien
// id? renvois le nom et le build de l'application"), cmd_ID);
// echo [decimal|ascii|hexa] ...               exemple de commande pour se rappeler comment décoder des arguments
// spy [on|off] | [ev+|ev-] eventnumber        expl: "spy off"
// push EVENT PARAM                            expl: "push 0x2501 800"     lumiere=800 Lux.
// db list                                     expl: "list"                liste le contenu de l'EEPROM
// db format                                   expl: "list"                formate le contenu de l'EEPROM
// db get varname                              expl: "db get sonorSeuil" 
// db set varname iValue|cValue                expl: "db set sonorSeuil 2047" "db set QuiritaIdentifiant Mon beau Sapin"


#include "Spy.h"
#include "EVManager.h"
#include "EVentList.h"
#include "SimpleSerialShell.h" //l'objet shell est déclaré en externe dans le .h de SimpleSerialShell 
#include "DB.h"


int cmd_ID(int /*argc*/ = 0, char ** /*argv*/ = NULL) {
  shell << "Running Quiritachronometre build " << __DATE__ << '\n';
  return EXIT_SUCCESS;
}//cmd_ID()


int cmd_echo(int argc, char **argv) { Push(EV::ShellActif,0x6563686F);
  for (int i = 1; i < argc; i++) {
    if (isdigit(argv[i][0])) {
      long iparam = 0;
      iparam = strtol(argv[i], nullptr, 0);  // ok pour décimal 1234567890 et hexadécimal 0x55FF00AA
      shell << "\n  param[" << i << "] is the number->" << iparam << " read from '" << argv[i] << '\'';
    } else {
      shell << "\n  param[" << i << "] is the string->" << argv[i] << " read from '" << argv[i] << '\'';
    }
  }
  shell << "\n\n";
  return EXIT_SUCCESS;
}  //cmd_echo()


// note : tous les arguments sont insensibles à la case "SPY ON" == "spy on" == "Spy On"
// spy on                     Mise en acutualisation des événements (par défaut)
// spy off                    Mise en pause de l'actualisation des événements 
// spy ev+ eventnumber        ajoute un ou plusieurs événements dans la liste à ignorer
// spy ev- eventnumber        retire un ou plusieurs événements dans la liste à ignorer
// spy default                reset aux évènements ignorer par défaut
int cmd_spy(int argc, char **argv) { Push(EV::ShellActif,0x73707920);
  //---- spy ON -------
  if (strcasecmp(argv[1], "ON") == 0) {
    shell << " Spy is ON\n";
    Push(EV::Spy_ON_OFF, true);
    return EXIT_SUCCESS;
  }//spy on
  //---- spy OFF -------
  if (strcasecmp(argv[1], "OFF") == 0) {
    shell << " Spy is OFF\n";
    Push(EV::Spy_ON_OFF, false);
    return EXIT_SUCCESS;
  }//spy off
  //---- spy ev+ --------
  if (strcasecmp(argv[1], "EV+") == 0) {
    if (argc <= 2) { goto cmd_spy_exit_with_error; }
    for (int i = 2; i < argc; i++) {
      if (!isdigit(argv[i][0])) { goto cmd_spy_exit_with_error; }
      long iparam = strtol(argv[i], nullptr, 0);
      shell << "Event " << argv[i] << " will be ignored\n";
      Push(EV::SpyIgnoreEven, iparam);
    }
    return EXIT_SUCCESS;
  }//spy ev+ EVENT [...]
  //---- spy ev- -------
  if (strcasecmp(argv[1], "EV-") == 0) {
    if (argc <= 2) { goto cmd_spy_exit_with_error; }
    for (int i = 2; i < argc; i++) {
      if (!isdigit(argv[i][0])) { goto cmd_spy_exit_with_error; }
      long iparam = strtol(argv[i], nullptr, 0);
      shell << "Event " << argv[i] << " will not be ignored\n";
      Push(EV::SpyPullIgnoreList, iparam);
    }
    return EXIT_SUCCESS;
  }//spy ev- EVENT [...]
  //---- spy default ------
  if (strcasecmp(argv[1], "DEFAULT") == 0) {
    // copié de CQuirita::initialisation()
    Push(EV::SpyIgnoreEven, (uint32_t)EV::Systic_100MS);
    Push(EV::SpyIgnoreEven, (uint32_t)EV::Micro);
    Push(EV::SpyIgnoreEven, (uint32_t)EV::ShellActif);
    Push(EV::SpyIgnoreEven, (uint32_t)EV::NoMore);
    return EXIT_SUCCESS;
  }//spy default
  
cmd_spy_exit_with_error:
  shell << " Syntax error. type HELP for help\n'";
  return EXIT_SUCCESS;
}//END cmd_spy()

//  commande EVPUSH ----------------------------------------------------
int cmd_evpush(int argc, char **argv) { Push(EV::ShellActif,0x70757368);
  if( argc!=3 ) goto cmd_evpush_exit_with_error; else {
    long ievent=strtol(argv[1],nullptr,0);
    long iparam=strtol(argv[2],nullptr,0);
    Push((EV)ievent, iparam);
    shell << "ievent=" << ievent <<  " iparam=" << iparam << " ";
    shell << " Event is posted\n'";
    return EXIT_SUCCESS;
  }
cmd_evpush_exit_with_error: shell << "Syntax error. PUSH need 2 arguments.\n'"; return EXIT_FAILURE;
}//evpush event param 

//  commande non implémentée ---------------------------------------
int cmd_dummy(int argc, char **argv){ Push(EV::ShellActif,0x64756D79);
  shell << " Command not yet implemented.\n'";
  return EXIT_SUCCESS;
}//commandes non implémentée

//  commande DB ----------------------------------------------------
int cmd_db(int argc, char **argv) { Push(EV::ShellActif,0x64622020);
  //---- db list -------
  if (strcasecmp(argv[1], "LIST") == 0) {
      AfficheDB();
      return EXIT_SUCCESS;
  }//db list
  //---- db get varname ------
  if (strcasecmp(argv[1], "GET") == 0) {
    if(argc != 3){ goto cmd_db_exit_with_error; }
    uint16_t index = db_find( argv[2] );
    if( index>0 ){
      AfficheDB(index);
    }else{
        shell << argv[2] << " not found in db\n" ;
    }
    return EXIT_SUCCESS;
  }//db get varname
  //---- db format ------
  if (strcasecmp(argv[1], "FORMAT") == 0) {
    db_format();
    AfficheDB();
    return EXIT_SUCCESS;
  }//db format    
  //---- db set varname value
  if (strcasecmp(argv[1], "SET") == 0) {
    if(argc < 4){ goto cmd_db_exit_with_error; }
    uint16_t index = db_find( argv[2] );
    if( index==0 ){ shell << argv[2] << " not found in db\n" ; goto cmd_db_exit_with_error; }
	  // retrouver l'ancienne valeur
	  size_t valueSize; uint8_t value[MAXSIZEOF]; char key[MAXSIZEOF];
    db_getTuplet( index,key,value,&valueSize );
    // db set d'un entier ou d'une chaine de caractère
	  // ces commandes ne modifient ni le type ni la taille (sizeof) de la variable originale
	  if( isdigit(argv[3][0]) ){ // traitement de db set varname ivalue
		  uint8_t param8; uint16_t param16; uint32_t param32 = strtol(argv[3],nullptr,0); 
		  switch( valueSize ){
		  case 4 : db_set(argv[2], (uint8_t*)&param32, valueSize); break;
		  case 2 : param16= (uint16_t)(param32&0xFFFF); db_set(argv[2], (uint8_t*)&param16, valueSize); break;
		  case 1 : param8 = (uint8_t)(param32&0xFF); db_set(argv[2], &param8, valueSize); break;
		  default : shell << "size of data type error\n" ; goto cmd_db_exit_with_error;
		  }//switch( valueSize )
	}else{ // traitement d'une chaine de caractères
		char spc[2]=" "; value[0]=0;
		for(int i=3; i<argc; i++){ // la chaine passée en paramètre est splitée dans argv[] 
      strncat( (char*)value, argv[i], valueSize );
      if(1+i<argc) strncat( (char*)value, spc, valueSize ); // ajout éventuel de l'espace séparateur
    }
		db_set(argv[2], value, valueSize);
    }//END db set d'un entier ou d'une chaine de caractère
	  AfficheDB(index); // affiche le résultat final de la commande SET
    return EXIT_SUCCESS;
  }//db set varname value

cmd_db_exit_with_error: shell << "argc=" << argc << "  Syntax error. Type HELP\n'"; return EXIT_FAILURE;
}//cmd_db 

//  commande BT pour l'application Soignant ----------------------------------------------------
int cmd_bt(int argc, char **argv) { Push(EV::ShellActif,0x65413641);
  if (strcasecmp(argv[1], "S") == 0) {  // niveau de stockage de la carte SD en %
      shell << "~s "<< quirita.GetSdValuePC() << "\n"; 
      return EXIT_SUCCESS;
  }
  if (strcasecmp(argv[1], "B") == 0) { // niveau batterie en %
      shell << "~b "<< quirita.GetBatterieValuePC() << "\n";
      return EXIT_SUCCESS;
  }
  if (strcasecmp(argv[1], "A") == 0) {
    uint32_t seuilAcce; dbget(seuilAcce);
      shell << "~a " << seuilAcce << "\n";
      return EXIT_SUCCESS;
  }
  if (strcasecmp(argv[1], "M") == 0) {
    uint32_t sonorSeuil; dbget(sonorSeuil);
      shell << "~m " << sonorSeuil << "\n";
      return EXIT_SUCCESS; 
  }
  if (strcasecmp(argv[1], "I") == 0) {
      char QuiritaIdentifiant[32]; dbget(QuiritaIdentifiant);
      shell << "~i " << QuiritaIdentifiant << "\n";
      return EXIT_SUCCESS; 
  }
}//cmd_bt

void Shell_AddCommands(){
  shell.addCommand(F("id? renvois le nom et le build de l'application"), cmd_ID);
  shell.addCommand(F("echo [decimal|ascii|hexa] ..."), cmd_echo);  // commande exemple pour se rappeler comment décoder des arguments
  shell.addCommand(F("spy [on|off] | [ev+|ev-] eventnumber  expl: \"spy off\" \"spy ev- 0x2601\""), cmd_spy);
  shell.addCommand(F("evpush EVENT PARAM  expl: \"evpush 0x2501 800\" lumiere=800 Lux."), cmd_evpush);
  shell.addCommand(F("db list|format|get varname|set varname ivalue|cvalue expl: \"db list\""), cmd_db);
  shell.addCommand(F("~ | commande spécifique pour l'application Soignant"), cmd_bt); 
  shell << "\n-------------------\n\nShell ready.\n";
  shell << "Type 'help' for help\n";
}  //SetupShell()
