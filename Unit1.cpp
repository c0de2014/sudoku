#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string>
#include <iostream.h>
#include <time.h>
using namespace std;
#define block 3
#define groesse (block*block)

int count=0, zaehl=0;                                                           // Variable um Funktionsaufrufe zu z�hlen
bool read_success;                                                              // Variable ob einlesen erfolgreich war


struct sudoku                                                                   // Datentyp um ganze Felder von Funktionen zur�ckzugeben
{
 int feld[groesse+1][groesse+1][groesse+1];                                     // [][] --> x,y --> Felder des Sudokus
};                                                                              //     [] --> hier werden f�r jedes Feld die M�glichkeiten gespeichert


void wait(int x)                                                                // wartet x hundertstel Sekunden
{
 x=(x*10000)/15625;                                                             // verkleinert x.. um auf "diesem Rechner" auf x hunderstel zu kommen..
 struct time time1, time2;                                                      //             ..dieser Rechner:  Pentium 4 2,4 GHz , 512MB RAM
 int counter=0;                                                                 // ... ... ...
 int t1,t2;
 gettime(&time1);
 t1=time1.ti_hund;
 do
 {
  gettime(&time2);
  t2=time2.ti_hund;
  if (t2!=t1) { counter++; t1=t2;}
 } while (counter<x);
}


sudoku vorbelegen(sudoku spiel)                                                 // alle Werte =0 und alle M�glichkeiten =1
{
 int x,y,z;
 for (y=0;y<=groesse;y++)                                                       // alle Zeilen durchlaufen
 {
  for (x=0;x<=groesse;x++)                                                      // alle Felder durchlaufen
  {
   spiel.feld[x][y][0]=0;                                                       // Feld als unbelegt setzen
   for (z=1;z<=groesse;z++)
   {
    spiel.feld[x][y][z]=1;                                                      // alle M�glichkeiten auf m�glich setzen
   }
  }
 }
 return spiel;                                                                  // vorbelegtes Sudoku zur�ckgeben
}


int possibilities(int x, int y, sudoku spiel)                                   // gibt die Anzahl der M�glichen Werte im Feld(x,y)
{
 int anzahl=0,z;
 if (spiel.feld[x][y][0]==0)                                                    // wenn (x,y) nicht belegt..
 {
  for (z=1;z<=groesse;z++)                                                      // alle M�glichkeiten durchlaufen
  {
   if (spiel.feld[x][y][z]==1) anzahl++;                                        // wenn z m�glich -- Anzahl +1
  }
 }
 return anzahl;                                                                 // anzahl zur�ckgeben
}


int moeglichkeiten(int x, int y, sudoku spiel)                                  // z�hlt die Anzahl der m�glichen Zahlen f�r ein Feld
{
 int anzahl=0,z,i,j;
 int moegl[groesse+1];
 int x_block, y_block;
 x_block=((((x-1)/block)*block)+1);
 y_block=((((y-1)/block)*block)+1);
 for (i=1;i<=groesse;i++) moegl[i]=1;
 for (i=1;i<=groesse;i++)
 {
  z=spiel.feld[i][y][0];
  if (z!=0) moegl[z]=0;
 }
 for (j=1;j<=groesse;j++)
 {
  z=spiel.feld[x][j][0];
  if (z!=0) moegl[z]=0;
 }
 for (i=x_block;i<=x_block+block-1;i++)
 {
  for (j=y_block;j<=y_block+block-1;j++)
  {
   z=spiel.feld[i][j][0];
   if (z!=0) moegl[z]=0;
  }
 }
 for (i=1;i<=groesse;i++)
 {
  if (moegl[i]!=0) anzahl++;
 }
 return anzahl;
}


int fertig(sudoku spiel)                                                        // fertig --> 1, nicht fertig --> 0 , Fehler --> 3
{
 int fert=1;                                                                    // Annahme: fertig
 int x,y, moegl;
 for (y=1;y<=groesse;y++)                                                       // alle Zeilen durchlaufen
 {
  for (x=1;x<=groesse;x++)                                                      // alle Felder durchlaufen
  {
   moegl=possibilities(x,y,spiel);                                              // Anzahl der M�glichkeiten nach moegl schreiben
   if (spiel.feld[x][y][0]==0 && moegl>0)                                       // wenn (x,y) "nicht belegt" und noch M�glichkeiten
   {
    fert=0;                                                                     // --> noch nicht fertig
                                                                                // hier nicht rauspringen.. da noch ein fehler enthalten sein k�nnte!!!
   }                                                                            // ... dies kann viele unn�tige Aufrufe von "loese()" sparen!!!
   else if (spiel.feld[x][y][0]==0 && moegl==0)                                 // sonst, wenn nicht belegt und keine M�glichkeiten mehr
   {
    fert=3;                                                                     // --> Fehler
    break;                                                                      // ..aus der schleife raus
   }
  }
  if (fert==3) break;                                                           // wenn Fehler ... aus der schleife raus
 }
 return fert;                                                                   // r�ckgabe von fert
}


void ausgeben(sudoku spiel)                                                     // gibt das feld aus..
{                                                                               //                    mit Rahmen
 int x,y,i;                                                                     //                    und gr�nen Sternen f�r unbelegte Felder
 printf("\xC9");                                                                // ... ... ...
 for (i=1;i<=(groesse*3);i++)
 {
  printf("\xCD");
 }
 printf("\xBB\n");
 for (y=1;y<=groesse;y++)
 {
  printf("\xBA");
  for (x=1;x<=groesse;x++)
  {
   if (spiel.feld[x][y][0]!=0) printf(" %i ",spiel.feld[x][y][0]);
   else
   {
    textcolor(2);
    cprintf(" * ");
   }
  }
  printf("\xBA\n");
  if (y==groesse)
  {
   printf("\xC8");
   for (i=1;i<=(groesse*3);i++)
   {
    printf("\xCD");
   }
   printf("\xBC\n");
  }
  else
  {
   printf("\xBA");
   for (i=1;i<=(groesse*3);i++)
   {
    printf(" ");
   }
   printf("\xBA\n");
  }
 }
 for (i=2;i<=18;i++)
 {
  gotoxy(10,i); printf("%c",179);
 }
 for (i=2;i<=18;i++)
 {
  gotoxy(19,i); printf("%c",179);
 }
 for (i=2;i<=28;i++)
 {
  gotoxy(i,7); printf("%c",196);
 }
 for (i=2;i<=28;i++)
 {
  gotoxy(i,13); printf("%c",196);
 }
 gotoxy(10,7); printf("%c",197);
 gotoxy(10,13); printf("%c",197);
 gotoxy(19,7); printf("%c",197);
 gotoxy(19,13); printf("%c",197);
}


sudoku streiche_waagrecht(int x, int y, sudoku spiel)                           // streicht den Wert von (x,y) in der entsprechenden Zeile
{
 int z=spiel.feld[x][y][0];
 if (z!=0)                                                                      // wenn (x,y) belegt..
 {
  for (x=1;x<=groesse;x++)                                                      // Zeile durchlaufen
  {
   spiel.feld[x][y][z]=0;                                                       // entsprechende M�glichkeit streichen
  }
 }
 return spiel;                                                                  // aktualisiertes Sudoku zur�ckgeben
}


sudoku streiche_senkrecht(int x, int y, sudoku spiel)                           // streicht den Wert von (x,y) in der entsprechenden Spalte
{
 int z=spiel.feld[x][y][0];
 if (z!=0)                                                                      // wenn (x,y) belegt..
 {
  for (y=1;y<=groesse;y++)                                                      // Spalte durchlaufen
  {
   spiel.feld[x][y][z]=0;                                                       // entsprechende M�glichkeit streichen
  }
 }
 return spiel;                                                                  // aktualisiertes Sudoku zur�ckgeben
}


sudoku streiche_block(int x, int y, sudoku spiel)                               // streicht den Wert von (x,y) im entsprechenden Block
{
 int x_block, y_block;
 int z=spiel.feld[x][y][0];
 if (z!=0)                                                                      // wenn (x,y) belegt..
 {
  x_block=((((x-1)/block)*block)+1);
  y_block=((((y-1)/block)*block)+1);
  for (x=x_block;x<=x_block+block-1;x++)                                        // Block durchlaufen
  {
   for (y=y_block;y<=y_block+block-1;y++)
   {
    spiel.feld[x][y][z]=0;                                                      // entsprechende M�glichkeit streichen
   }
  }
 }
 return spiel;                                                                  // aktualisiertes Sudoku zur�ckgeben
}


sudoku streiche(sudoku spiel)                                                   // l�scht alle Werte die nicht mehr m�glich sind aus den "M�glichkeiten"
{
 int x,y;
 for (y=1;y<=groesse;y++)
 {
  for (x=1;x<=groesse;x++)
  {
   spiel=streiche_waagrecht(x,y,spiel);                                         // Zeilenweise alle Felder durchgehn und Werte in der Zeile streichen
   spiel=streiche_senkrecht(x,y,spiel);                                         // Spaltenweise alle Felder durchgehn und Werte in der Spalte streichen
   spiel=streiche_block(x,y,spiel);                                             // Block in dem (x,y) liegt durchgehn und Werte in Block streichen
  }
 }
 return spiel;                                                                  // aktualisiertes Sudoku zur�ckgeben
}


sudoku set(int x, int y, int zahl, sudoku spiel)                                // schreibt zahl ins Feld(x,y) und streicht die abh�ngigen M�glichkeiten
{
 if (zahl>0 && zahl <=groesse)                                                  // ( nicht unbedingt n�tig... )
 {
  spiel.feld[x][y][0]=zahl;                                                     // Zahl ins Sudoku schreiben
  spiel=streiche(spiel);                                                        // Zahl an allen abh�ngigen Stellen im Sudoku als M�glichkeit streichen
 }
 return spiel;
}


sudoku eintragen_waagrecht(int x, int y, int zahl, sudoku spiel)                // HILFSFUNKTION f�r del()
{
 for (x=1;x<=groesse;x++)
 {
  spiel.feld[x][y][zahl]=1;
 }
 return spiel;
}


sudoku eintragen_senkrecht(int x, int y, int zahl, sudoku spiel)                // HILFSFUNKTION f�r del()
{
 for (y=1;y<=groesse;y++)
 {
  spiel.feld[x][y][zahl]=1;
 }
 return spiel;
}


sudoku eintragen_block(int x, int y, int zahl, sudoku spiel)                    // HILFSFUNKTION f�r del()
{
 int x_block, y_block;
 x_block=((((x-1)/block)*block)+1);
 y_block=((((y-1)/block)*block)+1);
 for (x=x_block;x<=x_block+block-1;x++)
 {
  for (y=y_block;y<=y_block+block-1;y++)
  {
   spiel.feld[x][y][zahl]=1;
  }
 }
 return spiel;
}


sudoku del(int x, int y, sudoku spiel)                                          // entspricht der set() funktion nur entfernt diese
{                                                                               // Funktion eine Zahl aus dem Feld
 int zahl;
 if (spiel.feld[x][y][0]>0 && spiel.feld[x][y][0] <=groesse)
 {
  zahl=spiel.feld[x][y][0];
  spiel.feld[x][y][0]=0;
  spiel=eintragen_waagrecht(x,y,zahl,spiel);
  spiel=eintragen_senkrecht(x,y,zahl,spiel);
  spiel=eintragen_block(x,y,zahl,spiel);
 }
 return spiel;
}


sudoku eingeben()                                                               // liest ein Sudoku zeilenweise von der Tastur ein.. (nicht verwendet)
{                                                                               // ... ... ...
 sudoku spiel;
 spiel=vorbelegen(spiel);
 int eingabe,x,y;
 for (y=1;y<=groesse;y++)
 {
  for (x=1;x<=groesse;x++)
  {
   scanf("%i",&eingabe);
   if (eingabe>0 && eingabe<=groesse) spiel=set(x,y,eingabe,spiel);
   else spiel=set(x,y,groesse,spiel);
  }
 }
 return spiel;                                                                  // eingelesenes Sudoku zur�ckgeben
}


sudoku einlesen(FILE *infile)                                                   // liest ein Sudoku aus einer Datei ein..
{                                                                               // ... ... ...
 int wert;
 char zeile[20];
 int x,y,i,j;
 sudoku spiel;
 string eingabe[groesse];
 spiel=vorbelegen(spiel);
 fgets(zeile,(groesse*2)+2,infile);
 if (zeile[0]=='#'&&zeile[1]=='s'&&zeile[2]=='u'&&zeile[3]=='d'                 // wenn 1. Zeile = "#sudoku"
     &&zeile[4]=='o'&&zeile[5]=='k'&&zeile[6]=='u')                             // ... ... ...
 {
  for (i=0;i<=groesse-1;i++)
  {
   fgets(zeile, (groesse*2)+2,infile);
   eingabe[i]=zeile;
   for (j=0;j<=20;j++)
   {
    zeile[j]= 32;
   }
  }
  for (y=1;y<=groesse;y++)
  {
   for (x=1;x<=2*groesse;x=x+2)
   {
    wert=eingabe[y-1][x-1]-48;
    if (wert>groesse||wert<1) wert=0;
    spiel=set(((x+1)/2),y,wert,spiel);
   }
  }
  read_success=true;                                                            // erfolgreich eingelesen
 }
 else read_success=false;                                                       // sonst ... nicht erfolgreich eingelesen
 return spiel;                                                                  // eingelesenes Sudoku zur�ckgeben
}


int eindeutige(sudoku spiel)                                                    // eindeutige Felder z�hlen
{
 int anzahl=0,i,j;
 for (i=1;i<=groesse;i++)
 {
  for (j=1;j<=groesse;j++)
  {
   if (moeglichkeiten(i,j,spiel)<2) anzahl++;
  }
 }
 return anzahl;
}


int eindeutige_unbelegt(sudoku spiel)                                           // eindeutige unbelegte Felder z�hlen
{
 int anzahl=0,i,j;
 for (i=1;i<=groesse;i++)
 {
  for (j=1;j<=groesse;j++)
  {
   if ((moeglichkeiten(i,j,spiel)==1)&&(spiel.feld[i][j][0]==0)) anzahl++;
  }
 }
 return anzahl;
}


void schreiben(char datei[50], sudoku spiel)                                    // schreibt ein Sudoku in eine Datei
{
 FILE *file;
 int x,y;
 if ((file=fopen(datei,"wt"))!=NULL)
 {
  fprintf(file, "#sudoku\n");
  for (y=1;y<=groesse;y++)
  {
   for (x=1;x<groesse;x++)
   {
    if (spiel.feld[x][y][0]!=0)
     fprintf(file, "%i ",spiel.feld[x][y][0]);
    else
     fprintf(file, "  ");
   }
   if (spiel.feld[x][y][0]!=0)
    fprintf(file, "%i\n",spiel.feld[x][y][0]);
   else
    fprintf(file, " \n");
  }
  fclose(file);
 }
}


bool gueltig(sudoku spiel)                                                      // gibt true wenn kein Fehler im Sudoku
{
 int zahlen[10];                                                                // Array um m�gliche Werte zu speichern
 int x,y,block_x,block_y,wert;
 bool gut=true;                                                                 // Annahme: Das Sudoku ist g�ltig.. wenn Fehler ---> ung�ltig
 for (y=1;y<=9;y++)                                                             // waagrecht pr�fen ( alle Zeilen durchlaufen )
 {
  zahlen[1]=1;                                                                  // f�r jede Zeile alle Zahlen auf m�glich setzen
  zahlen[2]=1;
  zahlen[3]=1;
  zahlen[4]=1;
  zahlen[5]=1;
  zahlen[6]=1;
  zahlen[7]=1;
  zahlen[8]=1;
  zahlen[9]=1;
  for (x=1;x<=9;x++)                                                            // Zeile durchlaufen
  {
   if (spiel.feld[x][y][0]>0)                                                   // wenn Feld belegt..
   {
    wert=spiel.feld[x][y][0];                                                   // Wert des Feldes nach wert schreiben
    if (zahlen[wert]==0) gut=false;                                             // wenn dieser Wert schon "verbraucht" --> Fehler "merken"
    else zahlen[wert]=0;                                                        // sonst diesen Wert als "verbraucht" markieren
   }
   if (!gut) break;                                                             // wenn Fehler --> abbruch
  }
  if (!gut) break;                                                              // wenn Fehler --> abbruch
 }
 if (gut)                                                                       // wenn noch kein Fehler gefunden..
 {
  for (x=1;x<=9;x++)                                                            // senkrecht pr�fen ( alle Spalten durchlaufen )
  {
   zahlen[1]=1;                                                                 // f�r jede Spalte alle Zahlen auf m�glich setzen
   zahlen[2]=1;
   zahlen[3]=1;
   zahlen[4]=1;
   zahlen[5]=1;
   zahlen[6]=1;
   zahlen[7]=1;
   zahlen[8]=1;
   zahlen[9]=1;
   for (y=1;y<=9;y++)
   {
    if (spiel.feld[x][y][0]>0)                                                  // wenn Feld belegt..
    {
     wert=spiel.feld[x][y][0];                                                  // Wert des Feldes nach wert schreiben
     if (zahlen[wert]==0) gut=false;                                            // wenn dieser Wert schon "verbraucht" --> Fehler "merken"
     else zahlen[wert]=0;                                                       // sonst diesen Wert als "verbraucht" markieren
    }
    if (!gut) break;                                                            // wenn Fehler --> abbruch
   }
   if (!gut) break;                                                             // wenn Fehler --> abbruch
  }
 }
 if (gut)                                                                       // wenn noch kein Fehler gefunden..
 {
  for (block_y=1;block_y<=7;block_y=block_y+3)                                  // bl�cke pr�fen ( alle Bl�cke durchlaufen )
  {
   for (block_x=1;block_x<=7;block_x=block_x+3)
   {
    zahlen[1]=1;                                                                // f�r jeden Block alle Zahlen m�glich setzen
    zahlen[2]=1;
    zahlen[3]=1;
    zahlen[4]=1;
    zahlen[5]=1;
    zahlen[6]=1;
    zahlen[7]=1;
    zahlen[8]=1;
    zahlen[9]=1;
    for (y=0;y<=2;y++)                                                          // Block durchlaufen
    {
     for (x=0;x<=2;x++)
     {
      if (spiel.feld[x+block_x][y+block_y][0]>0)                                // wenn Feld belegt..
      {
       wert=spiel.feld[x+block_x][y+block_y][0];                                // Wert des Feldes nach wert schreiben
       if (zahlen[wert]==0) gut=false;                                          // wenn dieser Wert schon "verbraucht" --> Fehler "merken"
       else zahlen[wert]=0;                                                     // sonst diesen Wert als "verbraucht" markieren
      }
      if (!gut) break;                                                          // wenn Fehler --> abbruch
     }
     if (!gut) break;                                                           // wenn Fehler --> abbruch
    }
    if (!gut) break;                                                            // wenn Fehler --> abbruch
   }
   if (!gut) break;                                                             // wenn Fehler --> abbruch
  }
 }
 return gut;                                                                    // R�ckgabe ob Fehler oder nicht..
}


sudoku loese_schnell(sudoku spiel)                                              // loest �bergebenes Feld und gibt die L�sung zur�ck
{
 int x, y, z;
 int found_x=groesse+1, found_y=groesse+1;
 int anzahl, found_anzahl=groesse+1;                                            // found_anzahl gr��er als max M�glichkeiten
 int ready;
 count++;                                                                       // Aufrufz�hler +1
 if (gueltig(spiel))                                                            // wenn das Sudoku keinen Fehler enth�lt..
 {
  sudoku neu;                                                                   // neues Feld um alle M�glichkeiten aufrufenzuk�nnen
  ready=fertig(spiel);
  if (ready==1) return spiel;                                                   // wenn fertig --> L�sung zur�ckgeben (Abbruch!)..
  else if (ready==0)
  {
   for (y=1;y<=groesse;y++)                                                     // n�chstes Feld mit kleinster Anzahl M�glichkeiten suchen
   {
    for (x=1;x<=groesse;x++)
    {
     anzahl=possibilities(x,y,spiel);                                           // Anzahl der M�glichkeiten nach anzahl schreiben
     if (anzahl>=1 && anzahl<found_anzahl)                                      // wenn feld (x,y) weniger M�glichkeiten
     {
      found_anzahl=anzahl;                                                      // found_anzahl aktualisieren
      found_x=x;                                                                // found_x --> X-Koordinate des gefundenen Feldes
      found_y=y;                                                                // found_y --> Y-Koordinate des gefundenen Feldes
     }
    }
   }
   for (z=1;z<=groesse;z++)                                                     // n�chste m�gliche Zahl suchen
   {
    if (spiel.feld[found_x][found_y][z]==1)                                     // wenn Zahl m�glich..
    {
     neu = loese_schnell(set(found_x,found_y,z,spiel));        // REKURSION!!!  // --> gefundene Zahl schreiben und weiterloesen..... TRICKY!!!!!!!!!!!!!
     ready=fertig(neu);                                                         // pr�fen ob jetzt fertig
     if (ready==1) return neu;                                                  // wenn fertig --> L�sung zur�ckgeben..  nach RETURN folgt abbruch!!
    }
   }
  }
 }
 return spiel;                                                                  // wenn diese Zeile erreicht wird, war ein fehler im sudoku
}

sudoku loese(sudoku spiel)                                                      // loest �bergebenes Feld und gibt die L�sung zur�ck
{
 int x, y, z;
 int found_x=groesse+1, found_y=groesse+1;
 int anzahl, found_anzahl=groesse+1;                                            // found_anzahl gr��er als max M�glichkeiten
 int ready;
 count++;                                                                       // Aufrufz�hler +1
 wait(10);                                                                      // warten... um die Ver�nderungen "zu sehen"
 gotoxy(1,1);                                                                   // Cursor an Position (1,23)
 ausgeben(spiel);                                                               // �bergebenes Sudoku ausgeben
 if (gueltig(spiel))                                                            // wenn das Sudoku keinen Fehler enth�lt..
 {
  sudoku neu;                                                                   // neues Feld um alle M�glichkeiten aufrufenzuk�nnen
  ready=fertig(spiel);
  if (ready==1) return spiel;                                                   // wenn fertig --> L�sung zur�ckgeben (Abbruch!)..
  else if (ready==0)
  {
   for (y=1;y<=groesse;y++)                                                     // n�chstes Feld mit kleinster Anzahl M�glichkeiten suchen
   {
    for (x=1;x<=groesse;x++)
    {
     anzahl=possibilities(x,y,spiel);                                           // Anzahl der M�glichkeiten nach anzahl schreiben
     if (anzahl>=1 && anzahl<found_anzahl)                                      // wenn feld (x,y) weniger M�glichkeiten
     {
      found_anzahl=anzahl;                                                      // found_anzahl aktualisieren
      found_x=x;                                                                // found_x --> X-Koordinate des gefundenen Feldes
      found_y=y;                                                                // found_y --> Y-Koordinate des gefundenen Feldes
     }
    }
   }
   for (z=1;z<=groesse;z++)                                                     // n�chste m�gliche Zahl suchen
   {
    if (spiel.feld[found_x][found_y][z]==1)                                     // wenn Zahl m�glich..
    {
     neu = loese(set(found_x,found_y,z,spiel));                // REKURSION!!!  // --> gefundene Zahl schreiben und weiterloesen..... TRICKY!!!!!!!!!!!!!
     ready=fertig(neu);                                                         // pr�fen ob jetzt fertig
     if (ready==1) return neu;                                                  // wenn fertig --> L�sung zur�ckgeben..  nach RETURN folgt abbruch!!
    }
   }
  }
 }
 return spiel;                                                                  // wenn diese Zeile erreicht wird, war ein fehler im sudoku
}


sudoku rnd_belegen(sudoku spiel)                                                // belegt 9 zuf�llige Felder mit 1..9
{
 int x, y, zahl;
 for (zahl=1;zahl<=groesse;zahl++)
 {
  do
  {
   x=random(groesse)+1;
   y=random(groesse)+1;
  } while (spiel.feld[x][y][0]!=0);
  spiel=set(x,y,zahl,spiel);
 }
 return spiel;
}


sudoku create_sudoku(sudoku spiel,int begin,int max)                            // HILFSFUNKTION um zu lange Berechnungszeiten zu
{                                                                               // vermeiden!! bricht nach zulanger Dauer ab!!
 int x,y,wert;
 if (begin<time(NULL)) return spiel;
 if (eindeutige(spiel)<=max)
 {
  return spiel;
 }
 else
 {
  for (y=1;y<=groesse;y++)
  {
   for (x=1;x<=groesse;x++)
   {
    wert=spiel.feld[x][y][0];
    if (wert!=0)
    {
     spiel=del(x,y,spiel);
     if ((moeglichkeiten(x,y,spiel)>1)||eindeutige(spiel)<=max-10)
     {
      spiel=set(x,y,wert,spiel);
     }
     else
     {
      spiel=create_sudoku(spiel, begin, max);
     }
    }
   }
  }
 }
 return spiel;
}


sudoku create(sudoku spiel,int level)                                           // DIESE FUNKTION zum Sudoku erzeugen benutzen!!
{                                                                               // Level 0 - 4
 int max;
 level=level*2+1;
 max=81-((level-1)*9)+5;
 do
 {
  int x,y,x_block,y_block,i=1;
  spiel=vorbelegen(spiel);                                                      // Feld vorbelegen
  spiel=rnd_belegen(spiel);                                                     // 1..9 zuf�llig verteilen
  spiel=loese_schnell(spiel);                                                   // daraus ein komplett gef�lltes Sudoku erzeugen
  for (y_block=0;y_block<=6;y_block=y_block+3)                                  // wieder 1 aus dem ersten Block l�schen
  {                                                                             // 2 aus dem zweiten Block
   for (x_block=0;x_block<=6;x_block=x_block+3)                                 // .. bis 9 aus dem neunten Block
   {
    for (y=1;y<=block;y++)
    {
     for (x=1;x<=block;x++)
     {
      if (spiel.feld[x_block+x][y_block+y][0]==i)
      {
       spiel=del(x_block+x,y_block+y,spiel);
       i++;
      }
     if (spiel.feld[x_block+x][y_block+y][0]==0) break;
     }
    if (spiel.feld[x_block+x][y_block+y][0]==0) break;
    }
   }
  }                                                                             // Sudoku hat jetzt 9 L�cken
  spiel=create_sudoku(spiel,time(NULL),max);                                    // noch weiter eindeutig l�sbare Zahlen l�schen
 } while ((eindeutige(spiel)>=max)||(eindeutige(spiel)<=max-10));
 return spiel;                                                                  // erzeugtes Sudoku zur�ckgeben
}


void solvermain()                                                               // Sudoku_Solver Rumpf
{
 sudoku spiel;                                                                  // Spielfeld
 char datei[50];                                                                // Variable f�r Dateinamen
 FILE *file;                                                                    // FILE-Zeiger
 char ende;
 do
 {
  count=0;                                                                      // Anzahl der Aufrufe =0
  printf("Sudokudatei angeben: ");
  fflush(stdin);
  scanf("%s",datei);                                                            // Dateiname einlesen
  if ((file=fopen(datei,"rb"))!=NULL)                                           // wenn Datei existiert.. "Datei �ffnen"
  {
   spiel=einlesen(file);                                                        // Sudoku aus Datei einlesen
   fclose(file);                                                                // Datei schliessen
   if (read_success)                                                            // wenn erfolgreich eingelesen..
   {
    clrscr();
    gotoxy(1,1);
    ausgeben(spiel);                                                            // Startsudoku ausgeben
    gotoxy (4,21);
    printf("Taste druecken...  \n");
    getch();
    clrscr();
    gotoxy(4,21);
    printf("Berechne Loesung...\n");

    spiel=loese(spiel);                                                         // Aufruf der L�sungsfunktion
    gotoxy(4,23);
    if (fertig(spiel)!=1) printf("nicht loesbares Sudoku..\n");
    else printf("Erfolgreich geloest...\n",count);
   }
   else printf("Datei enthaelt falsche Daten!\n");                              // sonst.. Fehler beim Einlesen
  }
  else cout << "Datei '" << datei << "' kann nicht geoeffnet werden!\n\n";      // else Zweig, falls Datei �ffnen nicht erfolgreich war
  gotoxy(4,25);
  printf("Beenden (j/J)?");
  ende=getch();
  clrscr();
 }
 while (ende!='j'&&ende!='J');                                                  // Nochmal??
}


void spielmain()                                                                // MAIN_FUNKTION
{
 int start_time, end_time;
 sudoku spiel, original;
 int x=1,y=1,level,zeit,stunden,minuten,sekunden;
 char input;
 char dateiname[50];
 bool assistent=false;
 clrscr();
 do
 {
  cout << "Datei laden? (j/J): ";                                               // Datei laden? Dialog
  input=getche();
  if ((input=='j')||(input=='J'))
  {
   FILE *file;
   cout << "\nDateiname: ";
   cin >> dateiname;
   if ((file=fopen(dateiname,"rb"))!=NULL)                                      // wenn Datei existiert.. "Datei �ffnen"
   {
    spiel=einlesen(file);                                                       // Sudoku aus Datei einlesen
    fclose(file);                                                               // Datei schliessen
    if (read_success)                                                           // wenn erfolgreich eingelesen..
    {
     if (fertig(loese_schnell(spiel))!=1)
     {
      printf("nicht loesbares Sudoku..\n\n");
      read_success=false;
     }
    }
    else printf("Datei enthaelt falsche Daten!\n\n");                           // sonst.. Fehler beim Einlesen
   }
   else
   {
    cout << "Datei '" << dateiname << "' kann nicht geoeffnet werden!\n\n";     // else Zweig, falls Datei �ffnen nicht erfolgreich war
   }
  }
 } while ((!read_success)&&((input=='j')||(input=='J')));
 if ((input!='j')&&(input!='J'))                                                // wenn keine Datei laden.. Level ausw�hlen
 {
  do
  {
   cout << "\n\nSchwierigkeitsstufe waehlen:\n";
   cout << "[0 = Kindergarten]  [1 = leicht]  [2 und 3 = mittel]  [4 = schwer]\n";
   cout << "Level: ";
   level=getche();
  } while ((level > 4+48)||(level < 0+48));

  level=level-48;

  cout << "\n\n\nBitte warten...";
  spiel=create(spiel,level);
 }
 original=spiel;                                                                // ab hier ..Spielablauf
 gotoxy(1,1);
 start_time=time(NULL);
 clrscr();
 ausgeben(spiel);
 gotoxy(34,6);                                                                  // Spielanleitung ausgeben
 printf("%c %c %c %c :\tCursor bewegen",24,25,26,27);
 gotoxy(34,8);
 printf("1 - 9 :\tZahl schreiben");
 gotoxy(34,10);
 printf("Leertaste :\tFeld loeschen");
 gotoxy(34,12);
 printf("ESC :\t\tBeenden");
 gotoxy(34,14);
 printf("h :\t\tAssistent ist aus");
 gotoxy(34,2);
 if ((input!='j')&&(input!='J')) printf("Level:\t\t%i", level);
 else cout << "Datei:\t\t" << dateiname;
 textcolor(2);
 do
 {
  gotoxy(40,17);
  printf("                      ");
  if (assistent)                                                                // Assistenteninformation anzeigen oder nicht
  {
   gotoxy(40,17);
   if (gueltig(spiel))
   {
    printf("Kein Fehler gefunden! ", eindeutige_unbelegt(spiel));
   }
   else
   {
    printf("Mindestens ein Fehler!", eindeutige_unbelegt(spiel));
   }
  }
  gotoxy(x*3,y*2);
  input=getche();                                                               // Taste einlesen
  gotoxy(x*3,y*2);                                                              // Cursor zur�cksetzen
  if ((original.feld[x][y][0]==0)&&(input>=1+48)&&(input<=9+48))                // Zahl eintragen
  {
   spiel=set(x,y,input-48,spiel);
  }
  else if (input=='h')                                                          // Assistent umschalten
  {
   assistent=(assistent+1)%2;
   gotoxy(59,14);
   if (assistent)
   {
    printf("ist an ");
   }
   else
   {
    printf("ist aus");
   }
   gotoxy(x*3,y*2);                                                             // gelesene Taste wieder �berschreiben
   if (original.feld[x][y][0]==0)
   {
    if (spiel.feld[x][y][0]==0) cprintf("*");
    else cprintf("%i",spiel.feld[x][y][0]);
   }
   else
   {
    if (spiel.feld[x][y][0]==0) printf("*");
    else printf("%i",spiel.feld[x][y][0]);
   }
  }
  else if ((original.feld[x][y][0]==0)&&(input==32))                            // Zahl l�schen
  {
   spiel=del(x,y,spiel);
   cprintf("*");                                                                // gelesene Taste wieder �berschreiben
  }
  else if ((input==77)||(input==75)||(input==80)||(input==72))                  // Cursor bewegen
  {
   if (original.feld[x][y][0]==0)                                               // gelesene Taste wieder �berschreiben
   {
    if (spiel.feld[x][y][0]==0) cprintf("*");
    else cprintf("%i",spiel.feld[x][y][0]);
   }
   else
   {
    if (spiel.feld[x][y][0]==0) printf("*");
    else printf("%i",spiel.feld[x][y][0]);
   }
   if ((input==77)&&(x<9)) x++;                                                 // Feld wechseln
   if ((input==75)&&(x>1)) x--;                                                 // Feld wechseln
   if ((input==80)&&(y<9)) y++;                                                 // Feld wechseln
   if ((input==72)&&(y>1)) y--;                                                 // Feld wechseln
  }
  else                                                                          //sonst gelesene Taste wieder �berschreiben
  {
   if (original.feld[x][y][0]==0)
   {
    if (spiel.feld[x][y][0]==0) cprintf("*");
    else cprintf("%i",spiel.feld[x][y][0]);
   }
   else
   {
    if (spiel.feld[x][y][0]==0) printf("*");
    else printf("%i",spiel.feld[x][y][0]);
   }
  }
  if ((fertig(spiel)==1)&&(gueltig(spiel))) break;                              // Wenn alle Felder und kein Fehler
 } while (input!=27);                                                           // Wenn ESC gedr�ckt wurde aus Schleife raus
 gotoxy(4,21);
 if (input==27) printf("Spiel abgebrochen..");
 else
 {
  end_time=time(NULL);                                                          // ben�tigte Zeit ausrechnen
  zeit=end_time-start_time;
  stunden=zeit/3600;
  minuten=(zeit%3600)/60;
  sekunden=(zeit%3600)%60;
  printf("Sudoku geloest!");
  gotoxy(4,23);
  printf("Benoetigte Zeit: %i:%i:%i", stunden, minuten, sekunden);
 }
 getche();
}

void creatormain()                                                              // Sudoku Creator Rumpf
{
 sudoku spiel;
 char dateiname[50];
 char filename[10];
 int i=0,max,level=1;
 cout << "Dateiname: ";                                                         // Dateinamen Prototyp einlesen
 cin >> filename;
 cout << "\n[1 = leicht   4 = schwer]\n";                                       // Schwierigkeit einlesen
 cout << "Schwierigkeit: ";
 cin >> level;
 if (level > 4) level = 4;
 else if (level < 1) level = 1;
 cout << "\nAnzahl: ";                                                          // Anzahl einlesen
 cin >> max;
 cout << "\n\n\nBitte Warten";
 do
 {
  spiel=create(spiel,level);                                                    // Sudoku erzeugen
  sprintf(dateiname, "%s%i.sud", filename, ++i);                                // Dateinamen erzeugen ( Prototyp + Zahl + ".sud" )
  gotoxy(1,1);
  ausgeben(spiel);                                                              // Sudoku + Informationen anzeigen
  gotoxy(32,1);
  printf("Dateiname:\t\t%s",dateiname);
  gotoxy(32,3);
  printf("Anzahl:\t\t\t%i von %i\t[%.0f%]",i,max,i/(max/100.0));
  gotoxy(32,5);
  printf("Schwierigkeit:\t\t%i", level);
  gotoxy(32,7);
  printf("Eindeutige Felder:\t  ");
  gotoxy(32,7);
  printf("Eindeutige Felder:\t%i", eindeutige_unbelegt(spiel));
  schreiben(dateiname,spiel);                                                   // Sudoku in Datei speichern
 } while (i<max);
 gotoxy(1,24);
 printf("Berechnung abgeschlossen..");
 getche();
}


main()                                                                          // MAIN --> nur noch das Startmenue!!
{
 randomize();
 int wahl;
 do
 {
  clrscr();
  cout << "1   - Creator\n";
  cout << "2   - Solver\n";
  cout << "3   - Sudoku spielen\n";
  cout << "ESC - EXIT\n";
  cout << "\nBitte waehlen: ";
  wahl=getche();
  clrscr();
  textcolor(0);
  switch (wahl)
  {
   case 1+48: creatormain();
           break;
   case 2+48: solvermain();
           break;
   case 3+48: spielmain();
           break;
   default: break;
  }
 }while (wahl!=27);
}
