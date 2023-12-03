#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "assets/ANSI-color-codes.h" // Dictionnaire de couleurs - credit : https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a

typedef int matrice[7][7];
int win1, win2;

// entêtes fonctions
int saisi_impair();
void attribution1(matrice a, int lin, int col);
void affichage(matrice a, int lin, int col);
void ajout(matrice a, int joueur, int lin, int col, int i, int sound);
void start(matrice a, int lin, int col, char *psd1, char *psd2, int win1, int win2, int sound);
int checkwin(matrice a, int lin, int col, int win1, int win2);
int save(int a);
int save2(int a);
//
int main()
{
    //  - Definitions des variables de base -       //
    int line, col;
    int jouer, verif;
    char pseudo1[] = "joueur1";
    char pseudo2[] = "joueur2";
    int sound = 0;

    //     Saisie des valeurs de bases
    //      - Dimensions du tableau -              //
    
    printf("Veuillez procedez a la saisie du nombre de lignes" BWHT " puis " reset "de colonnes, ces nombres doivent etres" BWHT " impaires\n" reset);
    line = saisi_impair();
    col = saisi_impair();
    printf("\n%d lignes pour %d colonnes\n", line, col);

    //      - Informations des utilisateurs -      //
    printf("Veuillez procedez a la saisie du pseudonyme pour le premier joueur " BWHT " puis " reset " pour le second joueur :\n");
    scanf("%s", pseudo1);
    scanf("%s", pseudo2);
    printf("Nous avons donc " BLU "%s" reset " et " RED "%s" reset ".\n", pseudo1, pseudo2);

    //      - Option Sonore -      //
    printf("Souhaitez vous des effets sonores durant votre(vos) partie(s) ? (1 = Oui // Autre = Non) : ");
    scanf("%d", &sound);

    //      - Initialisez notre matrice -      //
    matrice a;
    jouer = 0;
    //      - Boucle de n parties -      //
    do
    {
        attribution1(a, line, col);
        affichage(a, line, col);
        
        printf("Souhaitez-vous jouez ? (1 = Oui // Autre = Non) : ");
        scanf("%d", &verif);
        if (verif == 1)
        {
            jouer = 1;
            // Lancez une partie, a la fin de la partie l'utilisateur pourra en relancez une ou non.
            start(a, line, col, pseudo1, pseudo2, win1, win2, sound);
        }
        else
        {
            jouer = 0;
            
            //      - Fin du jeu -      //
            printf("Score final : " BLU "%d" reset " a " RED "%d" reset, win1, win2);
            Sleep(3000);
            // exit( EXIT_SUCCESS );
        }

    } while ((jouer == 1));
}

//      - Fonctions -      //

int saisi_impair()
{
    //      - Saisie d'un nombre impair compris entre 3 et 7 -      //
    int a = 0;
    printf("Entrez un nombre" BWHT " impaire " reset " compris entre 3 et 7:");
    scanf("%d", &a);
    while (a % 2 == 0 || a < 3 || a > 7)
    {
        printf("Veuillez recommencez avec un nombre " BWHT "impaire" reset " compris entre " BWHT "3 et 7 :" reset);
        scanf("%d", &a);
    }
    return a;
}

void attribution1(matrice a, int lin, int col)
{
    //      - Remplissage de notre matrice, limitée par lin et col, de 0 -      //
    int i, j;
    for (i = 0; i < lin; i++)
        for (j = 0; j < col; j++)
        {
            a[i][j] = 0;
        }
}

void affichage(matrice a, int lin, int col)
{
    //      - Affichage de notre matrice -      //
    int i, j;
    char empty[] = "-";
    char space[]= " ";
    int nb_space=(4*(col+1)-strlen("Matrice :"))/2;
    printf("\n");
printf("\n%*s" UWHT "Matrice" reset ":\n", nb_space, space);
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        if(a[i][j] == 0){
      printf("%4s",empty);
        } else if(a[i][j] < 0) {
         printf(RED "%4d" reset, abs(a[i][j]));
        } else {
          printf(BLU "%4d" reset, abs(a[i][j]));
        }
                printf("\n");
    }
}

void ajout(matrice a, int joueur, int lin, int col, int i, int sound)
{
    //      - Fonction permettant l'ajout d'un pion au tableau -      //
    int l, c;
    lin = lin - 1;
    col = col - 1;
    // On effectue ces opérations pour résoudre un bug vu que notre matrice début à [0][0] et non à [1][1] comme si on suivait la représentation graphique.
    if (joueur == 2)
    {
        i = 0 - i;

    }; // Si le joueur est le second alors ses pions sont négatifs
    
    printf("Veuillez saisir la ligne a laquelle vous deposez votre pion (Joueur %d): ", joueur);
    scanf("%d", &l);
    printf("Veuillez saisir la colonne a laquelle vous deposez votre pion (Joueur %d): ", joueur);
    scanf("%d", &c);
    //      - Vérification des données saisies -      //
    while ((l - 1 > lin) || (c - 1 > col) || (l - 1 < 0) || (c - 1 < 0) || (a[l - 1][c - 1] != 0))
    {
        printf("Veuillez saisir la ligne a laquelle vous deposez votre pion de tel sorte que celle-ci soit innoccupee : ");
        scanf("%d", &l);
        printf("Veuillez saisir la colonne a laquelle vous deposez votre pion de tel sorte que celle-ci soit innoccupee : ");
        scanf("%d", &c);
    }

    l = l - 1;
    c = c - 1;

if(sound == 1) {
    PlaySound("sounds/swoosh.wav", NULL, SND_FILENAME | SND_ASYNC); // Fonction permettant de lancer un son .wav de manière asynchrone.
}

    a[l][c] = i;
}

void start(matrice a, int lin, int col, char *psd1, char *psd2, int win1, int win2, int sound)
{
    int nombre_tours = (lin * col - 1) / 2;
    int i;
    
    //      - Pour chaque tour on efface la console et on effectue le déroulement de la partie -      //
    for (i = 1; i <= nombre_tours; i++)
    {
        system("cls");
        printf("-       Tour de " BLU "%s" reset "      -", psd1);
        affichage(a, lin, col);
        ajout(a, 1, lin, col, i, sound);
        system("cls");
        printf("-       Tour de " RED "%s" reset "      -", psd2);
        affichage(a, lin, col);
        ajout(a, 2, lin, col, i, sound);
    }
    //      - Vérification du gagnant -      //
    int x = checkwin(a, lin, col, win1, win2);
    if(sound == 1) {
    PlaySound("sounds/win.wav", NULL, SND_FILENAME | SND_ASYNC); // Fonction permettant de lancer un son .wav de manière asynchrone.
}

    if (x == 2)
    {
        printf("-   Victoire de " RED "%s" reset " (Joueur " RED "2" reset ")    -\n", psd2);
        win2 = (win2 + 1);
        Sleep(3000);
    }
    else if (x == 1)
    {
        printf("-   Victoire de " BLU "%s" reset " (Joueur " BLU "1" reset ")    -\n", psd1);
        win1 = (win1 + 1);
    }
    else if (x == 0)
    {
        printf("-   Match Nul    -\n");
    }
    //      - Affichage du total de fin de parties -      //
    printf("\nVictoires de " BLU "%s" reset " (Joueur " RED "1" reset ") : " YEL "%d" reset "\nVictoires de " RED "%s" reset " (Joueur " RED "2" reset ") : " YEL "%d" reset"\n", psd1, win1, psd2, win2);
    //      - On enregistre à part nos compteurs pour contourner la fonction void qui ne renvoie rien -      //
    save(win1);
    save2(win2);
    Sleep(2000);
}

int checkwin(matrice a, int lin, int col, int win1, int win2)
{
    int nline, ncol, i, j;
    
    //      - Obtenir la position de la case vide à la fin de la partie -      //
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        {
            if (a[i][j] == 0)
            {
                nline = i;
                ncol = j;
            }
        }
    }

    int som1, som2;
    //      - Obtenir la somme des jetons bleu et rouge séparément -      //
    for (i = nline - 1; i <= nline + 1; i++)
    {
        for (j = ncol - 1; j <= ncol + 1; j++)
        {
            if (a[i][j] > 0)
            {
                som1 = som1 + a[i][j];
            }
            else if (a[i][j] < 0)
            {
                som2 = som2 + a[i][j];
            }
        }
    }

    //      - Comparer ces sommes afin d'en tirer un vainqueur puis retourner ce vainqueur -      //

    som1 = abs(som1); // On ne garde que la valeure absolue même sur la première somme censé être positive par mesure de sécurité
    som2 = abs(som2); // On ne garde que la valeure absolue

    if (som1 > som2)
    {
        return 2;
    }
    else if (som2 > som1)
    {
        return 1;
    }
    else if (som2 == som1)
    {
        return 0;
    }
    else
        return 0;
}

int save(int a)
{
    //      - Sauvegarde de win1 -      //
    return win1 = a;
}
int save2(int a)
{
    //      - Sauvegarde de win2 -      //
    return win2 = a;
}

