#include "SPI.h"
#include "QuiriPinDefinition.h"
#include "db.h"
#include "util_debug.h"

// Programme de test et de démonstration du module DB.cpp
// avec exemples d'utilisation

// l'interface SPI doit exister avant toute utilisation
SPIClass IDB05A1_spi(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK);


void setup(){
  cout.begin(230400);
  for(int i=9;i>=0;i--,delay(4<<7)) cout << i << " " ;  // pour garder la main en cas de satellisation 
  cout << "\n.db.\n";
  if( !db_begin() ){ cout << "EEPROM non trouvée\n" ; while(true); }// et on bloque ici
  
  cout << "---contenu de la DB au RESET------------------------" ;
  AfficheDB();
  db_format();
  cout << "---contenu de la DB après formatage-----------------" ;
  AfficheDB();

  cout << "---dbset--- testArrayChar password shadok ----------";
  char testArrayChar[MAXSIZEOF]="Grande chaine de caractères";
  char password[MAXSIZEOF]="1234abcd";
  long shadok=6666666;
  dbset(testArrayChar);
  dbset(password);
  dbset(shadok);
  AfficheDB();

  cout << "---modif+dbset password ----------------------------";
  strcpy(password,"123456");
  dbset(password);
  AfficheDB();

  cout << "---dbset+modif+dbget d'une structure : MaStructure";
  struct UneStructure { long x=33; char c='0'; } MaStructure;
  dbset(MaStructure);
  MaStructure.x=22;
  dbget(MaStructure);
  if( MaStructure.x!=33 ) cout << "zut! \n";
  cout << "MaStructure={" << MaStructure.x << "," << MaStructure.c << "}";
  AfficheDB();

  cout << "---reset--------------------------------------------\n";
  password[0]=0;    // efface la chaine
  shadok=-1;        // efface le shadok 
  dbget(password);  // retrouve la chaine
  dbget(shadok);    // retrouve le shadok
  cout << "hippp " << password << " " << shadok << "\n\n";

  cout << "---dbdel de testArrayChar --------------------------";
  dbdel(testArrayChar);
  AfficheDB();

  cout << "---dbdel de shadok --------------------------";
  dbdel(shadok);
  AfficheDB();

  cout << "---dbreadint et dbreadstr (equivalents d'un dbget avec valeur par défaut)";
  int itest;
  char ctest[MAXSIZEOF];
  dbreadint(itest,-777);
  dbreadstr(ctest,"Hello world");
  cout << "\nitest=" << itest << " ctest=" << ctest ;
  AfficheDB();

  cout << "----QuiriUID----------------------------------------\n";
  AfficheQuiriUID();
}//setup()


void loop(){
  cout << ".";
  delay(10000);
}//loop()

void AfficheQuiriUID(){
  cout << "QuiriUID :";
  for(unsigned int i=0;i<sizeof(QuiriUID);i++){
    cout << QuiriUID[i] << ':';
  }
  cout << '\n';
}//AfficheQuiriUID()

