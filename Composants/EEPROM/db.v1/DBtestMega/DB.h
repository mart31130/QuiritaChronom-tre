/*------------- Database {key,value} ------------------

Ce module permet d'enregistrer des variables en eeprom et de les retrouver par la suite
il est destiné à enregistrer le paramètrage de l'application,
ainsi que tout ce qui doit garder sa valeur après une mise hors tension ou un RESET

EEPROM :
  Technologie généralement organisée en pages de 32 octets.
  L'eeprom comporte une RAM tampon de 32 octets qui ne sera écrit qu'après le transfert mpu vers tampon.
  cela économise la latence de 5 ms nécessaire à toute écriture en eeprom
=> si on écrit plus alors rollup (écrasement du début du tampon)
=> si on écrit à cheval sur 2 pages cela provoque 2 écritures (5ms de latence pour la seconde écriture)
Notes
	vous devez modifier la fonction eepromInit() dans DB.cpp si vous changez de type d'eeprom (ram,spi,i2c)
	vous devez modifier les constantes de dans DB.cpp si vous changez la taille de l'eeprom
------------------------------------------------------*/
#ifndef _DB_INC_
#define _DB_INC_

// taille maximale d'une variable en octets. NON MODIFIABLE car limite technologique.
// les tableaux de char devraient être dimensionnés à cette valeur.
#define MAXSIZEOF 32
// pour les tableaux de char cela limite à 31 caractères à cause du zéro terminal de chaine
// rappel : les nom des variables (c.a.d. les clefs) seront, eux, toujours tronqués à 30 caractères par la DB

//-------------- MACROS POUR LECTURE/ECRITURE en DB ----------------------------
// L'usage de ces "fonctions" qui sont en fait des macros est à privilégier car le code est plus facile à lire
// notez que ces macros ajoutent le & devant le nom de la variable qui est donc passée par référence

// lecture en eeprom
#define dbget(x)   db_get(#x,(uint8_t*)&x, sizeof(x))

// lecture en eeprom avec une valeur par défaut
#define dbreadint(x,y) if(dbfind(x)){dbget(x);}else{x=y;dbset(x);}
#define dbreadstr(x,y) if(dbfind(x)){dbget(x);}else{strcpy(x,y);dbset(x);}

// écriture en eeprom
#define dbset(x)   db_set(#x,(uint8_t*)&x, sizeof(x))

// détruire une clef
#define dbdel(x)   db_del(#x)

// tester l'existence d'une clef. renvoie true si existe.
#define dbfind(x)  db_find(#x)

// et enfin, juste pour la cohérence de la syntaxe
#define dbinit(x) db_init(x)

/* exemples :
	char MyTextVar[MaxSizeofData];

	// Mise à jour de la valeur
	strcpy(MyTextVar,"Hello world");
	dbset(MyTextVar); // la valeur sera mise à jour dans l'eeprom
	
	// chargement à la valeur enregistrée en eeprom
	dbget(MyVar); // si MyVar n'existe pas dans la DB, sa valeur ne sera pas modifiée.
	
	// Récupérer la valeur de MyFloatVar en eeprom
	float MyFloatVar;
	if(!dbget(MyFloatVar)){ // regarde si une valeur a été récupérée
		MyFloatVar=12.03;   // on positionne la valeur par défaut
		dbset(MyFloatVar);  // on enregistre une valeur par défaut
	}
	ou
	dbget(MyFloatVar,12.03);

	dbset(MyIntVar);
	dbset(MyFloatVar);
	....
	
*/
//---------------------------------------------------------
// Les macros développement vers les fonctions suivantes.
// !! donnez la préférence aux macros !!

// enregistre un tuplet {clef,valeur}, le crée s'il n'existe pas
// renvoie l'index du tuplet en cas de succès, false si une erreur s'est produite
uint16_t db_set(const char* key, uint8_t* value, size_t size);

// relit un tuplet {clef,valeur}
// renvoie l'index du tuplet en cas de succès, false si une erreur s'est produite
uint16_t db_get(const char* key, uint8_t* value, size_t size);
uint16_t db_get(uint16_t index,  uint8_t* value, size_t size);

// efface un tuplet de la base de données. 
// renvoie true en cas de succès
uint16_t db_del(const char* key);

// cherche un tuplet de la base de données. 
// renvoie l'index du tuplet en cas de succès, false si une erreur s'est produite
uint16_t db_find(const char* key);

//---------------------------------------------------------
// initialisation de la base de données
uint16_t db_init(int dummyArg); // pour l'instant l'argument ne sert pas.

//---------------------------------------------------------
// fonctions d'informations et de listage des valeurs
// !! pas de macro pour ces fonctions destinées à l'application technichien !!
uint16_t db_info(uint32_t* version, uint16_t* maxKey, uint16_t* keyCount, uint32_t* mpuuid);

// copie le tuplet[index] vers deux tampons. 
// Les tampons name et value doivent avoir une taille de EEPAGESIZE octets chacun
// renvoie false si l'index n'existe pas
// les tuplets sont enregistrés de 1 à keyCount.
uint16_t db_getTuplet(uint16_t index, char* key, uint8_t* value, size_t* valueSize);

//--------------------------------------------------------
// Identifiant MAC du dispositif
// les 4 premiers octets viendrons du mcuUID au moment de l'init
// la variable est en lecture seule
extern uint8_t QuiriUID[6];
#endif
