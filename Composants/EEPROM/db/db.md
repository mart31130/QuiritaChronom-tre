# L2023-QUIRITACHRONOMETRE
*********************************

# module db
Le composant db apporte une façon simple d'enregistrer des paramètres dans une eeprom.
usage :
   #include db.h
   puis dans le code :
   float MyFloatVar;
   if(!dbget(MyFloatVar)){ // regarde si une valeur a été récupérée
		MyFloatVar=12.03;   // on positionne la valeur par défaut
		dbset(MyFloatVar);  // on enregistre une valeur par défaut
   }
D'autres exemples sont documentés dans db.h
db.ino est une démonstration coomplète du composant.

Mise en oeuvre :
1) montez une carte NUCLEO IDB05A1 sur une STM32
2) passez l'exemple M95640.test\EepromValide
3) si ok passez l'exemple M95640.test\EepromRW
4) si ok passez la démonstration \db\db.ino et comparez le résultat avec db\db.log
5) si tout fonctionne ...
6) copiez les fichiers suivants dans votre projet :
   db.h
   db.cpp
   dbdriver.h
   M95640R.h
   M95640R.cpp
7) adaptez ou copiez les fichiers suivants dans votre projet :
   QuiriPinDefinition.h
   util_debug.h
8) dans votre fichier principal leproget.ino
   #include "SPI.h"
   #include "QuiriPinDefinition.h"
   #include "db.h"
   #include "util_debug.h"
   // l'interface SPI est un objet global qui doit exister avant toute utilisation de db
   SPIClass IDB05A1_spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);
   
   dans votre setup()
   while( !db_begin() ); // on bloque si db ne démarre pas


