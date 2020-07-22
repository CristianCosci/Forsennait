#include "gamelib.h"

int main(){
  short unsigned int sceltaMenu=0;//rappresenta la scelta sul menu che vuol fare il giocatore
  short unsigned int  controllo1=0;//primo controllo sul menu per i valori inseriti
  short unsigned int chiudi_gioco=0; //variabile che fa uscire dal ciclo del menu il gioco, usata in termina_gioco()
  printf("\033[1;31m");
  printf("INGRANDISCI LO SCHERMO...\n");
  printf("\033[0m");
  sleep(2);
  system("clear"); //serve a ripulire lo schermo in modo da rendere più chiara la visualizzazione del gioco
  printf("BENVENUTO IN:\n\n\n\n");
  printf("\033[1;36m");
  printf("_______  _______  _______  _______  _______  _        _        _______ __________________\n");
  printf("(  ____ \\(  ___  )(  ____ )(  ____ \\(  ____ \\( (    /|( (    /|(  ___  )\\__   __/\\__   __/\n");
  printf("| (    \\/| (   ) || (    )|| (    \\/| (    \\/|  \\  ( ||  \\  ( || (   ) |   ) (      ) (\n");
  printf("| (__    | |   | || (____)|| (_____ | (__    |   \\ | ||   \\ | || (___) |   | |      | |\n");
  printf("|  __)   | |   | ||     __)(_____  )|  __)   | (\\ \\) || (\\ \\) ||  ___  |   | |      | | \n");
  printf("| (      | |   | || (\\ (         ) || (      | | \\   || | \\   || (   ) |   | |      | | \n");
  printf("| )      | (___) || ) \\ \\__/\\____) || (____/\\| )  \\  || )  \\  || )   ( |___) (___   | | \n");
  printf("|/       (_______)|/   \\__/\\_______)(_______/|/    )_)|/    )_)|/     \\|\\_______/   )_( \n");
  sleep(2);
  printf("\n\n\n\n\n\n");
  printf("Questo è un gioco basato sullo stile di Fortnite, in cui i due giocatori(CICCIO e NINJA) si muovono liberamente sulla mappa cercando il loot per equipaggiarsi\n");
  printf("per quello che sarà lo scontro finale. Come nel vero fortnite(dopo aver creato la mappa), i due giocatori si lanceranno dal bus e dovranno\n");
  printf("atterrare in uno dei quattro angoli della mappa, a scelta dei giocatori. Per decidere chi si paracaduterà per primo verrà simulato un lancio di dadi.\n");
  sleep(12);
  printf("I due giocatori in seguito dovranno combattere e resistere a tutte le insiedie del gioco, quali le trappole(che uccidono immediatamente il giocatore)\n");
  printf("e gli alieni, che a volte si trovano a proteggere degli oggetti che potrebbero essere utili al giocatore.....\n");
  sleep(12);
  printf("Ogni tot di turni la mappa si restringerà quindi il consiglio è di affrettarsi il più possibile per raggiungere il centro della mappa.\n");
  printf("Se il giocatore si trova al di fuori dei bordi della mappa al suo restringimento, esso subirà danno e verrà riposizionato al bordo più vicino della mappa.\n");
  printf("Ovviamente le insidie rimarranno le stesse della vecchia mappa, quindi occhio alle trappole!!....\n");
  sleep(12);
  printf("Se in un qualunque momento i due giocatori si trovano nella stessa posizione della mappa, inizierà la battaglia finale.\n");
  printf("Ogni giocatore costruisce una torre di un numero di piani uguale al numero di materiale che ha nello zaino e si inizia a combattere.\n");
  printf("Il resto delle istruzioni per la battaglia finale verrà spiegato al momento in cui essa avrà inizio.\n\n\nBUONA FORTUNA!!\n\n\n");
  printf("attendere...caricamento del gioco in corso....\n");
  sleep(13);

  do {
    do {
      sceltaMenu=0;
      system("clear");
      printf("Menù di Gioco:\n" );
      printf("1)Crea Mappa\n2)Gioca\n3)Termina Gioco\n" );
      printf("\nScegli una delle 3 opzioni di gioco.\n");
      scanf("%hu",&sceltaMenu );
      if (sceltaMenu<1 || sceltaMenu>3) {
        printf("\033[1;31m");
        printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
        sleep(1);
        printf("\033[0m");
        system("clear");
      }
        else{
          controllo1=1;
          }
    } while(controllo1==0);
    switch (sceltaMenu) {
      case 1: system("clear");
              crea_mappa();
        break;
        case 2: system("clear");
                gioca();
        break;
      case 3: system("clear");
              termina_gioco();
              chiudi_gioco=1;
        break;
      default:
        break;
    }

  } while(chiudi_gioco==0);
  printf("Chiusura gioco in corso\n" );
  sleep(1);
  system("clear");
}
