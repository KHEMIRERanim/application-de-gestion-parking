#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"

int is_unique_id(Reservation r) {
    FILE *file = fopen("reservation.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1; // If the file doesn't exist, the ID is unique
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, r.id_reservation) != NULL) {
            fclose(file);
            return 0; // ID is not unique
        }
    }

    fclose(file);
    return 1; // ID is unique
}


void add_reservation(Reservation r) {
    FILE *file = fopen("reservation.txt", "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Debug print to check what is being written
    printf("Writing to file: %s,%s,%s,%s,%s,%s,%s,%s\n",
           r.id_reservation, r.id_citizen, r.vehicle_plate, r.service,
           r.date, r.month, r.year, r.parking);

    fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
            r.id_reservation, r.id_citizen, r.vehicle_plate, r.service,
            r.date, r.month, r.year, r.parking);

    fclose(file);
}

int charger_reservations(Reservation *reservations, int max_count) {
    FILE *file = fopen("reservation.txt", "r");
    int count = 0;

    if (file) {
        while (fscanf(file, "%19[^,],%19[^,],%19[^,],%29[^,],%2[^,],%2[^,],%4[^,],%49[^\n]",
                       reservations[count].id_reservation, reservations[count].id_citizen, reservations[count].vehicle_plate,
                       reservations[count].service, reservations[count].date, reservations[count].month,
                       reservations[count].year, reservations[count].parking) != EOF) {

            count++;
            if (count >= max_count) break;
        }
        fclose(file);
    } else {
        printf("Erreur lors de l'ouverture du fichier reservation.txt\n");
    }

    return count;
}

void afficher_reservation_in_treeview(GtkTreeView *treeview, Reservation *reservations, int count) {
    GtkListStore *list_store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

    for (int i = 0; i < count; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        gtk_list_store_set(list_store, &iter, 
                           0, reservations[i].id_reservation,
                           1, reservations[i].id_citizen,
                           2, reservations[i].vehicle_plate,
                           3, reservations[i].service,
                           4, reservations[i].date,
                           5, reservations[i].month,
                           6, reservations[i].year,
                           7, reservations[i].parking,
                           -1);
    }
}



int supprimer_reservation(char *filename, const char *id_reservation) {
    FILE *file = fopen(filename, "r");
    FILE *temp_file = fopen("temp.txt", "w");  // Temporary file for safe write operation
    char line[256];  // Buffer for each line
    int found = 0;

    if (!file || !temp_file) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 0;
    }

    // Debugging: Print the ID to delete
    printf("ID à supprimer : %s\n", id_reservation);

    // Process each line in the file
    while (fgets(line, sizeof(line), file)) {
        char temp_id_reservation[50];  // Buffer for ID extraction

        // Extract the ID (first field) from the line
        sscanf(line, "%49[^,]", temp_id_reservation);

        // Debugging: Print the current ID being checked
        printf("Vérification de la réservation ID : %s\n", temp_id_reservation);

        if (strcmp(temp_id_reservation, id_reservation) != 0) {
            // Write the line to the temporary file if the ID doesn't match
            fputs(line, temp_file);
        } else {
            found = 1;  // Found the reservation to delete
            printf("Réservation trouvée et supprimée : %s\n", temp_id_reservation);
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(filename);  // Delete original file
        rename("temp.txt", filename);  // Rename temp file to original file name
        return 1;  // Success
    } else {
        printf("Erreur: Réservation non trouvée.\n");
        remove("temp.txt");  // Clean up the temp file if reservation not found
        return 0;  // Failure
    }
}

int modifier_reservation(char *filename, char *id_reservation, Reservation r) {
    FILE *file = fopen(filename, "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char buffer[512]; // Temporary buffer to read each line
    Reservation temp;
    int found = 0;

    if (!file || !temp_file) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        if (file) fclose(file);
        if (temp_file) fclose(temp_file);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        // Parsing each line to fill the temp Reservation struct
        if (sscanf(buffer, "%19[^,],%19[^,],%19[^,],%29[^,],%3[^,],%3[^,],%4[^,],%49[^\n]",
                   temp.id_reservation, temp.id_citizen, temp.vehicle_plate, temp.service,
                   temp.date, temp.month, temp.year, temp.parking) == 8) {
            if (strcmp(temp.id_reservation, id_reservation) == 0) {
                // If the reservation ID matches, write the modified reservation
                fprintf(temp_file, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                        r.id_reservation, r.id_citizen, r.vehicle_plate, r.service,
                        r.date, r.month, r.year, r.parking);
                found = 1;
            } else {
                // Write the other lines unchanged
                fputs(buffer, temp_file);
            }
        } else {
            printf("Ligne ignorée (mauvais format) : %s", buffer);
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace the original file with the temporary file
    remove(filename);
    rename("temp.txt", filename);

    if (found) {
        printf("Réservation modifiée avec succès.\n");
    } else {
        printf("Aucune réservation trouvée avec l'ID : %s\n", id_reservation);
    }

    return found;
}

/*void populate_combobox_from_file(GtkComboBox *combobox, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char parking_name[50]; // To store the parking name
        // Extract the second field (parking name) from the line
        sscanf(line, "%*[^,],%49[^,]", parking_name);

        // Add the parking name to the combobox
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox), parking_name);
    }

    fclose(file);
}*/



