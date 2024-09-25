/*
 * base de donnée NOSQL fonctionnant sur l'association clef -> valeur
 * 
 * version 1.0 DaK 2022/03/20 initial release 
 *         1.1 DaK 2022/03/30 codage db_del + improve db_find + added QuiriUID()
 *         1.2 DaK 2024/04/20 adaptation à l'eeprom M95640R de la carte fille NUCLEO.IDB05A1
 */

// STM32F4xx specifique
#define STM32_UUID ((uint32_t *)0x1FFF7A10)

#include "arduino.h"
#include "dbdriver.h" // driver de l'eeprom
#include "util_debug.h"
#include "DB.h"

#undef DEBUG
#define DEBUG false

// une eeprom travaille en pages de 32 octets
#define EEPAGESIZELOG	(5)
#define EEPAGESIZE    (1<<EEPAGESIZELOG) // 1<<5 = 32 octets par page d'eeprom
#define TUPLETSIZELOG (1+EEPAGESIZELOG)  // 1+5 = 6
#define TUPLETSIZE    (EEPAGESIZE<<1)    // 1<<6 = 64 octets par tuplet soit 2 pages d'eeprom
#define KEYSIZEMAX    (EEPAGESIZE-2)     // nombre de caractères maximum d'une clef=30. après on tronque.
#define VARSIZEINDEX  (EEPAGESIZE-1)     // la taille de la donnée est située à buffer[VARSIZEINDEX]

// Table d'allocation de l'eeprom enregistrée dans le tout premier tuplet
struct dbAT_t {
    uint32_t MAGIC1;  // doit être 0x616D694C
    uint32_t MAGIC2;  // doit être 0x63617279
    uint32_t MPUUID;  // UID de microcontrôleur
    uint32_t DBVERS;  // majeur/mineur 1.1
    uint32_t KeyCount;// nombre de tuplets {clef,valeur} enregistrés
    uint32_t MaxKeys; // 128 tuplets maximum dans une eeeprom 8Ko
    uint32_t dummy1;  // réservé
    uint32_t dummy2;  // réservé
} dbAllocationTable;
// la déclaration factice suivante va exploser si dbAT_t dépasse la taille d'une page
extern char zzz[1 - 2*(sizeof(struct dbAT_t) > EEPAGESIZE)];


// protorypes et fonctions privées
// index=n° de la page = résultat du db_find()
uint16_t _db_WrTuplet( uint16_t index, const char* key, uint8_t* value, size_t  valueSize); // fonction interne non exportée

inline void _updateAllocationTable(){eeWrite(EEPAGESIZE,sizeof(dbAllocationTable),(uint8_t*)&dbAllocationTable);}

uint8_t QuiriUID[]={0,0,0,0,99,99}; // @MAC du dispositif = 4 octets MCUuid + 2 octets arbitaires
__STATIC_INLINE uint32_t stm32F4uuid(){return STM32_UUID[0]^STM32_UUID[1]^STM32_UUID[2];} // spécifique STM32F4

//initialisation de la base de donnée NOSQL {clef,valeur}
uint16_t db_begin(){
	uint32_t mpuUID = stm32F4uuid();
	//initialiser l'eeprom 
	if( !eeInit() )return false;
	// Fabrication de l'adresse MAC du dispositif en complétant QuiriUID[]
	memcpy(QuiriUID,&mpuUID,4);
	// relire la table d'allocation de cette eeprom
	eeRead(EEPAGESIZE, sizeof(dbAT_t), (uint8_t*)&dbAllocationTable);
	if(dbAllocationTable.MAGIC1 != 0x616D694C 
	|| dbAllocationTable.MAGIC2 != 0x63617279
	|| dbAllocationTable.MPUUID != mpuUID) db_format();
  return true;
}//db_begin()

uint16_t db_format(){
    dbAllocationTable.MAGIC1 =0x616D694C; // doit être 0x616D694C
    dbAllocationTable.MAGIC2 =0x63617279; // doit être 0x63617279
    dbAllocationTable.DBVERS =0x00010002; // majeur/mineur 1.1
    dbAllocationTable.MPUUID =stm32F4uuid();
    dbAllocationTable.KeyCount=1;	// nombre de tuplets clef,valeur enregistrés
    dbAllocationTable.MaxKeys=eeMAXKEYS;	// 128 tuplets dans une 8Ko
    //------ mise à jour de la table d'allocation
    return _db_WrTuplet(0, "dbAllocationTable", (uint8_t*)&dbAllocationTable, sizeof(dbAllocationTable) );
}//db_format()

uint16_t _db_WrTuplet( uint16_t index, const char* key, uint8_t* value, size_t valueSize){
	// paramètres supposés vérifiés
	//--- enregistrement du nom et de la taille de la variable -------
  uint16_t eeAddr=(index<<TUPLETSIZELOG);
	uint8_t buffer[EEPAGESIZE];
	uint8_t keySize=strlen(key); if(keySize>=KEYSIZEMAX) keySize=KEYSIZEMAX; // Tronque la clef à 30 caractères+\0
	memcpy(buffer,key,keySize);
	buffer[keySize]=0; // fin de chaine
	buffer[VARSIZEINDEX]=(uint8_t)(valueSize); // la taille de la variable est stockée dans le dernier octet de la page
	eeWrite(eeAddr,EEPAGESIZE,buffer);
	//------ enregistrement du contenu de la variable
	eeWrite(eeAddr+EEPAGESIZE,valueSize,value);
  return index;
}//_db_WrTuplet()

// enregistre un tuplet {clef,valeur} et le crée s'il n'existe pas déjà
uint16_t db_set(const char* key, uint8_t* value, size_t valueSize){
	// validation des paramètres
	if(valueSize>EEPAGESIZE) return 0; //false==0
	uint16_t index=db_find(key);
	if(index==0){ // un nouveau tuplet doit être créé
		index=dbAllocationTable.KeyCount;
		if(index>=dbAllocationTable.MaxKeys) return 0;
		_db_WrTuplet(index, key,value,(uint8_t)valueSize);
		//------ mise à jour de la table d'allocation
		dbAllocationTable.KeyCount=(++index);
		eeWrite(EEPAGESIZE,sizeof(dbAllocationTable),(uint8_t*)&dbAllocationTable);
	}else{ // tuplet existant => mise à jour de la valeur 
		eeWrite((index<<TUPLETSIZELOG)+EEPAGESIZE,valueSize,value);
	}
	return index;
}//db_set()

//---- db_find()------------------------
// on cherche à partir de l'index 1 car l'index 0 est la table d'allocation
// ret = 0 si pas trouvé 
// ret = index du tuplet si trouvé
//noteDaK : amélioration vitesse => stocker en RAM un tableau static des 4 premiers caractères pour faire une première recherche sur des long
//noteDaK : amélioration vitesse + encombrement eeprom => stocker le crc32 de la clef plutôt que la clef elle-même. Efficace car le stm32 contient un calculateur de crc hardware.
uint16_t db_find(const char* key){
  	static char candidat[EEPAGESIZE];
  	static uint32_t candindex=0;
  	uint16_t eeAddr=0;
  	if(dbAllocationTable.KeyCount==1) return 0; // la seule clef dans l'eeprom est la table d'allocation 	
  	//La clef demandée est peutêtre la dernière clef recherchée
    if((candindex>0)&&(strcmp(key,candidat)==0)) return candindex; 
  	// ben alors y faut parcourir la DB
  	for(candindex=1; candindex<dbAllocationTable.KeyCount; candindex++){
        eeAddr+=TUPLETSIZE; // index*(2*EEPAGESIZE)
        eeRead(eeAddr,EEPAGESIZE,(uint8_t*)candidat);
        if( strcmp(key,candidat)==0 ) return candindex;
  	}//for()
  	candindex--; //l'index du dernier candidat testé avant la sortie du for
  	return 0; // pas trouvé
}//db_find()

uint16_t db_get(const char* key, uint8_t* value, size_t size){
	uint16_t index=db_find(key);
	return (index)?db_get(index,value,size):index;
}//db_get

uint16_t db_get(uint16_t index, uint8_t* value, size_t size){
	uint16_t eeAddr=(index<<TUPLETSIZELOG)+EEPAGESIZE;
	eeRead(eeAddr,size,value);
	return index;
}//db_get()

uint16_t db_getTuplet(uint16_t index, char* key, uint8_t* value, size_t* valueSize){
  uint16_t eeAddr=(index<<TUPLETSIZELOG);
  // renvoyer la clef
  eeRead(eeAddr,EEPAGESIZE,(uint8_t*)key);
  uint8_t dataSize=key[EEPAGESIZE-1];
  *valueSize=dataSize;
  // renvoyer la valeur
  eeAddr+=EEPAGESIZE;
  eeRead(eeAddr,*valueSize,value);
  return index;
}//db_getTuplet()

// db_del(key|index)efface un tuplet de la DB.
// note : l'opération prend 10 ms car 3 écritures successsives en eeprom. Seule la dernière écriture est transparente.
uint16_t db_del(uint16_t index){
  if(index==0) return false; // key non trouvée ou table d'alloc non effaçable
  if(index>=dbAllocationTable.KeyCount) return false; // tuplet n'existe pas
  uint16_t head=dbAllocationTable.KeyCount-1; // tuplet en haut de la liste
  if(index<head){ // on va écraser le tuplet index avec le tuplet du haut
    uint8_t valtmp[EEPAGESIZE]; char keytmp[EEPAGESIZE]; size_t valsize;
    db_getTuplet( head, keytmp,valtmp,&valsize );
    _db_WrTuplet( index,keytmp,valtmp, valsize  );  
  }
  // reste à effacer la tête en mettant à jour la table
  dbAllocationTable.KeyCount=head;
  _updateAllocationTable();
  return index;
}//db_del(index)

uint16_t db_del(const char* key){ return db_del(db_find(key)); }

uint16_t db_info(uint32_t* version, uint32_t* maxKey, uint32_t* keyCount, uint32_t* mpuuid){
    *version=dbAllocationTable.DBVERS;
    *maxKey=dbAllocationTable.MaxKeys;
    *keyCount=dbAllocationTable.KeyCount;
    *mpuuid=dbAllocationTable.MPUUID;
    return *maxKey>eeMAXKEYS;
}//db_info()

void AfficheDB(){
    uint32_t keyCount=dbAllocationTable.KeyCount;
    delay(10); // attendre la fin d'une éventuelle écriture en cours
  	cout << "\nDB nversion=" << (dbAllocationTable.DBVERS>>16) << "." << (dbAllocationTable.DBVERS&0xFF) 
         << " maxKey=" << dbAllocationTable.MaxKeys 
         << " keyCount=" << keyCount 
         << " mpuUID=" << dbAllocationTable.MPUUID << '\n';
    //---- pour toutes les clef
  	for(uint32_t index=0; index<keyCount; index++){
        char clef[MAXSIZEOF]=""; uint8_t valeur[MAXSIZEOF]={0}; char ascii[MAXSIZEOF+1]=""; size_t valueSize=0;
        db_getTuplet(index,clef,valeur,&valueSize);
       //---- affiche le nom et la taille
        if(index<9) cout << "0";
        cout << index << " " << clef << "[" << valueSize << "] ";
        //---- aligne les sorties
        for(size_t s=strlen(clef)+(valueSize<9?1:2);s<20;s++) cout << " " ;
        //---- affiche le contenu en hexa
        for(size_t j=0;j<valueSize;j++){
            char b,c=valeur[j];
            if(j%4==0)cout << " ";
            b=c>>4;
            cout << (char)((b<10)?('0'+b):('A'+b-10)) ;
            b=c&0x0F;
            cout << (char)((b<10)?('0'+b):('A'+b-10)) ;
            ascii[j]=((valeur[j]<127)&&(valeur[j]>32))?valeur[j]:'.';
        }
        //---- affiche les caractères ascii
        ascii[valueSize]=0;
        cout << " " << ascii << "\n";
     }//for toutes les clef
     cout << "\n";
}//AfficheDB

//-------------rappels syntaxe
/*
int strcmp ( const char * str1, const char * str2 ); // 0=> egalité
int memcmp ( const void * ptr1, const void * ptr2, size_t num ); // 0=> egalité
void* memcpy( void* destination, const void* source, size_t count );
char* strcpy( char * destination, const char * source );
*/
