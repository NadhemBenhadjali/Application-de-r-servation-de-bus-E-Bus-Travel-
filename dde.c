#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_NOM_LENGTH 50
#define MAX_MDP_LENGTH 20
#define MAX_DEST_LENGTH 50
#define MAX_BUS_NUMBER 10
#define MAX_DEST_LENGTH 50
#define MAX_HORAIRES_LENGTH 10
struct Voyageur {
    int id;
    char nom[MAX_NOM_LENGTH];
    char prenom[MAX_NOM_LENGTH];
    char motDePasse[MAX_MDP_LENGTH];
};
void creerCompte() {
    struct Voyageur nouveauVoyageur;

    printf("Entrez votre nom de famille : ");
    scanf(" %[^\n]", nouveauVoyageur.nom);

    printf("Entrez votre prénom : ");
    scanf(" %[^\n]", nouveauVoyageur.prenom);

    printf("Choisissez un mot de passe : ");
    scanf(" %[^\n]", nouveauVoyageur.motDePasse);

    FILE *fichierVoyageurs = fopen("voyageurs.txt", "a");
    FILE *fichierID = fopen("voyageur_id.txt", "r+");

    int dernierIDVoyageur = 0;

    if (fichierID != NULL) {
        fscanf(fichierID, "%d", &dernierIDVoyageur);
        fclose(fichierID);
    }

    if (fichierVoyageurs == NULL) {
        printf("Impossible d'ouvrir le fichier des voyageurs.\n");
        return;
    }

    fprintf(fichierVoyageurs, "%d %s %s %s %d\n", dernierIDVoyageur + 1, nouveauVoyageur.nom,
            nouveauVoyageur.prenom, nouveauVoyageur.motDePasse);

    dernierIDVoyageur++;

    printf("Votre identifiant est : %d\n", dernierIDVoyageur);

    fclose(fichierVoyageurs);

    fichierID = fopen("voyageur_id.txt", "w");
    if (fichierID != NULL) {
        fprintf(fichierID, "%d", dernierIDVoyageur);
        fclose(fichierID);
    }
}


void Reserver_un_voyage() {
    char destination[MAX_DEST_LENGTH];
    int voyageurID = 0;
    int resID = 0;
    struct Voyageur nouveauVoyageur;


    int passwordMatches = 0;

    while (!passwordMatches) {
        char mdp[MAX_MDP_LENGTH];
        printf("Entrez votre ID voyageur : ");
        scanf("%d", &voyageurID);
        printf("Entrez votre mot de passe : ");
        scanf(" %[^\n]", mdp);

        FILE *fichierVoyageurs = fopen("voyageurs.txt", "r");
        if (fichierVoyageurs == NULL) {
            printf("Erreur lors de l'ouverture du fichier des voyageurs.\n");
            return;
        }

        int found = 0;

        while (fscanf(fichierVoyageurs, "%d %s %s %s", &nouveauVoyageur.id, nouveauVoyageur.nom,
                      nouveauVoyageur.prenom, nouveauVoyageur.motDePasse) != EOF) {
            if (nouveauVoyageur.id == voyageurID && strcmp(nouveauVoyageur.motDePasse, mdp) == 0) {
                found = 1;
                break;
            }
        }

        fclose(fichierVoyageurs);

        if (!found) {
            printf("ID voyageur ou mot de passe incorrect. Veuillez réessayer.\n");
        } else {
            passwordMatches = 1;
        }
    }

    printf("Entrez votre destination : ");
    scanf(" %[^\n]", destination);

    FILE *idFile = fopen("res_id.txt", "r");

    if (idFile != NULL) {
        fscanf(idFile, "%d", &resID);
        fclose(idFile);
    }

    FILE *filePointer = fopen("reservations.txt", "a");

    if (filePointer == NULL) {
        printf("Le fichier ne peut pas être ouvert.\n");
        return;
    }

    fprintf(filePointer, "%d %d %s\n", resID + 1, voyageurID, destination);

    resID++;

    fclose(filePointer);
    printf("Réservation pour : %s %s à la destination : %s\n", nouveauVoyageur.nom, nouveauVoyageur.prenom, destination);
}
void detailsbus() {
    FILE *filePointer;
    filePointer = fopen("buses.txt", "r");
    if (filePointer == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return;
    }
    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), filePointer) != NULL) {
        i++;
        printf(line);
    }
    fclose(filePointer);
}
void informationsBus(int busID) {
    FILE *busFile = fopen("buses.txt", "r");
    if (busFile == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier des bus.\n");
        return;
    }
    int currentID = 0;
    char line[100];
    while (fgets(line, sizeof(line), busFile) != NULL) {
        if (strstr(line, "Bus ") != NULL) {
            currentID++;
            if (currentID == busID) {
                printf("Informations pour Bus %d:\n", busID);
                while (fgets(line, sizeof(line), busFile) != NULL && strcmp(line, "\n") != 0) {
                    printf("%s", line);
                }
                break;
            }
        }
    }

    fclose(busFile);
}
void afficherReservations(int voyageurID) {
    FILE *resFile = fopen("reservations.txt", "r");
    if (resFile == NULL) {
        fprintf(stderr, "Error opening reservations file.\n");
        return; // Quit the function or return a value to handle the error
    }

    printf("Liste des réservations :\n");
    int resID, voyID;
    char dest[50]; // Increase the size of the destination array to handle longer strings

    while (fscanf(resFile, "%d %d %[^\n]", &resID, &voyID, dest) == 3) {
        printf("ID de la réservation : %d, ID du voyageur : %d, Destination : %s\n", resID, voyID, dest);
    }

    fclose(resFile);

    // Allow the user to select a reservation to modify
    int selectedResID;
    printf("Entrez l'ID de la réservation à modifier : ");
    scanf("%d", &selectedResID);

    resFile = fopen("reservations.txt", "r+");
    if (resFile == NULL) {
        fprintf(stderr, "Error opening reservations file for modification.\n");
        return; // Quit the function or return a value to handle the error
    }

    FILE *tempFile = fopen("temp_reservations.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Error creating temporary file.\n");
        fclose(resFile);
        return; // Quit the function or return a value to handle the error
    }

    rewind(resFile);

    while (fscanf(resFile, "%d %d %[^\n]", &resID, &voyID, dest) == 3) {
        if (resID == selectedResID) {
            printf("Modifiez la destination : ");
            scanf(" %[^\n]", dest); // Correct the input format specifier to read strings

            // Write the modified reservation data to the temporary file
            fprintf(tempFile, "%d %d %s\n", resID, voyID, dest);
        } else {
            fprintf(tempFile, "%d %d %s\n", resID, voyID, dest);
        }
    }

    fclose(resFile);
    fclose(tempFile);

    if (remove("reservations.txt") != 0) {
        fprintf(stderr, "Error removing original reservations file.\n");
        return; // Quit the function or return a value to handle the error
    }

    if (rename("temp_reservations.txt", "reservations.txt") != 0) {
        fprintf(stderr, "Error renaming temporary file.\n");
        return; // Quit the function or return a value to handle the error
    }
}


int main() {
    int numVoyageurs = 0;
    int choix = 0;

    while (choix != 6) {
        printf("\nOptions disponibles :\n");
        printf("1. Créer un compte voyageur\n");
        printf("2. Réserver un voyage\n");
        printf("3. Afficher les détails des bus\n");
        printf("4. Afficher les réservations\n");
        printf("5. Informations sur le départ du bus\n");
        printf("6. Quitter\n");
        printf("Veuillez choisir une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                creerCompte();
                break;
            case 2:
                Reserver_un_voyage();
                break;
            case 3:
                detailsbus();
                break;
            case 4:
                printf("Entrez l'ID du voyageur pour afficher les réservations : ");
                int a;
                scanf("%d", &a);
                afficherReservations(a);
                break;
            case 5:
                printf("Entrez l'ID du bus pour obtenir ses informations : ");
                int selectedBusID;
                scanf("%d", &selectedBusID);
                informationsBus(selectedBusID);
                break;
            case 6:
                printf("Merci d'avoir utilisé notre service de réservation de bus. Au revoir !\n");
                break;
            default:
                printf("Choix non valide. Veuillez choisir une option valide.\n");
                break;
        }
    }
    return 0;
}
