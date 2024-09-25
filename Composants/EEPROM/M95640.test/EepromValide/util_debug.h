#ifndef UTIL_DEBUG_INC
#define UTIL_DEBUG_INC
//---- Helper pour débogguer avec le nom de la fonction englobante. note: On commence par un '.' les affichages de debug.
#define debug(a)   if(DEBUG)cout << '.' << __func__  << "() " << #a << "=" << (a) << '\n'
//usage debug(myVar);
//usage debug(myVar1) << " autreVar" << autreVar << '\n' ;


//--- Affichage sur le PC avec la liaison USB -----------------
#define cout Serial2
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 
//usage: cout << "J'ai" << i << " bronzes de " << j << " kilo chacun.\n";

#endif

//---- Definir DEBUG false pour compiler sans affichage de debug
#ifndef DEBUG
#define DEBUG true
#endif


