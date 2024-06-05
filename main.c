#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 200

// Structure du Livre
typedef struct {
    char *titre;
    char *auteur;
    int annee;
    int ISBN;
    int quantite;
} Livre;

// Structure de l'Emprunteur
typedef struct {
    char *nom;
    int id;
} Emprunteur;

// Structure de l'Emprunt
typedef struct {
    Livre *livre;
    Emprunteur *emprunteur;
    char *dateEmprunt;
    char *dateRetour;
} Emprunt;

// Listes simplement chaînées
typedef struct liste_livres {
    Livre *livre;
    struct liste_livres *next;
} liste_livres;

typedef struct liste_emprunteurs {
    Emprunteur *emprunteur;
    struct liste_emprunteurs *next;
} liste_emprunteurs;

typedef struct liste_emprunts {
    Emprunt *emprunt;
    struct liste_emprunts *next;
} liste_emprunts;

// Fonction pour initialiser les listes chaînées
void *new_struct(void) {
    return NULL;
}

// Fonction pour vérifier si les listes sont vides
bool est_vide_livres(liste_livres *l) { return l == NULL; }
bool est_vide_emprunts(liste_emprunts *l) { return l == NULL; }
bool est_vide_emprunteurs(liste_emprunteurs *l) { return l == NULL; }

// Fonction pour ajouter un livre
void ajouter_livres(liste_livres **l, char *titre, char *auteur, int annee, int ISBN, int quantite) {
    liste_livres *p = (liste_livres *)malloc(sizeof(liste_livres));
    if (p == NULL) {
        fprintf(stderr, "Un problème d'allocation de la mémoire.\n");
        exit(EXIT_FAILURE);
    }
    p->livre = (Livre *)malloc(sizeof(Livre));
    if (p->livre == NULL) {
        fprintf(stderr, "Un problème d'allocation de la mémoire pour le livre.\n");
        exit(EXIT_FAILURE);
    }
    p->livre->titre = strdup(titre);
    p->livre->auteur = strdup(auteur);
    p->livre->annee = annee;
    p->livre->ISBN = ISBN;
    p->livre->quantite = quantite;
    p->next = *l;
    *l = p;
}

// Fonction pour ajouter plusieurs livres
void ajouter_n_livres(liste_livres **l) {
    int nbr_livre;
    char Auteur[MAX_SIZE], Titre[MAX_SIZE];
    int Annee, ISBN, Quantite;

    printf("Combien de livres souhaitez-vous ajouter? ");
    scanf("%d", &nbr_livre);

    for (int i = 0; i < nbr_livre; i++) {
        printf("Titre du livre %d: ", i + 1);
        scanf(" %[^\n]%*c", Titre);
        printf("Auteur du livre %d: ", i + 1);
        scanf(" %[^\n]%*c", Auteur);
        printf("Annee de publication du livre %d: ", i + 1);
        scanf("%d", &Annee);
        printf("ISBN du livre %d: ", i + 1);
        scanf("%d", &ISBN);
        printf("Quantite du livre %d: ", i + 1);
        scanf("%d", &Quantite);

        ajouter_livres(l, Titre, Auteur, Annee, ISBN, Quantite);
    }
}

// Fonction pour afficher la liste des livres
void afficher_ls_livres(liste_livres *l) {
    if (est_vide_livres(l)) {
        printf("Aucun livre n'existe dans la liste des livres.\n");
    } else {
        liste_livres *tmp = l;
        printf("Les livres dans la bibliothèque sont :\n");
        while (tmp != NULL) {
            printf("\tTitre : %s\n\tAuteur : %s\n\tAnnee : %d\n\tISBN : %d\n\tQuantite : %d\n\n",
                   tmp->livre->titre, tmp->livre->auteur, tmp->livre->annee, tmp->livre->ISBN, tmp->livre->quantite);
            tmp = tmp->next;
        }
    }
}

// Fonction pour supprimer un livre
void supprimer_livre(liste_livres **l, int ISBN) {
    liste_livres *tmp = *l, *prev = NULL;

    if (tmp != NULL && tmp->livre->ISBN == ISBN) {
        *l = tmp->next;
        free(tmp->livre->titre);
        free(tmp->livre->auteur);
        free(tmp->livre);
        free(tmp);
        printf("Livre avec ISBN %d supprimé.\n", ISBN);
        return;
    }

    while (tmp != NULL && tmp->livre->ISBN != ISBN) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    prev->next = tmp->next;
    free(tmp->livre->titre);
    free(tmp->livre->auteur);
    free(tmp->livre);
    free(tmp);
    printf("Livre avec ISBN %d supprimé.\n", ISBN);
}

// Fonction pour ajouter un emprunt
void ajouter_emprunt(liste_emprunts **emprunts, liste_emprunteurs **emprunteurs, liste_livres **livres) {
    char nom[MAX_SIZE], titre[MAX_SIZE], dateEmprunt[MAX_SIZE], dateRetour[MAX_SIZE];
    printf("Nom de l'emprunteur: ");
    scanf(" %[^\n]%*c", nom);
    printf("Titre du livre: ");
    scanf(" %[^\n]%*c", titre);
    printf("Date d'emprunt (JJ/MM/AAAA): ");
    scanf(" %[^\n]%*c", dateEmprunt);
    printf("Date de retour (JJ/MM/AAAA): ");
    scanf(" %[^\n]%*c", dateRetour);

    liste_livres *livreTmp = *livres;
    while (livreTmp != NULL && strcmp(livreTmp->livre->titre, titre) != 0) {
        livreTmp = livreTmp->next;
    }

    if (livreTmp == NULL) {
        printf("Livre non trouvé.\n");
        return;
    }

    if (livreTmp->livre->quantite == 0) {
        printf("Aucune copie disponible pour le livre.\n");
        return;
    }

    liste_emprunteurs *emprunteurTmp = *emprunteurs;
    while (emprunteurTmp != NULL && strcmp(emprunteurTmp->emprunteur->nom, nom) != 0) {
        emprunteurTmp = emprunteurTmp->next;
    }

    if (emprunteurTmp == NULL) {
        emprunteurTmp = (liste_emprunteurs *)malloc(sizeof(liste_emprunteurs));
        emprunteurTmp->emprunteur = (Emprunteur *)malloc(sizeof(Emprunteur));
        emprunteurTmp->emprunteur->nom = strdup(nom);
        emprunteurTmp->emprunteur->id = rand();
        emprunteurTmp->next = *emprunteurs;
        *emprunteurs = emprunteurTmp;
    }

    liste_emprunts *newEmprunt = (liste_emprunts *)malloc(sizeof(liste_emprunts));
    newEmprunt->emprunt = (Emprunt *)malloc(sizeof(Emprunt));
    newEmprunt->emprunt->livre = livreTmp->livre;
    newEmprunt->emprunt->emprunteur = emprunteurTmp->emprunteur;
    newEmprunt->emprunt->dateEmprunt = strdup(dateEmprunt);
    newEmprunt->emprunt->dateRetour = strdup(dateRetour);
    newEmprunt->next = *emprunts;
    *emprunts = newEmprunt;

    livreTmp->livre->quantite--;

    printf("Emprunt ajouté avec succès.\n");
}

// Fonction pour afficher la liste des emprunts
void afficher_ls_emprunt(liste_emprunts *l) {
    if (l == NULL) {
        printf("Aucun emprunt n'existe dans la liste des emprunts.\n");
    } else {
        liste_emprunts *tmp = l;
        printf("Les emprunts sont :\n");
        while (tmp != NULL) {
            printf("\tTitre du livre : %s\n\tNom de l'emprunteur : %s\n\tDate d'emprunt : %s\n\tDate de retour : %s\n\n",
                   tmp->emprunt->livre->titre, tmp->emprunt->emprunteur->nom, tmp->emprunt->dateEmprunt, tmp->emprunt->dateRetour);
            tmp = tmp->next;
        }
    }
}

// Fonction pour retourner un livre
void retourner_livre(liste_emprunts **emprunts, liste_livres **livres) {
    char titre[MAX_SIZE], nom[MAX_SIZE];
    printf("Titre du livre: ");
    scanf(" %[^\n]%*c", titre);
    printf("Nom de l'emprunteur: ");
    scanf(" %[^\n]%*c", nom);

    liste_emprunts *tmp = *emprunts, *prev = NULL;
    while (tmp != NULL && (strcmp(tmp->emprunt->livre->titre, titre) != 0 || strcmp(tmp->emprunt->emprunteur->nom, nom) != 0)) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        printf("Emprunt non trouvé.\n");
        return;
    }

    if (prev == NULL) {
        *emprunts = tmp->next;
    } else {
        prev->next = tmp->next;
    }

    liste_livres *livreTmp = *livres;
    while (livreTmp != NULL && strcmp(livreTmp->livre->titre, titre) != 0) {
        livreTmp = livreTmp->next;
    }

    if (livreTmp != NULL) {
        livreTmp->livre->quantite++;
    }

    free(tmp->emprunt->dateEmprunt);
    free(tmp->emprunt->dateRetour);
    free(tmp->emprunt);
    free(tmp);

    printf("Livre retourné avec succès.\n");
}

// Fonction pour afficher la liste des emprunteurs
void afficher_ls_emprunteurs(liste_emprunteurs *l) {
    if (est_vide_emprunteurs(l)) {
        printf("Aucun emprunteur n'existe dans la liste des emprunteurs.\n");
    } else {
        liste_emprunteurs *tmp = l;
        printf("Les emprunteurs dans la bibliothèque sont :\n");
        while (tmp != NULL) {
            printf("\tNom : %s\n\tID : %d\n\n", tmp->emprunteur->nom, tmp->emprunteur->id);
            tmp = tmp->next;
        }
    }
}

// Fonction pour afficher le menu
void afficher_menu() {
    printf("\nMenu:\n");
    printf("  1. Ajouter un livre\n");
    printf("  2. Supprimer un livre\n");
    printf("  3. Emprunter un livre\n");
    printf("  4. Retourner un livre\n");
    printf("  5. Afficher les livres\n");
    printf("  6. Afficher les emprunteurs\n");
    printf("  0. Quitter\n");
    printf("Choisissez une option: ");
}

int main() {
    liste_livres *livres = new_struct();
    liste_emprunteurs *emprunteurs = new_struct();
    liste_emprunts *emprunts = new_struct();

    int choix, ISBN;

    while (true) {
        afficher_menu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_n_livres(&livres);
                break;
            case 2:
                printf("Entrez l'ISBN du livre à supprimer: ");
                scanf("%d", &ISBN);
                supprimer_livre(&livres, ISBN);
                break;
            case 3:
                ajouter_emprunt(&emprunts, &emprunteurs, &livres);
                break;
            case 4:
                retourner_livre(&emprunts, &livres);
                break;
            case 5:
                afficher_ls_livres(livres);
                break;
            case 6:
                afficher_ls_emprunteurs(emprunteurs);
                break;
            case 0:
                printf("Au revoir!\n");
                exit(0);
            default:
                printf("Option invalide. Veuillez réessayer.\n");
        }
    }

    return 0;
}
