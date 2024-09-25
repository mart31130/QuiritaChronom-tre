#include "CEeprom.h"

CEeprom::CEeprom() {
}
CEeprom::~CEeprom() {}

void CEeprom::AfficheQuiriUID(){
  cout << "QuiriUID :";
  for(int i=0;i<sizeof(QuiriUID);i++){
    cout << QuiriUID[i] << ':';
  }
  cout << '\n';
}//AfficheQuiriUID()

void CEeprom::AfficheDB(){
  	uint32_t dbversion,mpuuid;
  	uint16_t maxKey,keyCount,index;
  	index=this->db.db_info(&dbversion, &maxKey, &keyCount, &mpuuid);
  	if(!index) return;
  	cout << "\nDB nversion=" << (dbversion>>8) << "." << (dbversion&0xFF) << " maxKey=" << maxKey << " keyCount=" << keyCount << " mcuiuid=" << mpuuid << '\n';
    //---- pour toutes les clef
  	for(uint16_t i=0; i<keyCount; i++){
    		char clef[MAXSIZEOF]; uint8_t valeur[MAXSIZEOF]; char ascii[MAXSIZEOF+1]; size_t valueSize;
    		this->db.db_getTuplet(i,clef,valeur,&valueSize);
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

/*bool CEeprom::Event(EV event, int32_t param ){
  switch(event){
    case EV::ColdReset:
      //this->initBLE();
      break;
    case EV::ShortPress:
      cout.println("Case ShortPress");

      break;
  }
  return ContinuerEVDistribution;
}*/
