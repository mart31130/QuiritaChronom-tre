# L2023-QUIRITACHRONOMETRE
*********************************
certains paramètres ne doit pas être codé en dur, mais enregistrés et retrouvés pendant l'init.
Par exemple, l'adresse MAC du quiri car elle doit être différente pour chaque dispositif.

La carte NUCLEO-IDB05A1 Bluetooth contient une eeprom M95640R. Cet eeprom est disponible, mais non utilisée dans la librairie bluetooth.

ici on trouve du code pour enregistrer des paramètres en EEPROM.
Versions eeprom i2c, eeprom SPI, eeprom embarquée dans le microcontrôleur (il n'y en a pas dans un STM32 mais il y en as dans les Arduino UNO et MEGA), et eeprom simulée en RAM pour le temps de mettre au point la solution.

M95640.test
  Mise en oeuvre et démo de la M95640

db.v1
  cette version v1 est basée sur un accès byte de db
  db : noyau de la base de données clef/valeur
  DBtest : db + émulation de l'eeprom spi M95640R
  DBtestMega : db + adaptation à l'eeprom interne d'un Arduino MEGA

M95640.doc
  Librairies, liens, et documentation de l'EEPROM embarquée dans la carte NUCLEO-IDB05A1 Bluetooth.
  
db
  cette version v2 pilote la M95640 de la carte NUCLEO   
  db.h : entête pour les clients de db.cpp
  db.cpp -> dbdriver.inc -> M95640R.h -> M95640R.cpp
  dbdriver.inc interface une eeprom quelconque avec db 