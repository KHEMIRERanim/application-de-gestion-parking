#include<stdio.h>
#include <string.h>
#include "parking.h"
#include <glib.h>


int ajouter_parking(char *filename, parking p) {
    FILE *file = fopen(filename, "a"); 
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }
    fprintf(file, "%d,%s,%s,%d,%d,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%s\n", p.id, p.nom, p.rue, p.numero, p.code_postal, p.ville, p.type, p.nbr_place_disponible, p.nbr_place_reserve, p.nbr_etages, p.type_acces, p.systeme_securite, p.heure_debut, p.heure_fin, p.statut_actuel, p.agent);
    fclose(file);
    return 1;
}




int charger_parkings(parking *parkings, int max_count) {
    FILE *fichier = fopen("parking.txt", "r");
    int count = 0;

    if (fichier) {
      while (fscanf(fichier, "%d,%49[^,],%49[^,],%d,%d,%49[^,],%19[^,],%d,%d,%d,%49[^,],%49[^,],%5[^,],%5[^,],%19[^,],%49[^\n]",
              &parkings[count].id, parkings[count].nom, parkings[count].rue, &parkings[count].numero,
              &parkings[count].code_postal, parkings[count].ville, parkings[count].type,
              &parkings[count].nbr_place_disponible, &parkings[count].nbr_place_reserve,
              &parkings[count].nbr_etages, parkings[count].type_acces,
              parkings[count].systeme_securite, parkings[count].heure_debut,
              parkings[count].heure_fin, parkings[count].statut_actuel, parkings[count].agent) != EOF) {

            count++;
            if (count >= max_count) break;
        }
        fclose(fichier);
    } else {
        printf("Erreur lors de l'ouverture du fichier parking.txt\n");
    }

    return count; 
}


void afficher_parkings_in_treeview(GtkTreeView *treeview, parking *parkings, int count) {
    GtkListStore *list_store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        
        gtk_list_store_set(list_store, &iter, 
                           0, parkings[i].id,
                           1, parkings[i].nom,
                           2, parkings[i].rue,
                           3, parkings[i].numero,
                           4, parkings[i].code_postal,
                           5, parkings[i].ville,
                           6, parkings[i].type,
                           7, parkings[i].nbr_place_disponible,
                           8, parkings[i].nbr_place_reserve,
                           9, parkings[i].nbr_etages,
                           10, parkings[i].type_acces,
                           11, parkings[i].systeme_securite,
                           12, parkings[i].heure_debut,
                           13, parkings[i].heure_fin,
                           14, parkings[i].statut_actuel,
                           15, parkings[i].agent,
                           -1); 
    }
}






int supprimer_parking(char *filename, int id_parking) {
    FILE *file = fopen(filename, "r");
    FILE *temp_file = fopen("temp.txt", "w");
    parking temp;
    int found = 0;

    if (!file || !temp_file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }

    while (fscanf(file, "%d,%49[^,],%49[^,],%d,%d,%49[^,],%19[^,],%d,%d,%d,%49[^,],%49[^,],%5[^,],%5[^,],%19[^,],%49[^\n]", &temp.id, temp.nom, temp.rue, &temp.numero, &temp.code_postal, temp.ville, temp.type, &temp.nbr_place_disponible, &temp.nbr_place_reserve, &temp.nbr_etages, temp.type_acces, temp.systeme_securite, temp.heure_debut, temp.heure_fin, temp.statut_actuel, temp.agent) != EOF) {
        if (temp.id != id_parking) {
            fprintf(temp_file, "%d,%s,%s,%d,%d,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%s\n", temp.id, temp.nom, temp.rue, temp.numero, temp.code_postal, temp.ville, temp.type, temp.nbr_place_disponible, temp.nbr_place_reserve, temp.nbr_etages, temp.type_acces, temp.systeme_securite, temp.heure_debut, temp.heure_fin, temp.statut_actuel, temp.agent);
        } else {
            found = 1;  
        }
    }
    fclose(file);
    fclose(temp_file);
    remove(filename);  
    rename("temp.txt", filename);  
    return found;
}





int affecter_agent_au_parking(const char *parking_id, const char *agent_name) {
    FILE *file = fopen("parking.txt", "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier parking.txt\n");
        return 0; // Erreur lors de l'ouverture
    }

    char lines[100][256]; // Stocker toutes les lignes du fichier
    int line_count = 0;
    int found_parking = 0;
    int agent_already_assigned = 0;

    char current_parking_id[10];
    char current_agent[50];

    // Lire toutes les lignes du fichier
    while (fgets(lines[line_count], sizeof(lines[line_count]), file) != NULL) {
        // Extraire l'ID du parking et le nom de l'agent
        sscanf(lines[line_count], "%[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%49[^\n]",
               current_parking_id, current_agent);

        // Vérifier si l'agent est déjà assigné à un autre parking
        if (strcmp(current_agent, agent_name) == 0) {
            printf("Erreur: L'agent '%s' est déjà affecté au parking ID %s.\n", agent_name, current_parking_id);
            agent_already_assigned = 1;
            break;
        }

        line_count++;
    }
    fclose(file);

    // Si l'agent est déjà assigné, on stoppe ici
    if (agent_already_assigned) {
        return -1; // L'agent est déjà affecté
    }

    // Réouvrir le fichier pour mise à jour
    file = fopen("parking.txt", "w");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier parking.txt en écriture\n");
        return 0;
    }

    // Réécrire les lignes du fichier
    for (int i = 0; i < line_count; i++) {
        sscanf(lines[i], "%[^,],", current_parking_id);

        if (strcmp(current_parking_id, parking_id) == 0) {
            found_parking = 1;

            // Remplacer "agent" par le nom de l'agent
            char *agent_position = strrchr(lines[i], ','); // Trouver la dernière virgule
            if (agent_position != NULL) {
                strcpy(agent_position + 1, agent_name); // Remplacer par le nouveau nom d'agent
                strcat(agent_position, "\n");          // Ajouter un saut de ligne
            }
        }

        fputs(lines[i], file); // Réécrire chaque ligne dans le fichier
    }
    fclose(file);

    if (!found_parking) {
        printf("Erreur: Le parking ID %s est introuvable.\n", parking_id);
        return 0; // Parking non trouvé
    }

    printf("L'agent '%s' a été affecté au parking %s avec succès.\n", agent_name, parking_id);
    return 1; // Affectation réussie
}
int modifier_parking(char *filename, int id_parking, parking p) {
    FILE *file = fopen(filename, "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char buffer[512]; // Tampon pour lire chaque ligne
    parking temp;
    int found = 0;

    if (!file || !temp_file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%d,%49[^,],%49[^,],%d,%d,%49[^,],%19[^,],%d,%d,%d,%49[^,],%49[^,],%5[^,],%5[^,],%19[^,],%49[^\n]",
                   &temp.id, temp.nom, temp.rue, &temp.numero, &temp.code_postal, temp.ville, temp.type,
                   &temp.nbr_place_disponible, &temp.nbr_place_reserve, &temp.nbr_etages, temp.type_acces,
                   temp.systeme_securite, temp.heure_debut, temp.heure_fin, temp.statut_actuel, temp.agent) == 16) {
            if (temp.id == id_parking) {
                // Ecrire la nouvelle version modifiée
                fprintf(temp_file, "%d,%s,%s,%d,%d,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%s\n",
                        id_parking, p.nom, p.rue, p.numero, p.code_postal, p.ville, p.type,
                        p.nbr_place_disponible, p.nbr_place_reserve, p.nbr_etages, p.type_acces,
                        p.systeme_securite, p.heure_debut, p.heure_fin, p.statut_actuel, p.agent);
                found = 1;
            } else {
                // Réécrire les autres lignes telles quelles
                fputs(buffer, temp_file);
            }
        } else {
            printf("Ligne ignorée (mauvais format) : %s", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Remplacement du fichier original par le fichier temporaire
    remove(filename);
    rename("temp.txt", filename);

    if (found) {
        printf("Parking modifié avec succès.\n");
    } else {
        printf("Aucun parking trouvé avec l'ID : %d\n", id_parking);
    }

    return found;
}

