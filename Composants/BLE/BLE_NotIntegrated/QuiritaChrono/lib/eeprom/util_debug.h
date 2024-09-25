#ifndef UTIL_DEBUG_INC
#define UTIL_DEBUG_INC
//---- Helper pour débogguer avec le nom de la fonction englobante
#define Debug if(DEBUG)cout << __func__ << ' '
//usage Debug << "data=" << data << '\n' ;
#define show(a) Debug << #a << "=" << (a) << '\n'


//--- Affichage sur le PC avec la liaison USB -----------------
#define cout Serial2
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 
//usage: cout << "J'ai" << i << " bronzes de " << j << " kilo chacun.\n";


//---- BEGIN MOFIFIABLE BY USER 
#define DEBUG true

#endif
