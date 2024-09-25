 #include "DB.h"
#include "util_debug.h"

// Programme de test du module DB.cpp
// avec exemples d'utilisation


void setup(){
  cout.begin(230400);
  delay(5000); // pour garder la main satellisation 
  cout << "\n";

  db_init(1);

  cout << "---dbset--- testArrayChar password shadok ----------\n";
  char testArrayChar[MAXSIZEOF]="Chaine de caractères";
  char password[MAXSIZEOF]="1234";
  long shadok=6666666;
  dbset(testArrayChar);
  dbset(password);
  dbset(shadok);
  AfficheDB();
  
  cout << "---modif+dbset password ----------------------------\n";
  strcpy(password,"123456");
  dbset(password);
  AfficheDB();
  
  cout << "---dbset+modif+dbget d'une structure : MaStructure\n";
  struct UneStructure {
    long x = 1;
    char c = '0';
  } MaStructure;
  dbset(MaStructure);
  MaStructure.x=22;
  dbget(MaStructure);
  if(MaStructure.x != 1) cout << "zut! \n";
  cout << "MaStructure={" << MaStructure.x << "," << MaStructure.c << "}\n\n";

  
  cout << "---dbreadint dbreadstr (equivalents d'un dbget avec valeur par défaut)\n";
  int itest;
  char ctest[MAXSIZEOF];
  dbreadint(itest,-777);
  dbreadstr(ctest,"Hello world");
  cout << "itest=" << itest << " ctest=" << ctest << '\n';
  AfficheDB();
  
  cout << "---reset--------------------------------------------\n";
  password[0]=0;    // efface la chaine
  shadok=-1;        // efface le shadok 
  dbget(password);  // retrouve la chaine
  dbget(shadok);    // retrouve le shadok
  cout << "hippp " << password << " " << shadok << "\n\n";


  cout << "---dbdel de testArrayChar --------------------------\n";
  dbdel(testArrayChar);
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
  for(int i=0;i<sizeof(QuiriUID);i++){
    cout << QuiriUID[i] << ':';
  }
  cout << '\n';
}//AfficheQuiriUID()

void AfficheDB(){
  	uint32_t dbversion,mpuuid;
  	uint16_t maxKey,keyCount,index;
  	index=db_info(&dbversion, &maxKey, &keyCount, &mpuuid);
  	if(!index) return;
  	cout << "\nDB nversion=" << (dbversion>>8) << "." << (dbversion&0xFF) << " maxKey=" << maxKey << " keyCount=" << keyCount << " mcuiuid=" << mpuuid << '\n';
    //---- pour toutes les clef
  	for(uint16_t i=0; i<keyCount; i++){
    		char clef[MAXSIZEOF]; uint8_t valeur[MAXSIZEOF]; char ascii[MAXSIZEOF+1]; size_t valueSize;
    		db_getTuplet(i,clef,valeur,&valueSize);
       //---- affiche le nom et la taille
        if(i<9) cout << "0";
    		cout << i << " " << clef << "[" << valueSize << "] ";
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
        cout << " " << ascii << '\n';
     }//for toutes les clef
     cout << "\n";
}//AfficheDB
