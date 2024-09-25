/*
 * base de donnée NOSQL fonctionnant sur l'association clef -> valeur
 * 
 * version 1.0 DaK 2022/03/20 initial release 
 *         1.1 DaK 2022/03/30 codage db_del + improve db_find + added QuiriUID()
 */

// STM32F4xx specifique
#define STM32_UUID ((uint32_t *)0x1FFF7A10)

// driver de l'eeprom
#include "M95640R_emulation.h"

#include "arduino.h"
#include "util_debug.h"


#include "DB.h"
// @MAC du dispositif = 4 octets MCUuid + 2 octets arbitaires
uint8_t QuiriUID[]={0,0,0,0,99,99};

#undef DEBUG
#define DEBUG false

// une eeprom travaille en pages de 32 octets
#define EEPAGESIZELOG	(5)     // 5 est le log(32) en base 2
#define EEPAGESIZE	(1<<EEPAGESIZELOG)
#define TUPLETSIZELOG (1+EEPAGESIZELOG) //Un tuplet = {varname,value}
#define TUPLETSIZE	(EEPAGESIZE<<1)

// nombre de caractères maximum d'une clef=30. après on tronque.
#define KEYSIZEMAX  (EEPAGESIZE-2)
// la taille de la donnée est située à buffer[VARSIZEINDEX]
#define VARSIZEINDEX (EEPAGESIZE-1)

// prototypes
void _db_WrTuplet(const char* keyName, uint8_t* value, uint8_t valueSize,  uint16_t eeBaseAddr);

// Table d'allocation de l'eeprom 
struct dbAT_t {
	uint32_t MAGIC1;  // doit être 0x616D694C
	uint32_t MAGIC2;  // doit être 0x63617279
	uint32_t MPUUID;  // UID de microcontrôleur
	uint32_t DBVERS;  // majeur/mineur 1.1
	uint32_t KeyCount;// nombre de tuplets {clef,valeur} enregistrés
	uint32_t MaxKeys; // 128 tuplets maximum dans une eeprom M98640 de 8Ko
	uint32_t dummy1;  // réservé
    uint32_t dummy2;  // réservé
};
// la déclaration factice suivante va exploser si dbAT_t dépasse la taille d'une page
extern char zzz[1 - 2*(sizeof(struct dbAT_t) > EEPAGESIZE)];
static dbAT_t dbAllocationTable;

// l'objet eeprom
// pas besoin de paramètres pour le constructeur de l'eeprom simulée en ram 
M95640R ee;

//uint32_t stm32F4uuid()
__STATIC_INLINE uint32_t stm32F4uuid(){return STM32_UUID[0]^STM32_UUID[1]^STM32_UUID[2];}

//initialisation de la base de donnée NOSQL {clef,valeur}
uint16_t db_init(int dummyArg){
	//initialiser l'objet eeprom 
	ee.begin();
	uint32_t mcuUID = stm32F4uuid();
	// Fabrication de l'adresse MAC du dispositif en complétant QuiriUID[]
	memcpy(QuiriUID,&mcuUID,4);
	// relire la table d'allocation de cette eeprom
	ee.EepromRead(EEPAGESIZE, sizeof(dbAT_t), (uint8_t*)&dbAllocationTable);
	if(dbAllocationTable.MAGIC1 != 0x616D694C 
	|| dbAllocationTable.MAGIC2 != 0x63617279
	|| dbAllocationTable.MPUUID != mcuUID){   
		// il faut "formater" l'eeprom
		dbAllocationTable.MAGIC1 =0x616D694C; // doit être 0x616D694C
		dbAllocationTable.MAGIC2 =0x63617279; // doit être 0x63617279
		dbAllocationTable.DBVERS =0x00010001; // majeur/mineur 1.1
		dbAllocationTable.MPUUID =mcuUID;
		dbAllocationTable.KeyCount=1;	// nombre de tuplets clef,valeur enregistrés
		dbAllocationTable.MaxKeys=128;	// 128 tuplets dans une 8Ko
		//------ mise à jour de la table d'allocation
		_db_WrTuplet("dbAllocationTable", (uint8_t*)&dbAllocationTable, sizeof(dbAllocationTable), 0);
	}//eeprom non formatée
  return true;
}//db_init()

uint16_t db_info(uint32_t* version, uint16_t* maxKey, uint16_t* keyCount, uint32_t* mpuuid){
    *version=dbAllocationTable.DBVERS;
    *maxKey=dbAllocationTable.MaxKeys;
    *keyCount=dbAllocationTable.KeyCount;
    *mpuuid=dbAllocationTable.MPUUID;
    return true;
}//db_info()


void _db_WrTuplet(const char* keyName, uint8_t* value, uint8_t valueSize,  uint16_t eeBaseAddr){
	// paramètres supposés vérifiés
	//--- enregistrement du nom et de la taille de la variable -------
	uint8_t buffer[EEPAGESIZE];
	uint8_t keySize=strlen(keyName);
	if(keySize>=KEYSIZEMAX) keySize=KEYSIZEMAX; // Tronque la clef à 30 caractères+\0
	memcpy(buffer,keyName,keySize);
	buffer[keySize]=0; // fin de chaine
	buffer[VARSIZEINDEX]=(uint8_t)(valueSize); // la taille de la variable est stockée dans le dernier octet de la page
	ee.EepromWrite(eeBaseAddr,EEPAGESIZE,buffer);
	//------ enregistrement du contenu de la variable
	ee.EepromWrite(eeBaseAddr+EEPAGESIZE,valueSize,value);
}//_db_WrTuplet()

// enregistre un tuplet {clef,valeur} et le crée s'il n'existe pas déjà
uint16_t db_set(const char* key, uint8_t* value, size_t valueSize){
	// validation des paramètres
	if(valueSize>EEPAGESIZE) return 0; //false==0
	uint16_t index=db_find(key);
	if(index==0){ // un nouveau tuplet doit être créé
		index=dbAllocationTable.KeyCount;
		if(index>=dbAllocationTable.MaxKeys) return 0;
		_db_WrTuplet(key,value,(uint8_t)valueSize,index<<TUPLETSIZELOG);
		//------ mise à jour de la table d'allocation
		dbAllocationTable.KeyCount=(++index);
		ee.EepromWrite(EEPAGESIZE,sizeof(dbAllocationTable),(uint8_t*)&dbAllocationTable);
	}else{ // tuplet existant => mise à jour de la valeur 
		ee.EepromWrite((index<<TUPLETSIZELOG)+EEPAGESIZE,valueSize,value);
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
  	static uint16_t candindex=0;
  	register uint16_t eeAddr=0;
  	if(dbAllocationTable.KeyCount==1) return 0; // la seule clef dans l'eeprom est la table d'allocation
  	
  	//La clef demandée est peutêtre la dernière clef recherchée
    if((candindex>0)&&(strcmp(key,candidat)==0)) return candindex;
  
  	// ben alors y faut parcourir la DB
  	for(candindex=1; candindex<dbAllocationTable.KeyCount; candindex++){
    		eeAddr+=TUPLETSIZE; // index*(2*EEPAGESIZE)
    		ee.EepromRead(eeAddr,EEPAGESIZE,(uint8_t*)candidat);
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
	ee.EepromRead(eeAddr,size,value);
	return index;
}//db_get()

uint16_t db_getTuplet(uint16_t index, char* key, uint8_t* value, size_t* valueSize){
  uint16_t eeAddr=(index<<TUPLETSIZELOG);
  // renvoyer la clef
  ee.EepromRead(eeAddr,EEPAGESIZE,(uint8_t*)key);
  uint8_t dataSize=key[EEPAGESIZE-1];
  *valueSize=dataSize;
  // renvoyer la valeur
  eeAddr+=EEPAGESIZE;
  ee.EepromRead(eeAddr,*valueSize,value);
  return index;
}//db_getTuplet()



// db_del(key) efface le tuplet key de la DB.
// note : l'opération prend 10 ms car 3 écritures successsives en eeprom. Seule la dernière écriture est transparente.
uint16_t db_del(const char* key){
	// crush the deleted key with the top key in DB, then decrement keycount
	uint16_t index=db_find(key);
	if(dbAllocationTable.KeyCount<=1) return false; // no key to delete
	if(index==dbAllocationTable.KeyCount){
		// that key is the last key, no crush needed 
	}else{
    dbAllocationTable.KeyCount--; // pointe maintenant vers le tuplet à déplacer
		// crush deleted key with last alive key
    uint8_t  tmp[EEPAGESIZE];
		uint16_t eeDst=(index<<TUPLETSIZELOG);
		uint16_t eeSrc=(dbAllocationTable.KeyCount<<TUPLETSIZELOG);
		// copy key[KeyCount] to key[index]
		ee.EepromRead( eeSrc,EEPAGESIZE,tmp);
		ee.EepromWrite(eeDst,EEPAGESIZE,tmp);
		// copy value[KeyCount] to value[index]
		ee.EepromRead( eeSrc+EEPAGESIZE,EEPAGESIZE,tmp);
		ee.EepromWrite(eeDst+EEPAGESIZE,EEPAGESIZE,tmp);
	}// crush deleted key with last key 
	//------ mise à jour de la table d'allocation
	ee.EepromWrite(EEPAGESIZE,sizeof(dbAllocationTable),(uint8_t*)&dbAllocationTable);
  return true;
}//db_del()

//-------------rappels syntaxe
/*
int strcmp ( const char * str1, const char * str2 ); // 0=> egalité
int memcmp ( const void * ptr1, const void * ptr2, size_t num ); // 0=> egalité
void* memcpy( void* destination, const void* source, size_t count );
char* strcpy( char * destination, const char * source );
*/
