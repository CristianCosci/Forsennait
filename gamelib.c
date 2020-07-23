/*NOTA: i turni dei due giocatori, sia nel corso della partita normale che nella battaglia finale,
non sono stati divisi mediante un'unica funzione a cui viene passato come parametro il giocatore
che sta eseguendo il turno. Questo perchè rende più di facile comprensione la divisione dei TURNI
e cosa ogni particolare controllo e ciclo svolge alle caratteristiche di ogni giocatore. Non
è stata usata una funzione anche per non rendere poco decifrabile il gioco non implementanto esageratamente
il numero di funzioni e 'spezzettare' troppo il codice. Infine non sono state usate delle funzioni per il turno
anche perche comunque questo tipo di utilizzo dei valori e delle variabili non potrebbe essere usato o
ri-usato in un altro gioco, per la particolarità di tale codice. Chiedo quindi scusa per l'elevata lunghezza
del codice relativo a questo programma.
Per il resto spero che il gioco venga apprezzato e considerato divertente.
Ho cercato di renderlo il più "giocabile" possibile e simile ad un vero gioco, secondo le mie conoscenze.*/


#include "gamelib.h"
static struct Giocatore Ninja = {solo_vita, .alieni_uccisi=0};//inserire statistiche dei due giocatori
static struct Giocatore Ciccio ={solo_vita, .alieni_uccisi=0};
static struct Cella *scacchiera=NULL; //puntatore a struct cella inizializzato a NULL in quanto non se ne conosce ancora la dimensione

//variabili globali utilizzate da più funzioni per una migliore gestione delle opzioni di gioco
unsigned short int mappa_creata=0; //permette di controllare se la mappa è stata creata oppure no
unsigned short int giocatori_posizionati=0; //permette di giocare solo se i giocatori sono stati posizionati
unsigned short int giocatore_morto=0; //indica se uno dei due giocatori è morto
unsigned short int vincitore=0; //indica il giocatore che vince. Se è uguale a 1 è Ninja, se è uguale a 2 è Ciccio
unsigned short int livello_difficolta=0;//indica il livello di difficoltà della partita
unsigned short int turno_passato=0; //controlla se la mossa è stata eseguita dal giocatore
unsigned short int scontro_finale=0; //se i due giocatori sono nella stessa cella si passa allo scontro finale
unsigned short int risultato_morra=0; //se è uguale a 1 vince chi sceglie di giocare altrimenti vince il giocatore che è stato sfidato
unsigned short int contatore_turni=0; //permette di contare il numero di turni trascorsi
static unsigned short int n=0; //variabile globale nella quale viene memorizzata la grandezza della scacchiera

//funzioni richiamate in crea mappa
static void crea_scacchiera();
static void stampa_scacchiera();
static void torna_indietro();//richiamata anche in gioca

//funzioni richiamate in gioca
static void posizionamento_giocatori();
static void inizia_partita();
static void stampa_mappa_e_giocatori(struct Giocatore giocatore1, struct Giocatore giocatore2);
//funzioni che permettono il movimento
static void muovi_su(struct Giocatore *giocatore);
static void muovi_giu(struct Giocatore *giocatore);
static void muovi_dx(struct Giocatore *giocatore);
static void muovi_sx(struct Giocatore *giocatore);
//funzione che permette di interagire con i pericoli e gli oggetti trovati nella mappa
static void interagire(struct Giocatore *giocatore);
//funzione che mostra lo stato del giocatore, nel caso volesse usare degli oggetti presenti nello zaino
static void stampa_statistiche(struct Giocatore *giocatore, unsigned short int nome);
//funzione che dimezza la mappa dopo un numero di turni uguale alla grandezza del lato della mappa stessa e riposiziona i giocatori
static void riduci_mappa(struct Giocatore *giocatore1, struct Giocatore *giocatore2);
//funzione dello scontro finale tra i due giocatori
static void battaglia_finale();
//funzione usata nello scontro finale
static void morra_cinese(unsigned short int nome);

//funzione richiamata dal main
void crea_mappa(){
  srand((unsigned)time(NULL));//permette di fornire un seme ogni volta diverso alla funzione random
  unsigned short int sceltaMenu2=0;//scelta di cosa vuole fare l'utente una volta dentro il crea mappa
  unsigned short int sceltaMenu3=0;//scelta della difficoltà di gioco
  unsigned short int controllo2=0; //controllo della scelta nel menu
  unsigned short int controllo3=0; //controllo della scelta del livello di difficoltà
  unsigned short int torna_al_menu=0;//permettere di chiudere il menu

  while (torna_al_menu==0) {
      do {
        printf("SCEGLI COSA VUOI FARE:\n1 CREA MAPPA\n2 STAMPA MAPPA\n3 TORNA AL MENU DI GIOCO\n" );
        scanf("%hu",&sceltaMenu2 );
        if (sceltaMenu2<1 || sceltaMenu2>3) {
          printf("Inserisci un valore corretto.\n" );
          sleep(1);
          system("clear");
            }
          else{
              controllo2=1;
                }
              } while(controllo2==0);

        switch (sceltaMenu2) {
          case 1: if (mappa_creata==1) {
            printf("Hai già creato una mappa\n" );
            sleep(2);
            system("clear");
              }
            else{
              printf("\n\n\n\n");
              /*permette di scegliere la difficoltà di gioco,
              per una giocabilità migliore si consiglia la modalità facile in modo
             da poter stampare la mappa ogni volta che si vuole nel corso del gioco*/
              do {
                printf("SCEGLI LA DIFFICOLTA' DI GIOCO:\n1: FACILE(puoi stampare la mappa prima e dopo aver posizionato i giocatori ogni volta che vuoi, consumando un turno)\n");
                printf("2: DIFFICILE(non puoi stampare la mappa dopo aver posizionato i giocatori e durante la partita. Mappa con più pericoli e meno loot)\n\n\n\n" );
                scanf("%hu",&sceltaMenu3 );
                if (sceltaMenu3<1 || sceltaMenu3>2) {
                  printf("Inserisci un valore corretto.\n" );
                  sleep(1);
                  system("clear");
                    }
                  else{
                      controllo3=1;
                      if (sceltaMenu3==1) {
                        livello_difficolta=0; //modalità facile
                        }
                        else{
                          if (sceltaMenu3==2) {
                            livello_difficolta=1; //modalità difficile
                          }
                        }
                      }
                  } while(controllo3==0);

              crea_scacchiera();
              sleep(2);
              system("clear");
                }
          break;
        case 2: if (mappa_creata==1) {
                  printf("Ora puoi stampare la mappa\n");
                  sleep(2);
                  system("clear");
                  stampa_scacchiera();
                    }
                  else{
                      if (mappa_creata!=1) {
                        printf("\033[1;31m");
                        printf("Prima di poter stampare la mappa ne devi creare una\n");
                        printf("\033[0m");
                        sleep(2);
                        system("clear");
                          }
                        }
                  printf("\033[1;36m");
          break;
        case 3: torna_al_menu=1;
                torna_indietro();
          break;
        default:
          break;
    }
  }
}

//funzione richiamata dal main
void gioca(){
  unsigned short int sceltaMenuGioca=0;//rappresenta la scelta sul menu che vuol fare il giocatore
  srand((unsigned)time(NULL));//permette di fornire un seme ogni volta diverso alla funzione random
  unsigned short int controlloGioca=0;//controllo sul meno che si apre quando si sceglie di giocare
  unsigned short int torna_al_menu1=0;//permettere di chiudere il menu
  if(mappa_creata==0){ //solo se la mappa è stata creata si può accedere al menù di gioca()
      printf("\033[1;31m");
      printf("\nNESSUNA MAPPA CREATA, CREANE UNA PER GIOCARE\n");
      sleep(1);
      printf("\033[1;36m");
      system("clear");
  }
  else{
    if (mappa_creata==1) {
      while (torna_al_menu1==0) {
        do {
          system("clear");
          printf("Menù opzioni per GIOCARE:\n" );
          printf("1)Posiziona Giocatori\n2)Stampa mappa e giocatori\n3)Inizia Partita\n4)Torna al menu di gioco\n" );
          printf("\nScegli una delle 3 opzioni di gioco.\n");
          scanf("%hu",&sceltaMenuGioca );
          if (sceltaMenuGioca<1 || sceltaMenuGioca>4) {
            printf("\033[1;31m");
            printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
            sleep(1);
            printf("\033[0m");
            system("clear");
          }
            else{
              controlloGioca=1;
              }
        } while(controlloGioca==0);
        switch (sceltaMenuGioca) {
          case 1: if (giocatori_posizionati==1) {
                  printf("\033[1;31m");
                  printf("Hai già posizionato i giocatori.\n" );
                  sleep(1);
                  printf("\033[0m");
                  system("clear");
                  }
                  else{
                    if (giocatori_posizionati==0) {
                      sleep(1);
                      system("clear");
                      posizionamento_giocatori();
                      giocatori_posizionati=1;
                      }
                    }
            break;
            case 2: system("clear");
                    if (livello_difficolta==0 && giocatori_posizionati==1) {
                    printf("Avendo scelto la modalità facile, potrai stampare i giocatori\n" );
                    sleep(2);
                    stampa_mappa_e_giocatori(Ciccio, Ninja);
                      }
                      else{
                        if (livello_difficolta!=0 || giocatori_posizionati!=1) {
                          printf("\033[1;31m");
                          printf("Non hai scelto la modalità facile. Oppure non hai posizionato i giocatori.\n" );
                          printf("Non puoi quindi usare questa funzione.\n" );
                          sleep(2);
                          printf("\033[0m");
                          system("clear");
                        }
                      }
              break;
            case 3: system("clear");
                    if (giocatori_posizionati==0) { //solo se i giocatori sono stati posizionati si può iniziare la partita
                      printf("\033[1;31m");
                      printf("Prima di poter cominciare una partita devi posizionare i giocatori sulla mappa.\n" );
                      sleep(2);
                      printf("\033[0m");
                      system("clear");
                      }
                      else{
                        if (giocatori_posizionati==1) {
                          sleep(1);
                          inizia_partita();
                        }
                        torna_al_menu1=1;
                        torna_indietro();
                      }

            break;
          case 4: system("clear");
                  torna_al_menu1=1;
                  torna_indietro();

            break;
          default:
            break;
          }
        }
      }
    }
}

void termina_gioco(){
  printf("\n");
}

//funzione che chiede in input la grandezza n della scacchiera e alloca una memoria grande n celle (scacchiera n x n di celle)
static void crea_scacchiera(){
  printf("\033[1;36m");
  unsigned short int controllo_grandezza_mappa=0;
  do {
    printf("INSERISCI LA GRANDEZZA DELLA MAPPA.\nNB: per un'esperienza di gioco ottimale si consiglia una grandezza di 10\n" );
    scanf("%hu",&n );
    printf("\n\n\n");
    if (n<=1 || n>25) { //controllo che la mappa non sia troppo piccola o di grandezze troppo grandi per renderla giocabile in poco tempo
      controllo_grandezza_mappa=0;
      printf("\033[1;31m");
      printf("Inserisci una dimensione accettabile, non troppo grande ma nemmeno troppo piccola...\n");
      sleep(1);
      printf("\033[1;36m");
      system("clear");
    }
    else{
      if (n>1 && n<25) {
        controllo_grandezza_mappa=1;
      }
    }
  } while(controllo_grandezza_mappa==0);
  unsigned short int casuale=0; //variabile che cambia ogni volta con il random nel popolamento
  unsigned int rows=n;
  unsigned int cols=n;
  scacchiera=(struct Cella *)malloc(rows*cols*sizeof(struct Cella));
  if (livello_difficolta==0) { //popolamento per modalità facile, offre migliore giocabilità
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
          casuale=rand() % 100;//assegno un volore random, da qui scelgo con cosa popolare le celle della scacchiera

              //popolamento del pericolo nelle celle
              if (casuale>=0 && casuale<60) {
                scacchiera[i*cols+j].pericolo=nessun_pericolo;
              }
              else{
                if (casuale>=60 && casuale<70) {
                  scacchiera[i*cols+j].pericolo=trappola;
                }
                else{
                  if (casuale>=70 && casuale<100) {
                    scacchiera[i*cols+j].pericolo=alieno;
                      }
                    }
                  }
            casuale=rand() % 100; //riassegno un valore random per far cambiare le combinazioni dei pericoli con gli oggetti
              //popolamento degli oggetti nelle celle
              if (casuale>=0 && casuale<35) {
                scacchiera[i*cols+j].oggetto=nessun_oggetto;
                  }
                  else{
                    if (casuale>=35 && casuale<55) {
                      scacchiera[i*cols+j].oggetto=medikit;
                    }
                    else{
                      if (casuale>=55 && casuale<75) {
                        scacchiera[i*cols+j].oggetto=pozione;
                          }
                          else{
                            if (casuale>=75 && casuale<90) {
                              scacchiera[i*cols+j].oggetto=materiale;
                            }
                            else{
                              if (casuale>=90 && casuale<100) {
                                scacchiera[i*cols+j].oggetto=colpi_lanciarazzi;
                              }
                            }
                          }
                        }
                      }

          }
        }
  }
  else{ //popolamento con probabilità differenti per la modalità difficile
    if (livello_difficolta==1) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            casuale=rand() % 100;//assegno un volore random, da qui scelgo con cosa popolare le celle della scacchiera

                //popolamento del pericolo nelle celle
                if (casuale>=0 && casuale<40) {
                  scacchiera[i*cols+j].pericolo=nessun_pericolo;
                }
                else{
                  if (casuale>=40 && casuale<60) {
                    scacchiera[i*cols+j].pericolo=trappola;
                  }
                  else{
                    if (casuale>=60 && casuale<100) {
                      scacchiera[i*cols+j].pericolo=alieno;
                        }
                      }
                    }
              casuale=rand() % 100; //riassegno un valore random per far cambiare le combinazioni dei pericoli con gli oggetti
                //popolamento degli oggetti nelle celle
                if (casuale>=0 && casuale<60) {
                  scacchiera[i*cols+j].oggetto=nessun_oggetto;
                    }
                    else{
                      if (casuale>=60 && casuale<70) {
                        scacchiera[i*cols+j].oggetto=medikit;
                      }
                      else{
                        if (casuale>=70 && casuale<80) {
                          scacchiera[i*cols+j].oggetto=pozione;
                            }
                            else{
                              if (casuale>=80 && casuale<90) {
                                scacchiera[i*cols+j].oggetto=materiale;
                              }
                              else{
                                if (casuale>=90 && casuale<100) {
                                  scacchiera[i*cols+j].oggetto=colpi_lanciarazzi;
                                }
                              }
                            }
                          }
                        }

            }
          }
    }
  }
  controllo_grandezza_mappa=0;
  mappa_creata=1;//indica che la mappa è stata creata
  printf("\033[1;36m");
  printf("\tMAPPA CREATA CORRETTAMENTE\n");
}

//funzione che permette di stampare la mappa
static void stampa_scacchiera(){
  unsigned int rows=n;
  unsigned int cols=n;
  for (int i = 0; i <rows ; i++) {
    for (int j = 0; j <cols; j++) {
      printf(" ||");

      //stampa i pericoli
      switch (scacchiera[i*cols+j].pericolo) {
        case 0: printf("0");
          break;
        case 1: printf("T");
          break;
        case 2: printf("A");
          break;
        default:
          break;
        }
        printf(" - ");
        //stampa gli oggetti
        switch (scacchiera[i*cols+j].oggetto) {
          case 0: printf("X" );
            break;
          case 1: printf("C");
            break;
          case 2: printf("P");
            break;
          case 3:printf("M");
            break;
          case 4:printf("L");
            break;
          default:
            break;
          }
      }
      printf("||\n\n");
    }
    printf("\n\n\n\n");
    printf("\033[1;31m");
    printf("%60s\n","LEGENDA:" );
    printf("A=Alieno   T=Trappola    C=Medikit    P=Pozione    M=Materiale   L=Colpo Lanciarazzi     " );
    printf("0=Nessun Pericolo     X=Nessun Oggetto\n");
    printf("\033[1;36m");
  sleep(15);
  system("clear");
}

//funzione che permette di ritornare al menu di gioco, uscendo cosi dalla modalità di creazione della mappa
static void torna_indietro(){
  system("clear");
}

//posiziona i giocatori nella posizione di paertenza prima di iniziare la partita
static void posizionamento_giocatori(){
  unsigned short int dado=0;//variabile a cui verrà assegnato il lancio del dado
  unsigned short int scelta_atterraggio1=0;//legge la scelta di dove atterrare nella mappa(DESTRA O SINISTRA) per il primo a scegliere
  unsigned short int scelta_atterraggio2=0;//legge la scelta di dove atterrare nella mappa(SOPRA O SOTTO) per il primo a scegliere
  unsigned short int scelta_atterraggio3=0; //legge la scelta di dove atterrare nella mappa(DESTRA O SINISTRA) per il secondo a scegliere
  unsigned short int scelta_atterraggio4=0;//legge la scelta di dove atterrare nella mappa(SOPRA O SOTTO) per il secondo a scegliere
  unsigned short int controllo1=0; //controllo dei valori inseriti dall'utente per il primo atterraggio
  unsigned short int controllo2=0; //controllo dei valori inseriti dall'utente per il primo atterraggio
  unsigned short int controllo3=0; //controllo dei valori inseriti dall'utente per il secondo atterraggio
  unsigned short int controllo4=0; //controllo dei valori inseriti dall'utente per il secondo atterraggio
  unsigned short int Ninja_posizionato=0;//indica che Ninja non è stato ancora posizionato
  unsigned short int Ciccio_posizionato=0;//indica che Ciccio non è stato ancora posizionato
  printf("PREPARATI ALL'ATTERRAGGIO...\n\n\n\n" );
  printf("\033[1;34m");
  printf("           ______\n");
  printf("           _\\ _~-\\___\n");
  printf("   =  = ==(____AA____D\n");
  printf("               \\_____\\___________________,-~~~~~~~`-.._\n");
  printf("                /     o O o o o o O O o o o o o o O o  |\\_\n");
  printf("                `~-.__        ___..----..                  )\n");
  printf("                      `---~~\\___________/------------`````\n");
  printf("                     =  ===(_________D\n\n");
  printf("          _  _\n");
  printf("         ( `   )_\n");
  printf("        (    )    `)\n");
  printf("      (_   (_ .  _) _)\n");
  printf("                                     _\n");
  printf("                                    (  )\n");
  printf("     _ .                         ( `  ) . )\n");
  printf("   (  _ )_                      (_, _(  ,_)_)\n");
  printf(" (_  _(_ ,)\n");
  printf("\033[0m");
  printf("\n\n\n\n\n");
  printf("Verrà simulato un lancio di dadi e in base a quale numero uscirà");
  printf(", Ciccio o Ninja potranno scegliere per primi in che luogo atterrare.....\n");
  sleep(6);
  printf("\nSE ESCE UN NUMERO TRA 1 E 3: Ciccio sarà il primo a scegliere.\nSE ESCE UN NUMERO TRA 4 E 6: Ninja sceglie per primo.\n\n\n\n" );
  sleep(5);
  system("clear");
  dado=1+(rand() %6);//simula il lancio dei dadi, assegnando alla variabile un numero tra 1 e 6
  printf("è uscito il numero %hu \n\n\n\n",dado );
  sleep(3);
  if (dado>=1 && dado<4) { //posiziona il primo giocatore nella mappa in base all'esito dei dadi
      printf("CICCIO SCEGLIE PER PRIMO\n" );
      sleep(3);
      system("clear");
      do { //scelta della zona in cui si vuole atterrare
        printf("VUOI ATTERRARE NELLA PARTE DESTRA O SINISTRA DELLA MAPPA?\n");
        printf("1:DESTRA\n2:SINISTRA\n\n");
        scanf("%hu",&scelta_atterraggio1);
        if (scelta_atterraggio1<1 || scelta_atterraggio1>2) {
          printf("Inserisci un valore corretto.\n" );
          sleep(1);
          system("clear");
            }
          else{
              controllo1=1;
                }
        } while(controllo1==0);
      if (scelta_atterraggio1==1) {
          Ciccio.x=n-1;
        }
        else{
          if (scelta_atterraggio1==2) {
          Ciccio.x=0;
          }
        }
      do { //scelta zona in cui si vuole atterrare
        printf("VUOI ATTERRARE NELLA PARTE SUPERIORE O INFERIORE DELLA MAPPA?\n");
        printf("1:SOPRA\n2:SOTTO\n\n");
        scanf("%hu",&scelta_atterraggio2);
        if (scelta_atterraggio2<1 || scelta_atterraggio2>2) {
          printf("Inserisci un valore corretto.\n" );
          sleep(1);
          system("clear");
            }
          else{
              controllo2=1;
                }
      } while(controllo2==0);
      if (scelta_atterraggio2==1) {
          Ciccio.y=0;
        }
        else{
          if (scelta_atterraggio2==2) {
          Ciccio.y=n-1;
          }
        }

    }
    else{
      if (dado>=4 && dado<7) {
        printf("NINJA SCEGLIE PER PRIMO\n" );
        sleep(3);
        system("clear");
        do {//scelta zona in cui si vuole atterrare
          printf("VUOI ATTERRARE NELLA PARTE DESTRA O SINISTRA DELLA MAPPA?\n");
          printf("1:DESTRA\n2:SINISTRA\n\n");
          scanf("%hu",&scelta_atterraggio1);
          if (scelta_atterraggio1<1 || scelta_atterraggio1>2) {
            printf("Inserisci un valore corretto.\n" );
            sleep(1);
            system("clear");
              }
            else{
                controllo1=1;
                  }
          } while(controllo1==0);
        if (scelta_atterraggio1==1) {
            Ninja.x=n-1;
          }
          else{
            if (scelta_atterraggio1==2) {
              Ninja.x=0;
            }
          }
        do {//scelta zona in cui si vuole atterrare
          printf("VUOI ATTERRARE NELLA PARTE SUPERIORE O INFERIORE DELLA MAPPA?\n");
          printf("1:SOPRA\n2:SOTTO\n\n");
          scanf("%hu",&scelta_atterraggio2);
          if (scelta_atterraggio2<1 || scelta_atterraggio2>2) {
            printf("Inserisci un valore corretto.\n" );
            sleep(1);
            system("clear");
              }
            else{
                controllo2=1;
                  }
        } while(controllo2==0);
        if (scelta_atterraggio2==1) {
            Ninja.y=0;
          }
          else{
            if (scelta_atterraggio2==2) {
              Ninja.y=n-1;
            }
          }
      }
    }
  sleep(1);
  system("clear");
  if (dado>=1 && dado<4) {
    printf("Ciccio è atterrato in posizione |%hu|-|%hu|\n",Ciccio.x, Ciccio.y );
    Ciccio_posizionato=1;//indica che ciccio è stato posizionato
    }
    else{
      if (dado>=4 && dado<7) {
        printf("Ninja è atterrato in posizione |%hu|-|%hu|\n",Ninja.x, Ninja.y );
        Ninja_posizionato=1;//indica che il ninja è stato posizionato
      }
    }
  printf("posizionamento primo giocatore completa\n\n" );
  sleep(3);
  system("clear");
  printf("ORA INIZIA L'ATTERRAGGIO SULLA MAPPA DEL SECONDO GIOCATORE\n" );
  sleep(2);
  system("clear");
  if (Ciccio_posizionato==1) {//se Ciccio è stato posizionato ora si posiziona Ninja
    do {//scelta zona in cui si vuole atterrare
      printf("VUOI ATTERRARE NELLA PARTE DESTRA O SINISTRA DELLA MAPPA?\n");
      printf("1:DESTRA\n2:SINISTRA\n\n");
      scanf("%hu",&scelta_atterraggio3);
      if (scelta_atterraggio3<1 || scelta_atterraggio3>2) {
        printf("Inserisci un valore corretto.\n" );
        sleep(1);
        system("clear");
          }
        else{
            controllo3=1;
              }
      } while(controllo3==0);
      if (scelta_atterraggio3==1) {
          Ninja.x=n-1;
        }
        else{
          if (scelta_atterraggio3==2) {
            Ninja.x=0;
          }
        }

      do {//scelta zona in cui si vuole atterrare
        printf("VUOI ATTERRARE NELLA PARTE SUPERIORE O INFERIORE DELLA MAPPA?\n");
        printf("1:SOPRA\n2:SOTTO\n\n");
        scanf("%hu",&scelta_atterraggio4);
        if (scelta_atterraggio4<1 || scelta_atterraggio4>2) {
          printf("Inserisci un valore corretto.\n" );
          sleep(1);
          system("clear");
            }
          else{
              controllo4=1;
                }
      } while(controllo4==0);
      /*nel caso in cui la zona scelta dai giocatori sia la stessa, il secondo a scegliere viene
      viene posizionato nelle riga opposta a quella del primo giocatore*/
      if (scelta_atterraggio2==scelta_atterraggio4 && scelta_atterraggio1==scelta_atterraggio3) {
        printf("\033[1;34m");
        printf("OH NO!\nNella stessa posizione in cui vuoi atterrare tu ci si è già fiondato Ciccio.\n");
        printf("Ti ha già rubato tutto il loot, non è saggio cercare di UCCIDERLO SOLO CON UN PICCONE, LUI HA LO SCAR!!...\n");
        sleep(3);
        printf("Ninja devia così la sua traiettoria di atterraggio verso la direzione opposta in cui stava andando.\n");
        printf("CI BECCHEREMO DOPO CICCIO!\nNON PENSARE DI FARLA FRANCA!!\n");
        printf("\033[0m");
        sleep(3);
        if (scelta_atterraggio4==1) {
          Ninja.y=n-1;
          }
          else{
            if (scelta_atterraggio4==2) {
              Ninja.y=0;
              }
            }
          }
          else{
            if (scelta_atterraggio2!=scelta_atterraggio4) {
              printf("\033[1;34m");
              printf("Non vedo nessuno nelle vicinanze. Sarà un atterraggio tranquillo.\n");
              printf("\033[0m");
              sleep(2);
              if (scelta_atterraggio4==1) {
                  Ninja.y=0;
                }
                else{
                  if (scelta_atterraggio4==2) {
                    Ninja.y=n-1;
                  }
                }
            }
          }


    }
    else{
      if (Ninja_posizionato==1) {//se Ninja è stato posizionato ora si posiziona Ciccio
        do {//scelta zona in cui si vuole atterrare
          printf("VUOI ATTERRARE NELLA PARTE DESTRA O SINISTRA DELLA MAPPA?\n");
          printf("1:DESTRA\n2:SINISTRA\n\n");
          scanf("%hu",&scelta_atterraggio3);
          if (scelta_atterraggio3<1 || scelta_atterraggio3>2) {
            printf("Inserisci un valore corretto.\n" );
            sleep(1);
            system("clear");
              }
            else{
                controllo3=1;
                  }
          } while(controllo3==0);
          if (scelta_atterraggio3==1) {
              Ciccio.x=n-1;
            }
            else{
              if (scelta_atterraggio3==2) {
                Ciccio.x=0;
              }
            }
          do {//scelta zona in cui si vuole atterrare
            printf("VUOI ATTERRARE NELLA PARTE SUPERIORE O INFERIORE DELLA MAPPA?\n");
            printf("1:SOPRA\n2:SOTTO\n\n");
            scanf("%hu",&scelta_atterraggio4);
            if (scelta_atterraggio4<1 || scelta_atterraggio4>2) {
              printf("Inserisci un valore corretto.\n" );
              sleep(1);
              system("clear");
                }
              else{
                  controllo4=1;
                    }
          } while(controllo4==0);
          /*nel caso in cui la zona scelta dai giocatori sia la stessa, il secondo a scegliere viene
          viene posizionato nelle riga opposta a quella del primo giocatore*/
          if (scelta_atterraggio2==scelta_atterraggio4 && scelta_atterraggio1==scelta_atterraggio3) {
            printf("\033[1;34m");
            printf("OH NO!\nNella stessa posizione in cui vuoi atterrare tu ci si è già fiondato Ninja.\n");
            printf("Ti ha già rubato tutto il loot, non è saggio cercare di UCCIDERLO SOLO CON UN PICCONE, LUI HA LO SCAR!!...\n");
            sleep(3);
            printf("Ciccio devia così la sua traiettoria di atterraggio verso la direzione opposta in cui stava andando.\n");
            printf("CI BECCHEREMO DOPO NINJA!\nNON PENSARE DI FARLA FRANCA!!\n");
            sleep(3);
            printf("\033[0m");
            if (scelta_atterraggio4==1) {
              Ciccio.y=n-1;
              }
              else{
                if (scelta_atterraggio4==2) {
                  Ciccio.y=0;
                  }
                }
              }
              else{
                if (scelta_atterraggio2!=scelta_atterraggio4) {
                  printf("\033[1;34m");
                  printf("Non vedo nessuno nelle vicinanze. Sarà un atterraggio tranquillo.\n\n\n");
                  printf("\033[0m");
                  sleep(2);
                  if (scelta_atterraggio4==1) {
                      Ciccio.y=0;
                    }
                    else{
                      if (scelta_atterraggio4==2) {
                        Ciccio.y=n-1;
                      }
                    }
                }
              }
      }
    }
    system("clear");
    if (dado>=1 && dado<4) {
      printf("Ninja è atterrato in posizione |%hu|-|%hu|\n",Ninja.x, Ninja.y );
      Ninja_posizionato=1;//indica che ciccio è stato posizionato
      }
      else{
        if (dado>=4 && dado<7) {
          printf("Ciccio è atterrato in posizione |%hu|-|%hu|\n",Ciccio.x, Ciccio.y );
          Ciccio_posizionato=1;//indica che il ninja è stato posizionato
        }
      }
  printf("posizionamento secondo giocatore completa" );
  sleep(2);
  system("clear");
  printf("\n\n\n" );
  printf("\033[1;34m");
  printf("Ninja è atterrato in posizione |%hu|-|%hu|\n",Ninja.x, Ninja.y );
  printf("Ciccio è atterrato in posizione |%hu|-|%hu|\n\n\n",Ciccio.x, Ciccio.y );
  printf("\033[0m");
  //azzeramento delle celle dove sono atterrati i due giocatori. A prescindere da cosa vi si trova dentro
  scacchiera[(Ninja.y * n)+(Ninja.x)].pericolo=nessun_pericolo;
  scacchiera[(Ciccio.y * n)+(Ciccio.x)].pericolo=nessun_pericolo;
  scacchiera[(Ninja.y * n)+(Ninja.x)].oggetto=nessun_oggetto;
  scacchiera[(Ciccio.y * n)+(Ciccio.x)].oggetto=nessun_oggetto;
  sleep(4);
  giocatori_posizionati=1;
  system("clear");

}

//fa iniziare la partita solo nel caso in cui i giocatori sono stati precedentemente posizionati
static void inizia_partita(){
  unsigned short int controllo_menu=0; //controlla le scelte del giocatore nel menu in cui sceglie cosa fare
  unsigned short int scelta1=0;//gestisce le scelte del giocatore nel relativo menu
  unsigned short int direzione=0;//gestisce la direzione che in cui si vuole muovere il giocatore
  unsigned short int controllo2=0;//controllo sul valore della direzione inserita
  unsigned short int controllo_turno=0;//permette di cambiare turno se la mossa del giocatore è stata esuguita correttamente
  unsigned short int opzione_eseguita=0; //verifica se l'opzione scelta nel menu è stata eseguita
  unsigned short int controllo_menu_zaino=0;//controlla i valori inseriti nel menu quando si apre lo zaino
  unsigned short int scelta_menu_zaino=0;//contiene la scelta nel menù dello zaino
  unsigned short int nome; //se è uguale a 1 indica Ninja , se è uguale a 2 indica Ciccio
  contatore_turni=0; //azzeramento
  printf("\033[1;38m");
  printf("Ora avrà inizio la partita vera e propria. I giocatori sono atterrati nella mappa e inizieranno\n" );
  printf("a muoversi all'interno di essa, interagendo con i vari oggetti incotrati e con le insiedie del territorio...\n" );
  sleep(4);
  printf("SE I DUE GIOCATORI AD UN CERTO PUNTO SI TROVANO NELLO STESSO PUNTO DELLA MAPPA SI PASSA ALLA FASE DELLO SCONTRO FINALE.\n" );
  printf("L'ultimo dei due giocatori che resta in vita vince la partita.\n" );
  printf("BUONA FORTUNA!\n\n\n" );
  sleep(6);
  system("clear");
  printf("Si giocherà a turni. Quindi Ninja e Ciccio si muoveranno uno alla volta...\n" );
  sleep(3);
  printf("\nI TURNI PARI GIOCA CICCIO  E  I TURNI DISPARI GIOCA NINJA\n\n");
  printf("Verrà scelto un numero a caso e in base a quel numero si deciderà chi inizia a muoversi per primo.\n");
  sleep(5);
  contatore_turni=(rand()%2);
  printf("\033[1;31m");
  printf("è stato sorteggiato il numero %hu \n\n\n\n",contatore_turni );
  printf("\033[0m");
  do {
    do {
      if (scontro_finale==1) { //se i giocatori non sono nella stessa posizione si continua con i turni
        printf("SCONTRO FINALE\n"); //se i due giocatori sono nella stessa cella parte lo scontro finale
        sleep(3);
        controllo_turno=1;//per passare subito allo scontro finale e saltare i controlli
      }
      else{
      controllo_menu=0; //riazzaremanto dei valori (per sicurezza)
      controllo_menu_zaino=0;
      controllo_turno=0;
      controllo2=0;
      turno_passato=0;
      scelta1=0;
      opzione_eseguita=0;
      scelta_menu_zaino=0;
      nome=0;
      if ((contatore_turni+1)%(2*n)==0 && scontro_finale==0) { //dopo un numero di turni la mappa si restringe
        riduci_mappa(&Ciccio, &Ninja);
        if (scontro_finale==0) { //la mappa viene stampata solo se quando si restringe la mappa i due giocatori non sono nella stessa posizione
          if (livello_difficolta==0) { //se si è in modalità facile si stampa normalmente
            stampa_mappa_e_giocatori(Ciccio,Ninja);
          }
          else{
            if (livello_difficolta==1) { //se si è in modalità difficile, si forza la funzione di stampa mappa e giocatori
              livello_difficolta=0; //si cambia il livello di difficoltà per permettere di stampare la mappa anche in modalità difficile quando si dimezza
              stampa_mappa_e_giocatori(Ciccio,Ninja);
              livello_difficolta=1;//si reimposta il livello di difficoltò
            }
          }
        }
      }
      if (contatore_turni%2==0 && scontro_finale==0) { //i turni pari e quando i due giocatori non sono nella stessa posizione
        printf("\033[1;35m");
        printf("E il turno di Ciccio\n" );
        nome=2;
        sleep(1);
        do {
          printf("SCEGLI COSA FARE:\n1: MUOVI\n2: APRI ZAINO(mostra stato del giocatore e permette di utilizzare gli oggetti)\n3: STAMPA MAPPA\n");
          printf("\nOGNI OPZIONE CONSUMA IL TURNO\n");
          scanf("%hu",&scelta1);
          if (scelta1<1 || scelta1>3) {
            printf("\033[1;31m");
            printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
            printf("\033[1;35m");
            sleep(1);
            system("clear");
              }
              else{
                controllo_menu=1;
                }
          } while(controllo_menu==0);
      switch (scelta1) {
        case 1:
                do {
                  do {
                  printf("SCEGLI IN CHE DIREZIONE VUOI MUOVERTI:\n1: SU\n2: GIU\n3: DESTRA\n4: SINISTRA\n" );
                  scanf("%hu",&direzione );
                  if (direzione<1 || direzione>4) {
                    printf("\033[1;31m");
                    printf("Inserisci una direzione tra quelli disponibili nel menù.\n" );
                    printf("\033[1;35m");
                    sleep(1);
                    system("clear");
                  }
                  else{
                    controllo2=1;
                    }
                  } while(controllo2==0);
                  switch (direzione) {
                    case 1: muovi_su(&Ciccio);
                      break;
                    case 2: muovi_giu(&Ciccio);
                      break;
                    case 3: muovi_dx(&Ciccio);
                      break;
                    case 4: muovi_sx(&Ciccio);
                      break;
                    default:
                      break;
                    }
                    if (turno_passato==1) {
                        controllo_turno=1;
                        opzione_eseguita=1;
                        if (Ninja.x == Ciccio.x && Ninja.y==Ciccio.y) {
                          scontro_finale=1;
                          }
                          else{
                            interagire(&Ciccio); //interagisce solo se il gicatore si è spostato correttamente
                            if (giocatore_morto==1) {//se il giocatore muore spostandosi su una trappola si assegna l'altro come vincitore
                              vincitore=1;
                            }
                          }
                        }
                        else{
                          if (turno_passato==0) {
                            controllo_turno=0;
                            }
                          }
                } while(opzione_eseguita==0);

          break;
        case 2: stampa_statistiche(&Ciccio, nome);
                do {
                  printf("SCEGLI COSA FARE:\n1: USA MEDIKIT\n2: USA POZIONE\n3: TORNA INDIETRO\n");
                  scanf("%hu",&scelta_menu_zaino);
                  if (scelta_menu_zaino<1 || scelta_menu_zaino>3) {
                    printf("\033[1;31m");
                    printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
                    printf("\033[1;35m");
                    sleep(1);
                    system("clear");
                  }
                  else{
                    controllo_menu_zaino=1;
                  }
                } while(controllo_menu_zaino==0);
                if (scelta_menu_zaino==1) {
                  if (Ciccio.zaino[medikit-1]>=1) {
                    switch (Ciccio.stato) {
                      case vulnerabile: Ciccio.stato=solo_vita;
                                        Ciccio.zaino[medikit-1]--;
                                        controllo_turno=1;
                                        stampa_statistiche(&Ciccio, nome);
                        break;
                      case scudo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                        break;
                      case solo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                        break;
                      case solo_scudo:  Ciccio.stato=scudo_vita;
                                        Ciccio.zaino[medikit-1]--;
                                        controllo_turno=1;
                                        stampa_statistiche(&Ciccio, nome);
                        break;
                      default:
                        break;
                    }
                    }
                    else{
                      printf("Non hai nessun medikit da poter utilizzare\n");
                    }
                  }
                  else{
                    if (scelta_menu_zaino==2) {
                      if (Ciccio.zaino[pozione-1]>=1) {
                          switch (Ciccio.stato) {
                            case vulnerabile: Ciccio.stato=solo_scudo;
                                            Ciccio.zaino[pozione-1]--;
                                            controllo_turno=1;
                                            stampa_statistiche(&Ciccio, nome);
                             break;
                            case scudo_vita: printf("Hai già lo scudo, non puoi usare la pozione.\n");
                              break;
                            case solo_vita: Ciccio.stato=scudo_vita;
                                          Ciccio.zaino[pozione-1]--;
                                          controllo_turno=1;
                                          stampa_statistiche(&Ciccio, nome);
                             break;
                            case solo_scudo:  printf("Hai già lo scudo, non puoi usare la pozione.\n");
                             break;
                            default:
                             break;
                           }
                          }
                          else{
                            printf("Non hai nessuna pozione da poter utilizzare\n");
                          }
                        }
                        else{
                          if (scelta_menu_zaino==3) {
                            torna_indietro();
                          }
                        }
                  }

          break;
        case 3: stampa_mappa_e_giocatori(Ciccio,Ninja);
                system("clear");
                if (livello_difficolta==0) { //il turno passa solo se la mappa può essere stampata
                  controllo_turno=1;
                }
          break;
        default:
          break;
        }
      }
      else{
        if (contatore_turni%2==1 && scontro_finale==0) {//i turni dispari e quando i due giocatori non sono nella stessa posizione
          printf("\033[1;33m");
          printf("è il turno di Ninja\n" );
          nome=1;
          sleep(1);
          do {
            printf("SCEGLI COSA FARE:\n1: MUOVI\n2: APRI ZAINO(mostra stato del giocatore e permette di utilizzare gli oggetti)\n3: STAMPA MAPPA\n");
            printf("\nOGNI OPZIONE CONSUMA IL TURNO\n");
            scanf("%hu",&scelta1);
            if (scelta1<1 || scelta1>3) {
              printf("\033[1;31m");
              printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
              printf("\033[1;33m");
              sleep(1);
              system("clear");
                }
                else{
                  controllo_menu=1;
                  }
            } while(controllo_menu==0);
            switch (scelta1) {
              case 1:
                  do {
                    do {
                    printf("SCEGLI IN CHE DIREZIONE VUOI MUOVERTI:\n1: SU\n2: GIU\n3: DESTRA\n4: SINISTRA\n" );
                    scanf("%hu",&direzione );
                    if (direzione<1 || direzione>4) {
                      printf("\033[1;31m");
                      printf("Inserisci una direzione tra quelli disponibili nel menù.\n" );
                      printf("\033[1;33m");
                      sleep(1);
                      system("clear");
                    }
                    else{
                      controllo2=1;
                      }
                    } while(controllo2==0);
                    switch (direzione) {
                      case 1: muovi_su(&Ninja);
                        break;
                      case 2: muovi_giu(&Ninja);
                        break;
                      case 3: muovi_dx(&Ninja);
                        break;
                      case 4: muovi_sx(&Ninja);
                        break;
                      default:
                        break;
                      }
                      if (turno_passato==1) {
                          controllo_turno=1;
                          opzione_eseguita=1;
                          if (Ninja.x == Ciccio.x && Ninja.y==Ciccio.y) {
                            scontro_finale=1;
                            }
                            else{
                              interagire(&Ninja); //interagisce solo se il gicatore si è spostato correttamente
                              if (giocatore_morto==1) {//se il giocatore muore spostandosi su una trappola si assegna l'altro come vincitore
                                vincitore=2;
                              }
                            }
                          }
                          else{
                            if (turno_passato==0) {
                              controllo_turno=0;
                              }
                            }
                  } while(opzione_eseguita==0);

            break;
          case 2: stampa_statistiche(&Ninja,nome);
                    do {
                    printf("SCEGLI COSA FARE:\n1: USA MEDIKIT\n2: USA POZIONE\n3: TORNA INDIETRO\n");
                    scanf("%hu",&scelta_menu_zaino);
                    if (scelta_menu_zaino<1 || scelta_menu_zaino>3) {
                      printf("\033[1;31m");
                      printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
                      printf("\033[1;33m");
                      sleep(1);
                      system("clear");
                    }
                    else{
                      controllo_menu_zaino=1;
                    }
                  } while(controllo_menu_zaino==0);
                  if (scelta_menu_zaino==1) {
                    if (Ninja.zaino[medikit-1]>=1) {
                      switch (Ninja.stato) {
                        case vulnerabile: Ninja.stato=solo_vita;
                                  Ninja.zaino[medikit-1]--;
                                  controllo_turno=1;
                                  stampa_statistiche(&Ninja,nome);
                          break;
                        case scudo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                          break;
                        case solo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                          break;
                        case solo_scudo:  Ninja.stato=scudo_vita;
                                  Ninja.zaino[medikit-1]--;
                                  controllo_turno=1;
                                  stampa_statistiche(&Ninja,nome);
                          break;
                        default:
                          break;
                        }
                      }
                      else{
                        printf("Non hai nessun medikit da poter utilizzare\n");
                      }
                    }
                    else{
                      if (scelta_menu_zaino==2) {
                        if (Ninja.zaino[pozione-1]>=1) {
                          switch (Ninja.stato) {
                            case vulnerabile: Ninja.stato=solo_scudo;
                                      Ninja.zaino[pozione-1]--;
                                      controllo_turno=1;
                                      stampa_statistiche(&Ninja, nome);
                              break;
                            case scudo_vita: printf("Hai già lo scudo, non puoi usare la pozione.\n");
                              break;
                            case solo_vita: Ninja.stato=scudo_vita;
                                            Ninja.zaino[pozione-1]--;
                                            controllo_turno=1;
                                            stampa_statistiche(&Ninja ,nome);
                              break;
                            case solo_scudo:  printf("Hai già lo scudo, non puoi usare la pozione.\n");
                              break;
                            default:
                              break;
                            }
                          }
                          else{
                            printf("Non hai nessuna pozione da poter utilizzare\n");
                          }
                        }
                        else{
                          if (scelta_menu_zaino==3) {
                            torna_indietro();
                          }
                        }
                    }
            break;
          case 3: stampa_mappa_e_giocatori(Ciccio,Ninja);
                  system("clear");
                  if (livello_difficolta==0) { //il turno passa solo se la mappa può essere stampata
                    controllo_turno=1;
                  }
            break;
          default:
            break;
          }
        }
      }
        printf("\033[0m");
        }
    } while(controllo_turno==0);
    contatore_turni++; //incrementa i turni
    system("clear");
    printf("\033[0m");
    if (scontro_finale==1) {
      battaglia_finale();
    }
  } while(giocatore_morto==0); //esegue finchè un giocatore muore
  if (giocatore_morto==1) { //stampa statistiche partite e azzeramento di tutti i valori



//INSERIRE PRINT CO VITTORIA REALE


    if (vincitore==1) {
      printf("\033[1;33m");
      printf("\n\n\n\n");
      printf("\t\t\t __   __   __     ______   ______   ______     ______     __     ______        ______     ______     ______     __         ______   \n");
      printf("\t\t\t/\\ \\ / /  /\\ \\   /\\__  _\\ /\\__  _\\ /\\  __ \\   /\\  == \\   /\\ \\   \\/\\  __ \\      /\\  == \\   /\\  ___\\   /\\  __ \\   /\\ \\       /\\  ___\\  \n");
      printf("\t\t\t\\ \\ \\'/   \\ \\ \\  \\/_/\\ \\/ \\/_/\\ \\/ \\ \\ \\/\\ \\  \\ \\  __<   \\ \\ \\  \\ \\  __ \\     \\ \\  __<   \\ \\  __\\   \\ \\  __ \\  \\ \\ \\____  \\ \\  __\\\n");
      printf("\t\t\t \\ \\__|    \\ \\_\\    \\ \\_\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\  \\ \\_\\ \\_\\     \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_____\\ \n");
      printf("\t\t\t  \\/_/      \\/_/     \\/_/     \\/_/   \\/_____/   \\/_/ /_/   \\/_/   \\/_/\\/_/      \\/_/ /_/   \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/\n");
      sleep(4);
      printf("\n\n\n\n\n\n");
      printf("COMPLIMENTI NINJA HAI VINTO LA PARTITA!!\n\n");
      printf("STATISTICHE ALIENI UCCISI:\n");
      printf("Ninja hai ucciso %hu alieni \n\n\n",Ninja.alieni_uccisi );
      sleep(2);
      nome=1;
      sleep(3);
      stampa_statistiche(&Ninja,nome);
    }
    else{
      if (vincitore==2) {
        printf("\033[1;35m");
        printf("\n\n\n\n");
        printf("\t\t\t __   __   __     ______   ______   ______     ______     __     ______        ______     ______     ______     __         ______   ");
        printf("\t\t\t/\\ \\ / /  /\\ \\   /\\__  _\\ /\\__  _\\ /\\  __ \\   /\\  == \\   /\\ \\   \\/\\  __ \\      /\\  == \\   /\\  ___\\   /\\  __ \\   /\\ \\       /\\  ___\\  \n");
        printf("\t\t\t\\ \\ \\'/   \\ \\ \\  \\/_/\\ \\/ \\/_/\\ \\/ \\ \\ \\/\\ \\  \\ \\  __<   \\ \\ \\  \\ \\  __ \\     \\ \\  __<   \\ \\  __\\   \\ \\  __ \\  \\ \\ \\____  \\ \\  __\\\n");
        printf("\t\t\t \\ \\__|    \\ \\_\\    \\ \\_\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_\\  \\ \\_\\ \\_\\     \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_____\\ \n");
        printf("\t\t\t  \\/_/      \\/_/     \\/_/     \\/_/   \\/_____/   \\/_/ /_/   \\/_/   \\/_/\\/_/      \\/_/ /_/   \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/\n");
        sleep(4);
        printf("\n\n\n\n\n\n");
        printf("COMPLIMENTI CICCIO HAI VINTO LA PARTITA!!\n\n");
        printf("STATISTICHE ALIENI UCCISI:\n");
        printf("Ciccio hai ucciso %hu alieni \n",Ciccio.alieni_uccisi );
        nome=2;
        sleep(3);
        stampa_statistiche(&Ciccio,nome);
      }
    }
  }
  sleep(3);
  //riazzeramento di tutti i valori
  mappa_creata=0;
  giocatori_posizionati=0;
  giocatore_morto=0;
  vincitore=0;
  livello_difficolta=0;
  scontro_finale=0;
  risultato_morra=0;
  contatore_turni=0;
  n=0;
  free(scacchiera);
  scacchiera=NULL;
  Ninja.stato=solo_vita;
  Ninja.alieni_uccisi=0;
  Ninja.zaino[medikit-1]=0;
  Ninja.zaino[pozione-1]=0;
  Ninja.zaino[materiale-1]=0;
  Ninja.zaino[colpi_lanciarazzi-1]=0;
  Ninja.x=0;
  Ninja.y=0;
  Ciccio.stato=solo_vita;
  Ciccio.alieni_uccisi=0;
  Ciccio.zaino[medikit-1]=0;
  Ciccio.zaino[pozione-1]=0;
  Ciccio.zaino[materiale-1]=0;
  Ciccio.zaino[colpi_lanciarazzi-1]=0;
  Ciccio.x=0;
  Ciccio.y=0;
  printf("\033[1;36m");
}

static void stampa_mappa_e_giocatori(struct Giocatore giocatore1, struct Giocatore giocatore2){
  unsigned int rows=n;
  unsigned int cols=n;
  unsigned short int posizione1=0;
  unsigned short int posizione2=0;
  system("clear");
  printf("\033[1;34m");
  /*la mappa può essere stampata o meno in base al livello di difficoltà e solo se
  i giocatori sono stati precedentemente posizionati*/
  if (livello_difficolta==0 && giocatori_posizionati==1) {
    for (int i = 0; i <rows ; i++) {
      for (int j = 0; j <cols; j++) {
        posizione1=0;
        posizione2=0;
        printf(" ||");
        if (giocatore1.y==i && giocatore1.x==j) {
          posizione1=1;
            }
            else{
              if (giocatore2.y==i && giocatore2.x==j) {
                posizione2=1;
                  }
                }
          if (posizione1==1) {
            printf("CICCIO");
              }
              else{
                if (posizione2==1) {
                  printf("NINJA");
                    }
                    else{
                      //stampa i pericoli
                      switch (scacchiera[i*cols+j].pericolo) {
                        case 0: printf("0");
                          break;
                        case 1: printf("T");
                          break;
                        case 2: printf("A");
                          break;
                        default:
                          break;
                        }
                      printf(" - ");
                        //stampa gli oggetti
                      switch (scacchiera[i*cols+j].oggetto) {
                          case 0: printf("X" );
                            break;
                          case 1: printf("C");
                            break;
                          case 2: printf("P");
                            break;
                          case 3:printf("M");
                            break;
                          case 4:printf("L");
                            break;
                          default:
                            break;
                      }
                    }
                  }
                }
                printf("||\n\n");
              }
          printf("\n\n\n\n");
          printf("\033[1;31m");
          printf("%60s\n","LEGENDA:" );
          printf("A=Alieno   T=Trappola    C=Medikit    P=Pozione    M=Materiale   L=Colpo Lanciarazzi     " );
          printf("0=Nessun Pericolo     X=Nessun Oggetto\n");
          printf("\033[0m");
          sleep(15);
          system("clear");
    }
    else{
      if (livello_difficolta!=0 || giocatori_posizionati!=1) {
        printf("\033[1;31m");
        printf("Non hai scelto la modalità facile oppure non hai posizionato i giocatori.\n" );
        printf("Non puoi quindi usare questa funzione.\n" );
        sleep(2);
        printf("\033[0m");
        system("clear");
        }
    }
    printf("\033[0m");
}

//da qui le quattro funzioni che permettono il movimento sulla mappa dei giocatori
static void muovi_su(struct Giocatore *giocatore){
  if (giocatore->y >=1) {
    printf("muovi su\n" );
    giocatore->y--;
    turno_passato=1; //variabile globale che controlla se la mossa è stata eseguita correttamente
    }
    else{
      if (giocatore->y==0) {
        printf("Non puoi eseguire questa mossa perche ti trovi già al bordo superiore della mappa\n");
        sleep(2);
        system("clear");
      }
    }
}
static void muovi_giu(struct Giocatore *giocatore){
  if (giocatore->y<(n-1)) {
    printf("muovi giu\n" );
    giocatore->y++;
    turno_passato=1;//variabile globale che controlla se la mossa è stata eseguita correttamente
    }
    else{
      if (giocatore->y==(n-1)) {
        printf("Non puoi eseguire questa mossa perche ti trovi già al bordo inferiore della mappa\n");
        sleep(2);
        system("clear");
      }
    }
}
static void muovi_dx(struct Giocatore *giocatore){
  if (giocatore->x<(n-1)) {
    printf("muovi dx\n" );
    giocatore->x++;
    turno_passato=1;//variabile globale che controlla se la mossa è stata eseguita correttamente
    }
    else{
      if (giocatore->x==(n-1)) {
        printf("Non puoi eseguire questa mossa perche ti trovi già al bordo destro della mappa\n");
        sleep(2);
        system("clear");
      }
    }
}
static void muovi_sx(struct Giocatore *giocatore){
  if (giocatore->x>=1) {
    printf("muovi sx\n" );
    giocatore->x--;
    turno_passato=1;//variabile globale che controlla se la mossa è stata eseguita correttamente
    }
    else{
      if (giocatore->x==0) {
        printf("Non puoi eseguire questa mossa perche ti trovi già al bordo sinistro della mappa\n");
        sleep(2);
        system("clear");
      }
    }
}

//funzione per interagire con gli oggetti e i pericoli nella mappa
static void interagire(struct Giocatore *giocatore){
  unsigned short int controllo=0;
  unsigned short int oggetto_visibile=0; //variabile che serve al controllo per decidere se l'oggetto può essere visto o meno dal giocatore
  unsigned short int sceltaMenuOggetti=0;
  unsigned short int sceltaMenuAlieno=0;
  unsigned short int controlloAlieno=0;//controllo del ciclo con alieno
  unsigned short int scontro_alieno=0; //variabile che assume un valore casuale in base a cui il giocatore esce illeso o meno dallo scontro con l'alieno
  unsigned short int scontro_alieno2=0;//nel caso il giocatore subisca danno, con questa variabile viene scelto se perde uno tra scudo o vita.
  switch (scacchiera[(giocatore->y * n)+(giocatore->x)].pericolo) {
    case 0: printf("Tranquillo qui non c'è nessun pericolo, siamo al sicuro\n" );
            oggetto_visibile=1;
      break;
    case 1: printf("OH NO! Qui c'è una trappola.\n" );
            printf("SEI MORTO A CASUSA DELLA TRAPPOLA...\n" );
            giocatore_morto=1;
            oggetto_visibile=0;
      break;
    case 2: printf("OPS! C'è un alieno.\n\n\n\n" );
            sleep(1);
            printf("\033[1;39m");
            printf("      .--.   |V|\n");
            printf("     /    \\ _| /\n");
            printf("     q .. p \\ /\n");
            printf("      \\--/  //\n");
            printf("     __||__//\n");
            printf("    /.    _/\n");
            printf("   // \\  /\n");
            printf("  //   ||\n");
            printf("  \\\\  /  \\\n");
            printf("   )\\|    |\n");
            printf("  / || || |\n");
            printf("  |/\\| || |\n");
            printf("     | || |\n");
            printf("     \\ || /\n");
            printf("   __/ || \\__\n");
            printf("  \\____/\\____/\n");
            sleep(3);
            printf("\n\n\n\n");
            do {
              printf("VUOI COMBATTERE CONTRO L'ALIENO? Solo battendolo avrai la possibilità di prendere l'oggetto che si trova qui.\n1)SI\n2)NO\n" );
              scanf("%hu",&sceltaMenuAlieno );
              if (sceltaMenuAlieno<1 || sceltaMenuAlieno>2) {
                controlloAlieno=0;
                printf("Inserisci un valore corretto.\n" );
                sleep(1);
                system("clear");
                }
                else{
                  controlloAlieno=1;
                }
              } while(controlloAlieno==0);
              if (sceltaMenuAlieno==1) {
                scontro_alieno=rand() % 100; //per vedere se il giocatore esce illeso o meno dallo scontro
                if (scontro_alieno>=0 && scontro_alieno<35) {
                  printf("Sei stato ferito durante lo scontro con l'alieno.\n");
                  scontro_alieno2=rand() % 100;
                  if (scontro_alieno2>=0 && scontro_alieno2<35) {
                    printf("Hai perso la vita.\n");
                    if (giocatore->stato==vulnerabile) {
                      printf("Sei stato ucciso durante lo scontro con l'alieno.\n" );
                      giocatore_morto=1;
                      oggetto_visibile=0;
                      }
                      else{
                        switch (giocatore->stato) { //toglie la vita quando è possibile o lascia in stato vulnerabile
                          case scudo_vita: giocatore->stato=solo_scudo;
                            break;
                          case solo_scudo: giocatore->stato=vulnerabile;
                            break;
                          case solo_vita: giocatore->stato=vulnerabile;
                            break;
                          default:
                            break;
                        }
                        scacchiera[(giocatore->y * n)+(giocatore->x)].pericolo=nessun_pericolo;
                        giocatore->alieni_uccisi++;
                        oggetto_visibile=1;
                      }
                    }
                    else{
                      if (scontro_alieno2>=35) {
                        printf("Hai perso lo scudo.\n");
                        if (giocatore->stato==vulnerabile) {
                          printf("Sei stato ucciso durante lo scontro con l'alieno.\n" );
                          giocatore_morto=1;
                          oggetto_visibile=0;
                          }
                          else{
                            switch (giocatore->stato) { //toglie lo scudo quando è possibile o lascia in stato vulnerabile
                              case scudo_vita: giocatore->stato=solo_vita;
                                break;
                              case solo_scudo: giocatore->stato=vulnerabile;
                                break;
                              case  solo_vita: giocatore->stato=vulnerabile;
                                break;
                              default:
                                break;
                            }
                            scacchiera[(giocatore->y * n)+(giocatore->x)].pericolo=nessun_pericolo;
                            giocatore->alieni_uccisi++;
                            oggetto_visibile=1;
                          }
                      }
                    }
                  }
                  else{
                    if (scontro_alieno>=35) {
                      printf("Sei uscito illeso dallo scontro.\n" );
                      scacchiera[(giocatore->y * n)+(giocatore->x)].pericolo=nessun_pericolo;
                      giocatore->alieni_uccisi++;
                      oggetto_visibile=1;
                    }
                  }
                }
                else{
                  if (sceltaMenuAlieno==2) {
                    printf("Hai deciso di non combattere l'alieno. Non potrai prendere l'oggetto che si trova qui.\n");
                    oggetto_visibile=0; //rendere l'oggetto della casella in cui si trova l'alieno visibile o meno
                  }
                }
                sleep(3);
      break;
    default:
      break;
    }
  /*interagisce con gli oggetti solo se il giocatore non ha incontrato una trappola e non è morto
  e ha sconfitto l'alieno*/
  if (giocatore_morto==0 && oggetto_visibile==1) {
    switch (scacchiera[(giocatore->y * n)+(giocatore->x)].oggetto) {
      case 0: printf("Qui non c'è nessun oggetto\n");
        break;
      case 1: printf("Qui c'è un Medikit\n");
              do {
                printf("VUOI RACCOGLIERE L'OGGETTO?\n1)SI\n2)NO\n" );
                scanf("%hu",&sceltaMenuOggetti );
                if (sceltaMenuOggetti<1 || sceltaMenuOggetti>2) {
                  controllo=0;
                  printf("Inserisci un valore corretto.\n" );
                  sleep(1);
                  system("clear");
                  }
                  else{
                    controllo=1;
                  }
                } while(controllo==0);
                if (sceltaMenuOggetti==1) {
                    if (giocatore->zaino[medikit-1]<3) { //controlla se l'oggeto può essere aggiunto allo zaino
                      giocatore->zaino[medikit-1]++;
                      scacchiera[(giocatore->y * n)+(giocatore->x)].oggetto=nessun_oggetto;
                      printf("Hai raccolto un medikit, ora ne hai %hu nel tuo zaino.\n",giocatore->zaino[medikit-1]);
                    }
                    else{
                      if (giocatore->zaino[medikit-1]>=3) {
                        printf("Hai lo scompartimento per i medikit pieno, non puoi raccoglierne un altro.\n");
                      }
                    }
                  }
                  else{
                    if (sceltaMenuOggetti==2) {
                      printf("Hai lasciato qui un medikit.\n");
                    }
                  }
        break;
      case 2: printf("Qui c'è una pozione\n");
              do {
                printf("VUOI RACCOGLIERE L'OGGETTO?\n1)SI\n2)NO\n" );
                scanf("%hu",&sceltaMenuOggetti );
                if (sceltaMenuOggetti<1 || sceltaMenuOggetti>2) {
                  controllo=0;
                  printf("Inserisci un valore corretto.\n" );
                  sleep(1);
                  system("clear");
                  }
                  else{
                    controllo=1;
                  }
              } while(controllo==0);
              if (sceltaMenuOggetti==1) {
                if (giocatore->zaino[pozione-1]<3) {//controlla se l'oggeto può essere aggiunto allo zaino
                  giocatore->zaino[pozione-1]++;
                  scacchiera[(giocatore->y * n)+(giocatore->x)].oggetto=nessun_oggetto;
                  printf("Hai raccolto una pozione, ora ne hai %hu nel tuo zaino.\n",giocatore->zaino[pozione-1]);
                  }
                  else{
                    if (giocatore->zaino[pozione-1]>=3) {
                      printf("Hai lo scompartimento per le pozioni pieno, non puoi raccoglierne un'altra.\n");
                    }
                  }
                }
                else{
                  if (sceltaMenuOggetti==2) {
                    printf("Hai lasciato qui una pozione.\n");
                  }
                }
        break;
      case 3:printf("Qui c'è del materiale\n");
            do {
              printf("VUOI RACCOGLIERE L'OGGETTO?\n1)SI\n2)NO\n" );
              scanf("%hu",&sceltaMenuOggetti );
              if (sceltaMenuOggetti<1 || sceltaMenuOggetti>2) {
                controllo=0;
                printf("Inserisci un valore corretto.\n" );
                sleep(1);
                system("clear");
                }
              else{
                controllo=1;
              }
            } while(controllo==0);
            if (sceltaMenuOggetti==1) {
              if (giocatore->zaino[materiale-1]<3) {//controlla se l'oggeto può essere aggiunto allo zaino
                giocatore->zaino[materiale-1]++;
                scacchiera[(giocatore->y * n)+(giocatore->x)].oggetto=nessun_oggetto;
                printf("Hai raccolto del materiale, ora ne hai %hu nel tuo zaino.\n",giocatore->zaino[materiale-1]);
              }
              else{
                if (giocatore->zaino[materiale-1]>=3) {
                  printf("Hai lo scompartimento per il materiale pieno, non puoi raccoglierne dell'altro.\n");
                }
              }
            }
            else{
              if (sceltaMenuOggetti==2) {
                printf("Hai lasciato qui del materiale.\n");
              }
            }
        break;
      case 4:printf("Qui c'è un colpo lanciarazzi\n");
             do {
               printf("VUOI RACCOGLIERE L'OGGETTO?\n1)SI\n2)NO\n" );
               scanf("%hu",&sceltaMenuOggetti );
               if (sceltaMenuOggetti<1 || sceltaMenuOggetti>2) {
                 controllo=0;
                 printf("Inserisci un valore corretto.\n" );
                 sleep(1);
                 system("clear");
               }
               else{
                 controllo=1;
              }
            } while(controllo==0);
            if (sceltaMenuOggetti==1) {
              if (giocatore->zaino[colpi_lanciarazzi-1]<3) {//controlla se l'oggeto può essere aggiunto allo zaino
                giocatore->zaino[colpi_lanciarazzi-1]++;
                scacchiera[(giocatore->y * n)+(giocatore->x)].oggetto=nessun_oggetto;
                printf("Hai raccolto un colpo per il lanciarazzi, ora ne hai %hu nel tuo zaino.\n",giocatore->zaino[colpi_lanciarazzi-1]);
                }
                else{
                  if (giocatore->zaino[colpi_lanciarazzi-1]>=3) {
                    printf("Hai lo scompartimento per i colpi del lanciarazzi pieno, non puoi raccoglierne dell'altro.\n");
                  }
                }
              }
              else{
                if (sceltaMenuOggetti==2) {
                  printf("Hai lasciato qui un colpo per il lanciarazzi.\n");
                }
              }
        break;
      default:
        break;
    }
  }
  printf("Hai ucciso %hu alieni\n",giocatore->alieni_uccisi );
  oggetto_visibile=0; //riazzeramento della possibilità di vedere o meno un oggetto
  sleep(4);
  system("clear");
}

//funzione per stampare le statistiche di vita del giocatore e il suo zaino del giocatore
static void stampa_statistiche(struct Giocatore *giocatore, unsigned short int nome){
  system("clear");
  if (nome==1) {
    printf("STATISTICHE DI NINJA\n\n\n");
  }
  else{
    if (nome==2) {
      printf("STATISTICHE DI CICCIO\n\n\n");
    }
  }
  printf("\033[1;31m");
  printf("\t+------------+-----------+---------------+---------------+--------------+\n");
  printf("\t| CONDIZIONE |  MEDIKIT  |    POZIONI    |     RAZZI     |   MATERIALE  |\n");
  printf("\t+------------+-----------+---------------+---------------+--------------+\n");
  switch (giocatore->stato) {
      case vulnerabile: printf("        | vulnerabile|");
        break;
      case solo_vita: printf("        | solo vita  |");
        break;
      case solo_scudo: printf("        | solo scudo |");
        break;
      case scudo_vita: printf("        |scudo e vita|");
        break;
      default:
        break;
  }
  switch(giocatore->zaino[medikit-1]){
      case 0: printf("     0     |");
        break;
      case 1: printf("    1      |");
        break;
      case 2: printf("     2     |");
        break;
      case 3: printf("     3     |");
        break;
      default:
        break;

  }
  switch(giocatore->zaino[pozione-1]){
    case 0: printf("      0        |");
      break;
    case 1: printf("      1        |");
      break;
    case 2: printf("      2        |");
      break;
    case 3: printf("       3       |");
      break;
    default:
      break;
      }
  switch(giocatore->zaino[colpi_lanciarazzi-1]){
    case 0: printf("        0      |");
      break;
    case 1: printf("       1       |");
      break;
    case 2: printf("       2       |");
      break;
    case 3: printf("       3       |");
      break;
    default:
      break;
    }
    switch(giocatore->zaino[materiale-1]){
      case 0: printf("     0        |");
        break;
      case 1: printf("     1        |");
        break;
      case 2: printf("     2        |");
        break;
      case 3: printf("     3        |");
        break;
      default:
        break;
      }
  printf("\n\t+------------+-----------+---------------+---------------+--------------+\n");
  printf("\033[0m");
  sleep(4);
}

/*funzione che riduce la mappa copiando la nuova parte in un'altra matrice e la rialloca nel
puntatore scacchiera utilizzato per contenere l'altra mappa; ovviamente il restringimento della mappa avviene
verso la parte centrale della mappa, come in una partita vera. Gli oggetti presenti nelle celle vengono MANTENUTI e i giocatori
al di fuori della mappa vengono RIPOSIZIONATI ai bordi e non casualmente, subendo DANNO. */
static void riduci_mappa(struct Giocatore *giocatore1, struct Giocatore *giocatore2){
  struct Cella nuova_scacchiera[n/2][n/2]; //matrice ausiliaria per copiare solo la parte della mappa dimezzata
  unsigned int nuova_dim=n/2; //dimensione nuova scacchiera
  unsigned int d=n/4; //variabile ausiliaria per iniziare a copiare le celle della vecchia scacchiera che si vogliono (indica le colonne)
  unsigned int d1=n/4; //indica le righe
  unsigned short int fuori_zona1=0;//per vedere se giocatore1 è fuori dalla mappa quando si restringe
  unsigned short int fuori_zona2=0;//per vedere se giocatore2 è fuori dalla mappa quando si restringe
   //fa partire le posizioni sulla vecchia scacchiera dal punto in cui si vuole iniziare a copiare
  if (n>3) { //solo se la dimensione della mappa può essere ridotta
    printf("La tempesta sta arrivando!...\n");
    sleep(3);
    for (unsigned short int  i = 0; i < nuova_dim; i++) {
      for (unsigned short int j = 0; j < nuova_dim; j++) {
        nuova_scacchiera[i][j].pericolo=scacchiera[d1*n+d].pericolo;
        nuova_scacchiera[i][j].oggetto=scacchiera[d1*n+d].oggetto;
        d++;
      }
      d=n/4;
      d1++;
    }
    free(scacchiera); //libera la scacchiera precendete
    scacchiera=NULL;//e la rialloca in memoria con la nuova dimensione ridotta
    scacchiera=(struct Cella *)malloc(nuova_dim*nuova_dim*sizeof(struct Cella));
    for (unsigned short int  a = 0; a < nuova_dim; a++) { //ricopia tutte le celle nella scacchiera originale
      for (unsigned short int b = 0; b < nuova_dim; b++) {
        scacchiera[a*nuova_dim+b].pericolo=nuova_scacchiera[a][b].pericolo;
        scacchiera[a*nuova_dim+b].oggetto=nuova_scacchiera[a][b].oggetto;
      }
    }
    printf("ATTENZIONE :la mappa si è ristretta.\n" );
    //riposizionamento giocatori nel caso si trovino fuori dal bordo della mappa al suo restringimento
    d=n/4;
    fuori_zona1=0;
    fuori_zona2=0;
    //colonne
    if (giocatore1->x >(n/2)-1+d) {
      giocatore1->x=(n/2)-1;
      fuori_zona1=1;
    }
    else{
      if (giocatore1->x <= n/4) {
        giocatore1->x=0;
        fuori_zona1=1;
      }
      else{
        giocatore1->x = (giocatore1->x -(n/4));
      }
    }

    if (giocatore2->x >(n/2)-1+d) {
      giocatore2->x=(n/2)-1;
      fuori_zona2=1;
     }
     else{
       if (giocatore2->x <= n/4) {
         giocatore2->x=0;
         fuori_zona1=1;
       }
       else{
         giocatore2->x = (giocatore2->x -(n/4));
       }
     }
    //righe
    if (giocatore1->y >(n/2)-1+d) {
      giocatore1->y=(n/2)-1;
      fuori_zona1=1;
     }
     else{
       if (giocatore1->y <= n/4) {
         giocatore1->y=0;
         fuori_zona1=1;
       }
       else{
         giocatore1->y= (giocatore1->y -(n/4));
       }
     }

    if (giocatore2->y >(n/2)-1+d) {
      giocatore2->y=(n/2)-1;
      fuori_zona2=1;
     }
     else{
       if (giocatore2->y <= n/4) {
         giocatore2->y=0;
         fuori_zona2=1;
       }
       else{
         giocatore2->y= (giocatore2->y -(n/4));
       }
     }

    /*se il giocatore1 si trova fuori dalla mappa quando si restringe prende danno
    ma per rendere il gioco più giocabile non viene ucciso*/
    if (fuori_zona1==1) {
      switch (giocatore1->stato) {
        case scudo_vita: giocatore1->stato=solo_scudo;
          break;
        case solo_scudo: giocatore1->stato=vulnerabile;
          break;
        case solo_vita: giocatore1->stato=vulnerabile;
          break;
        default:
          break;
      }
      printf("Ciccio hai preso danno perchè la tempesta ti ha colpito mentre rientravi in safe zone.\n" );
    }
    /*se il giocatore1 si trova fuori dalla mappa quando si restringe prende danno
    ma per rendere il gioco più giocabile non viene ucciso*/
    if (fuori_zona2==1) {
      switch (giocatore2->stato) {
        case scudo_vita: giocatore2->stato=solo_scudo;
          break;
        case solo_scudo: giocatore2->stato=vulnerabile;
          break;
        case solo_vita: giocatore2->stato=vulnerabile;
          break;
        default:
          break;
      }
      printf("Ninja hai preso danno perchè la tempesta ti ha colpito mentre rientravi in safe zone.\n" );
    }
    if (giocatore1->x == giocatore2->x && giocatore1->y == giocatore2->y) {
      printf("Rientrando in safe Ninja e Ciccio si sono incontrati! SI PASSA ALLO SCONTRO FINALE\n"); //controlla se i giocatori sono nella stessa posizione al restringimento della mappa
      scontro_finale=1;
    }
  }
  else{
    printf("si passa allo scontro finale\n");
    scontro_finale=1;
     /*se la dimensione della mappa è 3 o più piccola, dimezzarla implica che i due giocatori si troveranno nella stessa Cella
    quindi si passa direttamente allo scontro finale*/
  }
  n=n/2; //dimezza la dimensione del lato, in modo da renderlo utilizzabile correttamente anche a tutte le altre funzioni che ne fanno uso
  sleep(2);
  //riazzeramento valoris
  fuori_zona1=0;
  fuori_zona2=0;
}

static void battaglia_finale(){
unsigned short int sceltaMenuFinale; //contiene i valori nel menù della sfida finale
unsigned short int controllo=0; //controlla le scelte inserite dall'utente nello scontro finale
unsigned short int turno_passato_finale=0;
unsigned short int scelta_menu_zaino_finale=0;
unsigned short int controllo_menu_zaino=0;
unsigned short int nome=0;
struct Piano *torre_Ciccio1=NULL;
struct Piano *torre_Ninja1=NULL;
struct Piano *torre_Ciccio2=NULL;
struct Piano *torre_Ninja2=NULL;
struct Piano *torre_Ciccio3=NULL;
struct Piano *torre_Ninja3=NULL;

printf("Ecco il tanto atteso scontro finale, i due giocatori sono sopravvisuti fino ad essersi incontrati nella mappa.\n");
printf("Ora ogni giocatore costruirà la sua torre in base al numero di materiali che possiede(se li possiede) e vi salirà sopra.\n");
sleep(4);
printf("L'obbiettivo dei due giocatori è quello di distruggere la torre avversaria lasciando così senza difese il nemico, avendo così la possibilità\n");
printf(" di attaccarlo direttamente. In questa parte di gioco sono fondamentali, e fondamentale è il loro uso, i colpi lanciarazzi.\n");
sleep(8);
printf("Ogni colpo sparato distrugge automaticamente un piano della torre avversaria o se il nemico è scoperto lo lascia in stato vulnerabile anche se ha sia lo scudo che la vita.\n");
printf("Vi è anche la possibilità di sfidare l'avversario a morra cinese in modo da non sprecare razzi fino alla fine, se a morra cinese si sconfigge l'avversario\n");
printf("si ha la possibilità di attaccare, togliendo quindi uno stato di vita o un piano della torre quando possibile. Se però non si vince\n");
printf("la sfida a morra cinese, il turno è come se fosse stato sprecato. Si consiglia quindi un uso strategico di queste combinazioni tra colpi lanciarazzi e \n");
printf("morra cinese, in modo da sconfiggere l'avversario il prima possibile.\nCome nella partita normale ad ogni turno vi è la possibilità di usare ");
printf("delle cure se il giocatore ne possiede, è anche possibile verificare lo stato di salute del giocatore.\n");
sleep(15);
printf("BUONA FORTUNA. L'ULTIMO CHE RESTA IN PIEDI VINCE!\n\n\n\n");
sleep(4);

/*è stato scelte questo modo per inizializzare e creare le liste solo perchè il numero massimo
di piani è 3, e le variabili da utilizzare sono comunque 3 anche nel caso si utilizzasse
una funzione con 3 variabili: testa, coda e scorrimento per allocare memoria per la torre.
ho scelto di utilizzare un semplice switch per rendere più chiara e più semplici
la costruzione delle torri e il loro utilizzo.*/
switch (Ciccio.zaino[materiale-1]) {
  case 1: torre_Ciccio1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio1->numero_piano=1;
          torre_Ciccio1->piano_successivo=NULL;
    break;
  case 2: torre_Ciccio1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio2 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio2->numero_piano=2;
          torre_Ciccio2->piano_successivo=NULL;
          torre_Ciccio1->numero_piano=1;
          torre_Ciccio1->piano_successivo=torre_Ciccio2;
    break;
  case 3: torre_Ciccio1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio2 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio3 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ciccio3->numero_piano=3;
          torre_Ciccio3->piano_successivo=NULL;
          torre_Ciccio2->numero_piano=2;
          torre_Ciccio2->piano_successivo=torre_Ciccio3;
          torre_Ciccio1->numero_piano=1;
          torre_Ciccio1->piano_successivo=torre_Ciccio2;
    break;
  case 0: printf("Ciccio non ha materiale, non può costrtuire una torre.\n\n");
    break;
  default:
    break;
    }
switch (Ninja.zaino[materiale-1]) {
  case 1: torre_Ninja1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja1->numero_piano=1;
          torre_Ninja1->piano_successivo=NULL;
    break;
  case 2: torre_Ninja1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja2 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja2->numero_piano=2;
          torre_Ninja2->piano_successivo=NULL;
          torre_Ninja1->numero_piano=1;
          torre_Ninja1->piano_successivo=torre_Ninja2;
    break;
  case 3: torre_Ninja1 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja2 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja3 =(struct Piano*)malloc (sizeof(struct Piano));
          torre_Ninja3->numero_piano=3;
          torre_Ninja3->piano_successivo=NULL;
          torre_Ninja2->numero_piano=2;
          torre_Ninja2->piano_successivo=torre_Ninja3;
          torre_Ninja1->numero_piano=1;
          torre_Ninja1->piano_successivo=torre_Ninja2;

    break;
  case 0: printf("Ninja non ha materiale, non può costrtuire una torre.\n\n");
    break;
  default:
    break;
  }

  Ninja.zaino[materiale-1]=0; //una volta costruitoa la torre il materiale viene azzerato
  Ciccio.zaino[materiale-1]=0;

  printf("\033[1;35m");
  printf("\nTORRE DI CICCIO\n");
  printf("---\n");
  if (torre_Ciccio1==NULL) {
    printf("Non ci sono piani\n");
    }
  else{
    struct Piano *successivo1= torre_Ciccio1;
    while (successivo1!=NULL) {
      if (successivo1->numero_piano<=3 && successivo1->numero_piano>=1) {
        printf("|%hu|\n",successivo1->numero_piano );
        printf("---\n");
      }
      successivo1= successivo1->piano_successivo;
    }
  }
  printf("\n\n\n");
  printf("\033[1;33m");
  printf("\nTORRE DI NINJA\n");
  printf("---\n");
  if (torre_Ninja1==NULL) {
    printf("Non ci sono piani\n");
    }
  else{
    struct Piano *successivo2 = torre_Ninja1;
    while (successivo2!=NULL) {
      if (successivo2->numero_piano<=3 && successivo2->numero_piano>=1) {
        printf("|%hu|\n",successivo2->numero_piano);
        printf("---\n");
      }
      successivo2= successivo2->piano_successivo;
    }
  }

  printf("\n\n\n\n\n");

  do {
    turno_passato_finale=0; //riazzeramento dei valori
    nome=0;
    sceltaMenuFinale=0;
    controllo=0;
    scelta_menu_zaino_finale=0;
    controllo_menu_zaino=0;
    turno_passato_finale=0;
    if (contatore_turni%2==0){
      printf("\033[1;35m");
      printf("è il turno di Ciccio\n\n\n");
      nome=2;
      sleep(3);
        do {
          printf("\033[1;35m");
          do {
            system("clear");
            printf("Menù opzioni FINALI:\n" );
            printf("1)SPARA COLPO LANCIARAZZI\n2)STAMPA STATISTICHE\n3)MORRA CINESE\n" );
            printf("\nScegli una delle 3 opzioni di gioco.\n");
            scanf("%hu",&sceltaMenuFinale );
            if (sceltaMenuFinale<1 || sceltaMenuFinale>3) {
              printf("\033[1;31m");
              printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
              printf("\033[1;35m");
              sleep(1);
              system("clear");
            }
              else{
                controllo=1;
                }
          } while(controllo==0);
          switch (sceltaMenuFinale) {
            case 1: if (Ciccio.zaino[colpi_lanciarazzi-1]>=1) {
                    printf("Colpo lanciarazzi\n");
                    if (torre_Ninja1==NULL) {
                      switch (Ninja.stato) {
                        case vulnerabile: giocatore_morto=1;
                                          vincitore=2;
                          break;
                        default: Ninja.stato=vulnerabile;
                          break;
                        }
                      }
                      else{
                        if (torre_Ninja2==NULL) {
                          free(torre_Ninja1);
                          torre_Ninja1=NULL;
                        }
                        else{
                          if (torre_Ninja3==NULL) {
                            free(torre_Ninja2);
                            torre_Ninja2=NULL;
                          }
                          else {
                            free(torre_Ninja3);
                            torre_Ninja3=NULL;
                          }
                        }
                      }
                      Ciccio.zaino[colpi_lanciarazzi-1]--;
                      turno_passato_finale=1;
                    }
                    else{
                      if (Ciccio.zaino[colpi_lanciarazzi-1]<1) {
                        printf("Non hai colpi da poter sparare\n");
                      }
                    }
              sleep(2);
              break;
            case 2:   stampa_statistiche(&Ciccio, nome);
                      do {
                        printf("SCEGLI COSA FARE:\n1: USA MEDIKIT\n2: USA POZIONE\n3: TORNA INDIETRO\n");
                        scanf("%hu",&scelta_menu_zaino_finale);
                        if (scelta_menu_zaino_finale<1 || scelta_menu_zaino_finale>3) {
                          printf("\033[1;31m");
                          printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
                          printf("\033[1;35m");
                          sleep(1);
                          system("clear");
                        }
                        else{
                          controllo_menu_zaino=1;
                        }
                      } while(controllo_menu_zaino==0);
                      if (scelta_menu_zaino_finale==1) {
                        if (Ciccio.zaino[medikit-1]>=1) {
                          switch (Ciccio.stato) {
                            case vulnerabile: Ciccio.stato=solo_vita;
                                              Ciccio.zaino[medikit-1]--;
                                              turno_passato_finale=1;
                                              stampa_statistiche(&Ciccio, nome);
                              break;
                          case scudo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                            break;
                          case solo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                            break;
                          case solo_scudo:  Ciccio.stato=scudo_vita;
                                            Ciccio.zaino[medikit-1]--;
                                            turno_passato_finale=1;
                                            stampa_statistiche(&Ciccio, nome);
                            break;
                          default:
                            break;
                          }
                        }
                        else{
                          printf("Non hai nessun medikit da poter utilizzare\n");
                        }
                      }
                      else{
                        if (scelta_menu_zaino_finale==2) {
                          if (Ciccio.zaino[pozione-1]>=1) {
                            switch (Ciccio.stato) {
                              case vulnerabile: Ciccio.stato=solo_scudo;
                                                Ciccio.zaino[pozione-1]--;
                                                turno_passato_finale=1;
                                                stampa_statistiche(&Ciccio, nome);
                                break;
                              case scudo_vita: printf("Hai già lo scudo, non puoi usare la pozione.\n");
                                break;
                              case solo_vita: Ciccio.stato=scudo_vita;
                                              Ciccio.zaino[pozione-1]--;
                                              turno_passato_finale=1;
                                              stampa_statistiche(&Ciccio, nome);
                              break;
                            case solo_scudo:  printf("Hai già lo scudo, non puoi usare la pozione.\n");
                              break;
                            default:
                              break;
                            }
                          }
                          else{
                            printf("Non hai nessuna pozione da poter utilizzare\n");
                          }
                        }
                      }
                      sleep(1);

              break;
            case 3: morra_cinese(nome);
                    if (risultato_morra==1) {
                      if (torre_Ninja1==NULL) {
                        switch (Ninja.stato) {
                          case vulnerabile: giocatore_morto=1;
                                            vincitore=2;
                            break;
                          case scudo_vita: Ninja.stato=solo_vita;
                            break;
                          case solo_vita: Ninja.stato=vulnerabile;
                            break;
                          case solo_scudo: Ninja.stato=vulnerabile;
                            break;
                          default:
                            break;
                          }
                        }
                        else{
                          if (torre_Ninja2==NULL) {
                            free(torre_Ninja1);
                            torre_Ninja1=NULL;
                          }
                          else{
                            if (torre_Ninja3==NULL) {
                              free(torre_Ninja2);
                              torre_Ninja2=NULL;
                            }
                            else {
                              free(torre_Ninja3);
                              torre_Ninja3=NULL;
                            }
                          }
                        }
                    }
                    turno_passato_finale=1;
              break;
            default:
              break;
          }
        } while(turno_passato_finale==0);
      sleep(2);
    }
    else{
      if (contatore_turni%2==1) {
        printf("\033[1;33m");
        printf("è il turno di Ninja\n\n\n\n");
        nome=1;
        sleep(2);
          do {
            printf("\033[1;33m");
            do {
              system("clear");
              printf("Menù opzioni FINALI:\n" );
              printf("1)SPARA COLPO LANCIARAZZI\n2)STAMPA STATISTICHE\n3)MORRA CINESE\n" );
              printf("\nScegli una delle 3 opzioni di gioco.\n");
              scanf("%hu",&sceltaMenuFinale );
              if (sceltaMenuFinale<1 || sceltaMenuFinale>3) {
                printf("\033[1;31m");
                printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
                sleep(1);
                printf("\033[1;33m");
                system("clear");
              }
                else{
                  controllo=1;
                  }
            } while(controllo==0);
            switch (sceltaMenuFinale) {
              case 1: if (Ninja.zaino[colpi_lanciarazzi-1]>=1) {
                      printf("Colpo lanciarazzi\n");
                      if (torre_Ciccio1==NULL) {
                        switch (Ciccio.stato) {
                          case vulnerabile: giocatore_morto=1;
                                            vincitore=1;
                            break;
                          default: Ciccio.stato=vulnerabile;
                            break;
                          }
                        }
                        else{
                          if (torre_Ciccio2==NULL) {
                            free(torre_Ciccio1);
                            torre_Ciccio1=NULL;
                          }
                          else{
                            if (torre_Ciccio3==NULL) {
                              free(torre_Ciccio2);
                              torre_Ciccio2=NULL;
                            }
                            else {
                              free(torre_Ciccio3);
                              torre_Ciccio3=NULL;
                            }
                          }
                        }
                        Ninja.zaino[colpi_lanciarazzi-1]--;
                        turno_passato_finale=1;
                      }
                      else{
                        if (Ninja.zaino[colpi_lanciarazzi-1]<1) {
                          printf("Non hai colpi da poter sparare\n");
                        }
                      }
                      sleep(2);
                break;
              case 2:   stampa_statistiche(&Ninja, nome);
                        do {
                          printf("SCEGLI COSA FARE:\n1: USA MEDIKIT\n2: USA POZIONE\n3: TORNA INDIETRO\n");
                          scanf("%hu",&scelta_menu_zaino_finale);
                          if (scelta_menu_zaino_finale<1 || scelta_menu_zaino_finale>3) {
                            printf("\033[1;31m");
                            printf("Inserisci un'opzione tra quelle disponibili nel menù.\n" );
                            printf("\033[1;33m");
                            sleep(1);
                            system("clear");
                          }
                          else{
                            controllo_menu_zaino=1;
                          }
                        } while(controllo_menu_zaino==0);
                        if (scelta_menu_zaino_finale==1) {
                          if (Ninja.zaino[medikit-1]>=1) {
                            switch (Ninja.stato) {
                              case vulnerabile: Ninja.stato=solo_vita;
                                                Ninja.zaino[medikit-1]--;
                                                turno_passato_finale=1;
                                                stampa_statistiche(&Ninja, nome);
                                break;
                            case scudo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                              break;
                            case solo_vita: printf("Hai già la vita piena, non puoi usare il medikit.\n");
                              break;
                            case solo_scudo:  Ninja.stato=scudo_vita;
                                              Ninja.zaino[medikit-1]--;
                                              turno_passato_finale=1;
                                              stampa_statistiche(&Ninja, nome);
                              break;
                            default:
                              break;
                            }
                          }
                          else{
                            printf("Non hai nessun medikit da poter utilizzare\n");
                          }
                        }
                        else{
                          if (scelta_menu_zaino_finale==2) {
                            if (Ninja.zaino[pozione-1]>=1) {
                              switch (Ninja.stato) {
                                case vulnerabile: Ninja.stato=solo_scudo;
                                                  Ninja.zaino[pozione-1]--;
                                                  turno_passato_finale=1;
                                                  stampa_statistiche(&Ninja, nome);
                                  break;
                                case scudo_vita: printf("Hai già lo scudo, non puoi usare la pozione.\n");
                                  break;
                                case solo_vita: Ninja.stato=scudo_vita;
                                                Ninja.zaino[pozione-1]--;
                                                turno_passato_finale=1;
                                                stampa_statistiche(&Ninja, nome);
                                break;
                              case solo_scudo:  printf("Hai già lo scudo, non puoi usare la pozione.\n");
                                break;
                              default:
                                break;
                              }
                            }
                            else{
                              printf("Non hai nessuna pozione da poter utilizzare\n");
                            }
                          }
                        }
                      sleep(1);
                break;
              case 3: morra_cinese(nome);
                      if (risultato_morra==1) {
                        if (torre_Ciccio1==NULL) {
                          switch (Ciccio.stato) {
                            case vulnerabile: giocatore_morto=1;
                                              vincitore=1;
                            break;
                            case scudo_vita: Ciccio.stato=solo_vita;
                            break;
                            case solo_vita: Ciccio.stato=vulnerabile;
                            break;
                            case solo_scudo: Ciccio.stato=vulnerabile;
                            break;
                            default:
                            break;
                          }
                        }
                        else{
                          if (torre_Ciccio2==NULL) {
                            free(torre_Ciccio1);
                            torre_Ciccio1=NULL;
                          }
                          else{
                            if (torre_Ciccio3==NULL) {
                              free(torre_Ciccio2);
                              torre_Ciccio2=NULL;
                            }
                            else {
                              free(torre_Ciccio3);
                              torre_Ciccio3=NULL;
                            }
                          }
                        }
                      }
                      turno_passato_finale=1;
                break;
              default:
                break;
            }
          } while(turno_passato_finale==0);
        sleep(2);
      }
    }


    if (giocatore_morto!=1){
      printf("\033[1;35m");
      printf("\n\nTORRE DI CICCIO\n");
      printf("---\n");
      if (torre_Ciccio1==NULL) {
        printf("Non ci sono piani\n");
        }
      else{
        struct Piano *successivo1= torre_Ciccio1;
        while (successivo1!=NULL) {
          if (successivo1->numero_piano<=3 && successivo1->numero_piano>=1 ) {
            printf("|%hu|\n",successivo1->numero_piano );
            printf("---\n");
          }
          successivo1= successivo1->piano_successivo;
        }
        printf("\n");
      }
      printf("\n\n\n");
      printf("\033[1;33m");
      printf("\nTORRE DI NINJA\n");
      printf("---\n");
      if (torre_Ninja1==NULL) {
        printf("Non ci sono piani\n");
        }
      else{
        struct Piano *successivo2 = torre_Ninja1;
        while (successivo2!=NULL) {
          if (successivo2->numero_piano<=3 && successivo2->numero_piano>=1) {
            printf("|%hu|\n",successivo2->numero_piano);
            printf("---\n");
          }
          successivo2= successivo2->piano_successivo;
        }
      }
      printf("\n\n\n");
    }
    contatore_turni++;
  } while(giocatore_morto==0);
  if (giocatore_morto==1) { //riazzeramento valori nel caso in cui un giocatore muore
    free(torre_Ciccio1);
    torre_Ciccio1=NULL;
    free(torre_Ciccio2);
    torre_Ciccio2=NULL;
    free(torre_Ciccio3);
    torre_Ciccio3=NULL;
    free(torre_Ninja1);
    torre_Ninja1=NULL;
    free(torre_Ninja2);
    torre_Ninja2=NULL;
    free(torre_Ninja3);
    torre_Ninja3=NULL;
  }
}

//funzione minigioco per essere usata nella battaglia finale
static void morra_cinese(unsigned short int nome){
  risultato_morra=0; //se è uguale a 0 lo sfidante non ha vinto e non può attaccare, se è uguale a 1 ha vinto e può attaccare
  unsigned short int sceltaMorra1=0;
  unsigned short int controlloMorra1=0;
  unsigned short int sceltaMorra2=0;
  unsigned short int controlloMorra2=0;
  printf("SFIDA A MORRA CINESE...\n");
  if (nome==2) {
    printf("CICCIO SFIDA NINJA\n\n\n");
    printf("Ciccio fai la tua mossa...\n");
  }
  else{
    if (nome==1) {
      printf("NINJA SFIDA CICCIO\n\n\n");
      printf("Ninja fai la tua mossa...\n");
    }
  }
  sleep(3);
  do {
    system("clear");
    printf("Menù opzioni FINALI:\n" );
    printf("1)SASSO\n2)CARTA\n3)FORBICI\n" );
    printf("\nScegli una delle 3 mosse.\n");
    scanf("%hu",&sceltaMorra1 );
    if (sceltaMorra1<1 || sceltaMorra1>3) {
      printf("\033[1;31m");
      printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
      sleep(1);
      printf("\033[0m");
      system("clear");
    }
      else{
        controlloMorra1=1;
        }
  } while(controlloMorra1==0);
  sleep(1);
  if (nome==2) {
    printf("Ninja ora tocca a te..." );
    sleep(2);
    printf(" fai la tua mossa...\n");
  }
  else{
    if (nome==1) {
      printf("Ciccio ora tocca a te...");
      sleep(2);
      printf(" fai la tua mossa...\n");
    }
  }
  sleep(2);
  do {
    system("clear");
    printf("Menù opzioni FINALI:\n" );
    printf("1)SASSO\n2)CARTA\n3)FORBICI\n" );
    printf("\nScegli una delle 3 mosse.\n");
    scanf("%hu",&sceltaMorra2 );
    if (sceltaMorra2<1 || sceltaMorra2>3) {
      printf("\033[1;31m");
      printf("Inserisci un valore tra quelli disponibili nel menù.\n" );
      sleep(1);
      printf("\033[0m");
      system("clear");
    }
      else{
        controlloMorra2=1;
        }
  } while(controlloMorra2==0);
  sleep(1);
  switch (sceltaMorra1) {
    case 1: switch (sceltaMorra2) {
            case 1: printf("Sasso contro Sasso.\nPAREGGIO\nnon puoi eseguire l'attacco.\n");
                    risultato_morra=0;
              break;
            case 2: printf("Sasso contro Carta.\nHAI PERSO\nnon puoi attaccare.\n" );
                    risultato_morra=0;
              break;
            case 3: printf("Sasso contro Forbici.\nHAI VINTO\nPuoi eseguire l'attacco.\n");
                    risultato_morra=1;
              break;
            default:
              break;

            }
      break;
    case 2: switch (sceltaMorra2) {
            case 1: printf("Carta contro Sasso.\nHAI VINTO\npuoi eseguire l'attacco.\n");
                    risultato_morra=1;
              break;
            case 2: printf("Carta contro Carta.\n PAREGGIO\nnon puoi eseguire l'attacco.\n");
                    risultato_morra=0;
              break;
            case 3:printf("Carta contro Forbici.\nHAI PERSO\nnon puoi attaccare.\n");
                    risultato_morra=0;
              break;
            default:
              break;

            }
      break;
    case 3: switch (sceltaMorra2) {
            case 1:printf("Forbici contro Sasso.\nHAI PERSO\nnon puoi attaccare.\n");
                    risultato_morra=0;
              break;
            case 2:printf("Forbici contro Carta.\nHAI VINTO\npuoi eseguire l'attacco.\n");
                    risultato_morra=1;
              break;
            case 3:printf("Forbici contro Forbici.\nPAREGGIO\nnon puoi eseguire l'attacco.\n");
                    risultato_morra=0;
              break;
            default:
              break;

            }
      break;
    default:
      break;
  }
  sleep(3);
  controlloMorra1=0;
  controlloMorra2=0;
}
