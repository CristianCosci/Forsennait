#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
void crea_mappa();
void gioca();
void termina_gioco();
enum Tipo_pericolo {
  nessun_pericolo,
  trappola,
	alieno
};

enum Tipo_oggetto {
  nessun_oggetto,
  medikit,
  pozione,
  materiale,
  colpi_lanciarazzi
};

enum Stato_giocatore {
  vulnerabile,
  scudo_vita,
  solo_vita,
  solo_scudo
};

struct Giocatore { //struttura che definisce le caratteristiche di ogni giocatore
	enum Stato_giocatore stato; //stato di vita del giocatore
  unsigned short int x,y; //posizione x e y sulla scacchiera (mappa), x=RIGHE ,y=COLONNE
	unsigned short int zaino[4]; //contiene gli oggetti di ogni giocatore
	unsigned short int alieni_uccisi; //serve a contare il numero di alieni uccisi
};

struct Cella { //struttura che definisce le caratteristiche di ogni cella della scacchiera
  enum Tipo_pericolo pericolo;
  enum Tipo_oggetto oggetto;
};

struct Piano{
  unsigned short int numero_piano; //indica il numero del Piano
	struct Piano *piano_successivo;
};

