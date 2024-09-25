#include <SPI.h>
#include <SdFat.h>

// Définir les broches pour le SPI3 (SPI3_MOSI, SPI3_MISO, SPI3_SCK)
#define CS_PIN  10 // Chip Select Pin pour la carte SD

// SPIClass pour SPI3 sur STM32F401RE
SPIClass SPI_3(PB15, PB14, PB13); // MOSI, MISO, SCK

// Instance de SdFat
SdFat sd;

// Configuration SPI pour SdFat
SdSpiConfig spiConfig(PB12, DEDICATED_SPI, SD_SCK_MHZ(50), &SPI_3);

// Déclaration des fonctions
void printDirectory(File &dir, int numTabs);
void writeCSV(const char* filename);
int32_t GetSdValuePC();
uint64_t getTotalCapacity();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Attendre que le port série soit prêt
  }

  Serial.print("Initialisation de la carte SD...");

  // Initialiser SPI3
  SPI_3.begin();

  // Initialiser la carte SD avec SPI3
  if (!sd.begin(spiConfig)) {
    Serial.println("Initialisation échouée!");
    return;
  }
  Serial.println("Initialisation réussie.");

  // Lister les fichiers sur la carte SD
  File root = sd.open("/");
  printDirectory(root, 0);

  // Créer et écrire dans un fichier CSV
  writeCSV("datalog.csv");

  // Calculer et afficher l'espace utilisé sur la carte SD
  int32_t usedSpace = GetSdValuePC();
  if (usedSpace >= 0) {
    Serial.print("Space used on SD card: ");
    Serial.print(usedSpace);
    Serial.println("%");
  } else {
    Serial.println("Error calculating space used on SD card.");
  }
}

void loop() {
  // Votre code ici
}

void printDirectory(File &dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) {
      // Pas d'autre fichier
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    // Utiliser getName pour obtenir le nom du fichier
    char name[100];
    entry.getName(name, sizeof(name));
    Serial.print(name);

    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // Afficher la taille du fichier
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void writeCSV(const char* filename) {
  // Ouvrir le fichier en écriture
  File file = sd.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Erreur lors de l'ouverture du fichier CSV pour écriture.");
    return;
  }

  // Écrire des en-têtes de colonnes dans le fichier CSV
  file.println("Colonne1,Colonne2,Colonne3");

  // Ajouter des données dans le fichier CSV
  for (int i = 0; i < 10; i++) {
    file.print(i);
    file.print(",");
    file.print(i * 2);
    file.print(",");
    file.println(i * 3);
  }

  // Fermer le fichier
  file.close();
  Serial.println("Données écrites dans le fichier CSV.");
}

int32_t GetSdValuePC() {
  double totalSize = 0;
  uint64_t totalCapacity = 15703441408; // Obtenir la capacité totale de la carte SD

  // Parcourir le répertoire racine et calculer la taille totale des fichiers
  File file = sd.open("/");
  if (!file) {
    Serial.println("Failed to open root directory");
    return -1;
  }

  File entry;
  while ((entry = file.openNextFile())) {
    if (!entry.isDirectory()) {
      totalSize += entry.size();
    }
    entry.close();
  }
  file.close();

  // Convertir la taille en pourcentage de la capacité totale
  return (int32_t)((totalSize / totalCapacity) * 100);
}

uint64_t getTotalCapacity() {
  uint64_t sectorCount = sd.card()->sectorCount();
  if (sectorCount == 0) {
    return 0; // Erreur de détermination de la taille de la carte
  }
  return sectorCount * 512; // Taille de la carte en octets (sectorCount est en secteurs de 512 octets)
}
