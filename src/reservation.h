#ifndef RESERVATION_H
#define RESERVATION_H

#include <gtk/gtk.h> // Needed for GtkTreeView

typedef struct {
    char id_reservation[20];  // changed from int to char array
    char id_citizen[20];
    char vehicle_plate[20];
    char service[30];
    char date[3];
    char month[3];
    char year[5]; 
    char parking[50];
} Reservation;


// Function to add a reservation
void add_reservation(Reservation r);

// Function to check if a reservation ID is unique
int is_unique_id(Reservation r);

// Function to load reservations from a file
int charger_reservations(Reservation *reservations, int max_count);

// Function to display reservations in a GTK TreeView
void afficher_reservation_in_treeview(GtkTreeView *treeview, Reservation *reservations, int count);
int supprimer_reservation(char *filename, const char *id_reservation);
int modifier_reservation(char *filename, char *id_reservation, Reservation r);
/*void populate_combobox_from_file(GtkComboBox *combobox, const char *filename);*/
#endif // RESERVATION_H

