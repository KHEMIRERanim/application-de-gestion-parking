#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "callbacks.h"
#include "reservation.h"
#include "parking.h"
#include "interface.h"

// Declare the lookup_widget function
GtkWidget* lookup_widget(GtkWidget *widget, const gchar *widget_name);

int red=1;
void on_buttonval_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entryidr = lookup_widget(GTK_WIDGET(button), "entryidr");
    GtkWidget *entryidc = lookup_widget(GTK_WIDGET(button), "entryidc");
    GtkWidget *entrymat = lookup_widget(GTK_WIDGET(button), "entrymat");
    GtkWidget *entryser = lookup_widget(GTK_WIDGET(button), "entryser");
    GtkWidget *spinbutton1 = lookup_widget(GTK_WIDGET(button), "spinbutton1");
    GtkWidget *spinbutton2 = lookup_widget(GTK_WIDGET(button), "spinbutton2");
    GtkWidget *entryanne = lookup_widget(GTK_WIDGET(button), "entryanne");
    GtkWidget *comboboxpark = lookup_widget(GTK_WIDGET(button), "comboboxpark");
    GtkWidget *labelres = lookup_widget(GTK_WIDGET(button), "labelres");

    Reservation r;

    // Copy inputs into the Reservation struct
    strncpy(r.id_reservation, gtk_entry_get_text(GTK_ENTRY(entryidr)), sizeof(r.id_reservation) - 1);
    r.id_reservation[sizeof(r.id_reservation) - 1] = '\0';  // Ensure null-termination

    strncpy(r.id_citizen, gtk_entry_get_text(GTK_ENTRY(entryidc)), sizeof(r.id_citizen) - 1);
    r.id_citizen[sizeof(r.id_citizen) - 1] = '\0';

    strncpy(r.vehicle_plate, gtk_entry_get_text(GTK_ENTRY(entrymat)), sizeof(r.vehicle_plate) - 1);
    r.vehicle_plate[sizeof(r.vehicle_plate) - 1] = '\0';

    strncpy(r.service, gtk_entry_get_text(GTK_ENTRY(entryser)), sizeof(r.service) - 1);
    r.service[sizeof(r.service) - 1] = '\0';

    snprintf(r.date, sizeof(r.date), "%d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton1)));
    snprintf(r.month, sizeof(r.month), "%d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton2)));
    strncpy(r.year, gtk_entry_get_text(GTK_ENTRY(entryanne)), sizeof(r.year) - 1);
    r.year[sizeof(r.year) - 1] = '\0';

    // Handle ComboBox value
    const gchar *selected_parking = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxpark));
    if (selected_parking != NULL) {
        strncpy(r.parking, selected_parking, sizeof(r.parking) - 1);
        r.parking[sizeof(r.parking) - 1] = '\0';  // Ensure null-termination
    } else {
        gtk_label_set_text(GTK_LABEL(labelres), "Veuillez sélectionner un parking.");
        return;
    }

    // Check for unique ID and conditions
    int is_unique = is_unique_id(r);
    if (red == 1 && is_unique) {
        add_reservation(r);
        gtk_label_set_text(GTK_LABEL(labelres), "Réservation réussie !");
        printf("Reservation added successfully: %s\n", r.id_reservation);  // Debug
    } else {
        if (red == 2) {
            gtk_label_set_text(GTK_LABEL(labelres), "Réservation échouée: données non acceptées.");
        } else if (!is_unique) {
            gtk_label_set_text(GTK_LABEL(labelres), "Réservation échouée: ID déjà utilisé.");
        } else {
            gtk_label_set_text(GTK_LABEL(labelres), "Réservation échouée !");
        }
    }
}


void
on_buttonmodif_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttonsupp_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window2 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *treeviewres = lookup_widget(window2, "treeviewres");

    if (!treeviewres) {
        g_print("Erreur: TreeView non trouvé.\n");
        return;
    }

    // Get the selection from the TreeView
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeviewres));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Retrieve the ID (column 0) from the selected row
        char *id_reservation_str;
        gtk_tree_model_get(model, &iter, 0, &id_reservation_str, -1);  // Column 0 is the ID
        g_print("ID récupéré depuis TreeView : %s\n", id_reservation_str);

        // Trim extra whitespace or newlines (if any)
        id_reservation_str = g_strstrip(id_reservation_str);

        // Delete the reservation from the file
        if (supprimer_reservation("reservation.txt", id_reservation_str)) {
            g_print("La réservation avec ID %s a été supprimée avec succès.\n", id_reservation_str);

            // Remove the row from the TreeView
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        } else {
            g_print("Erreur: La réservation avec ID %s n'a pas pu être supprimée.\n", id_reservation_str);
        }

        // Free the memory allocated for the ID
        g_free(id_reservation_str);
    } else {
        // If no row is selected, show an error dialog
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window2),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner une réservation à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void
on_buttonajout_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{  
    GtkWidget *window1;
    window1=create_window1();
    gtk_widget_show(window1);
    
    
}

void
on_buttonvoir_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window2;
    GtkWidget *treeviewres;
    GtkListStore *store;
    GtkTreeIter iter;

    // Create the window from the Glade file
    window2 = create_window2();
    gtk_widget_show(window2);

    // Get the TreeView from the window
    treeviewres = lookup_widget(window2, "treeviewres");

    // Create a GtkListStore with 8 columns for the reservation data
    store = gtk_list_store_new(8, 
                               G_TYPE_STRING,  // ID de réservation
                               G_TYPE_STRING,  // ID du citoyen
                               G_TYPE_STRING,  // Plaque du véhicule
                               G_TYPE_STRING,  // Service
                               G_TYPE_STRING,  // Date
                               G_TYPE_STRING,  // Mois
                               G_TYPE_STRING,  // Année
                               G_TYPE_STRING   // Parking
    );

    // Load the reservation data from the file
    Reservation reservations[100];  // Array to store up to 100 reservations
    int count = charger_reservations(reservations, 100);  // Function to load reservations

    // If no reservations are found, ensure the TreeView is empty
    if (count == 0) {
        gtk_label_set_text(GTK_LABEL(lookup_widget(window2, "labelres")), "Aucune réservation trouvée.");
    }

    // Add the reservations to the GtkListStore
    for (int i = 0; i < count; i++) {
        gtk_list_store_append(store, &iter);

        // Add the data to the store
        gtk_list_store_set(store, &iter,
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

    // Add columns to the TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column;

    column = gtk_tree_view_column_new_with_attributes("ID de réservation", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("ID du citoyen", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Plaque du véhicule", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Service", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Mois", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Année", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    column = gtk_tree_view_column_new_with_attributes("Parking", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewres), column);

    // Associate the store with the TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewres), GTK_TREE_MODEL(store));
    g_object_unref(store);  // Free the memory allocated

}


void
on_radiobuttonok_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
red=1;
}


void
on_radiobuttonnon_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
red=2;
}


void
on_buttonchercher_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entryidmode; 
    GtkWidget *labelresmodif;
    const char *id_value;
    char buffer[256];
    int found = 0;

    // Récupération des widgets
    entryidmode = lookup_widget(GTK_WIDGET(button), "entryidmode");
    labelresmodif = lookup_widget(GTK_WIDGET(button), "labelresmodif");

    if (entryidmode && labelresmodif) {
        id_value = gtk_entry_get_text(GTK_ENTRY(entryidmode));

        if (id_value && strlen(id_value) > 0) {
            FILE *file = fopen("reservation.txt", "r"); // Change to the relevant file
            if (file) {
                while (fgets(buffer, sizeof(buffer), file)) {
                    char id[256];
                    sscanf(buffer, "%255[^,]", id); // Lire l'ID de la première colonne
                    if (strcmp(id, id_value) == 0) {
                        found = 1;
                        break;
                    }
                }
                fclose(file);
            } else {
                printf("Erreur: Impossible d'ouvrir le fichier reservation.txt\n");
            }

            if (found) {
                // Afficher un message de succès dans le label
                gtk_label_set_text(GTK_LABEL(labelresmodif), "ID trouvé : Veuillez modifier la réservation.");
                printf("ID trouvé : Veuillez modifier la réservation.\n");

                // Vérifier si le signal est déjà connecté pour éviter les doublons
                if (!g_signal_handlers_block_matched(button, G_SIGNAL_MATCH_FUNC, 0, 0, NULL, G_CALLBACK(on_buttonchercher_clicked), NULL)) {
                    gpointer user_data_id = g_strdup(id_value);  // Allouer une copie de l'ID
                    g_signal_connect(button, "clicked", G_CALLBACK(on_buttonchercher_clicked), user_data_id);
                }
            } else {
                // Afficher un message d'erreur dans le label
                gtk_label_set_text(GTK_LABEL(labelresmodif), "ID non trouvé.");
                printf("ID non trouvé.\n");
            }
        } else {
            // Afficher un message d'erreur dans le label si l'ID est vide
            gtk_label_set_text(GTK_LABEL(labelresmodif), "Veuillez entrer un ID valide.");
            printf("Veuillez entrer un ID valide.\n");
        }
    } else {
        printf("Erreur: Impossible de récupérer les widgets.\n");
    }
}


void
on_buttonmodife_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer l'ID de la réservation depuis le champ de saisie
GtkWidget *entryidmode = lookup_widget(GTK_WIDGET(button), "entryidmode");
const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(entryidmode));

if (strlen(id_recherche) == 0) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "Veuillez entrer un ID valide.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
}

// Créer une structure pour la réservation et remplir les champs avec les widgets Gtk
Reservation r;
strcpy(r.id_reservation, id_recherche);
g_print("ID Réservation : %s\n", r.id_reservation);  // Debug : Afficher l'ID récupéré

// Récupérer les widgets associés aux autres champs
GtkWidget *entryidcmod = lookup_widget(GTK_WIDGET(button), "entryidcmod");
GtkWidget *entrymatmo = lookup_widget(GTK_WIDGET(button), "entrymatmo");
GtkWidget *entrysmodife = lookup_widget(GTK_WIDGET(button), "entrysmodife");
GtkWidget *spinbuttondatemod = lookup_widget(GTK_WIDGET(button), "spinbuttondatemod");
GtkWidget *spinbuttonmonthmod = lookup_widget(GTK_WIDGET(button), "spinbuttonmonthmod");
GtkWidget *entryyearmod = lookup_widget(GTK_WIDGET(button), "entryyearmod");
GtkWidget *comboboxparkkk = lookup_widget(GTK_WIDGET(button), "comboboxparkkk");


strncpy(r.id_citizen, gtk_entry_get_text(GTK_ENTRY(entryidcmod)), sizeof(r.id_citizen) - 1);
r.id_citizen[sizeof(r.id_citizen) - 1] = '\0';

strncpy(r.service, gtk_entry_get_text(GTK_ENTRY(entrysmodife)), sizeof(r.service) - 1);
r.service[sizeof(r.service) - 1] = '\0';

strncpy(r.vehicle_plate, gtk_entry_get_text(GTK_ENTRY(entrymatmo)), sizeof(r.vehicle_plate) - 1);
r.vehicle_plate[sizeof(r.vehicle_plate) - 1] = '\0';

snprintf(r.date, sizeof(r.date), "%02d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttondatemod)));
snprintf(r.month, sizeof(r.month), "%02d", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttonmonthmod)));

strncpy(r.year, gtk_entry_get_text(GTK_ENTRY(entryyearmod)), sizeof(r.year) - 1);
r.year[sizeof(r.year) - 1] = '\0';

// Get selected parking
const gchar *selected_parking = gtk_combo_box_get_active_text(GTK_COMBO_BOX(comboboxparkkk));
if (selected_parking != NULL) {
    strncpy(r.parking, selected_parking, sizeof(r.parking) - 1);
    r.parking[sizeof(r.parking) - 1] = '\0';
} else {
    r.parking[0] = '\0';  // If no parking is selected
}


// Appeler la fonction de modification
g_print("Tentative de modification de la réservation avec ID %s...\n", r.id_reservation);
if (modifier_reservation("reservation.txt", r.id_reservation, r)) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Modification réussie !");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
} else {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "Erreur lors de la modification.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

}

void load_combo_box_from_file(GtkWidget *combobox, const char *filename) {
    // Remove all items from the ComboBox (GTK+ 2.x)
    gint item_count = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
    while (item_count >= 0) {
        gtk_combo_box_remove_text(GTK_COMBO_BOX(combobox), item_count);
        item_count--;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    char line[1024];  // Increased size to handle longer lines
    while (fgets(line, sizeof(line), file)) {
        // Remove any trailing newline characters from the line
        line[strcspn(line, "\n")] = 0;

        // Strip any leading/trailing spaces from the line (optional)
        g_strstrip(line);

        // Add the entire line into the ComboBox
        gtk_combo_box_append_text(GTK_COMBO_BOX(combobox), line);
    }

    fclose(file);
}


void
on_buttoncharge_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *comboboxpark = lookup_widget(GTK_WIDGET(button), "comboboxpark");
    
    // Ensure ComboBox is populated
    //load_combo_box_from_file(GTK_COMBO_BOX(comboboxpark), "parking.txt");
    load_combo_box_from_file(comboboxpark, "parking.txt");
}


void
on_buttonannaj_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window1 = gtk_widget_get_toplevel(GTK_WIDGET(button));

// Vérifier si l'objet est bien une fenêtre
if (GTK_IS_WINDOW(window1)) {
    gtk_widget_destroy(window1); // Fermer la fenêtre
}

}


void
on_buttonquittsupp_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window2 = gtk_widget_get_toplevel(GTK_WIDGET(button));

// Vérifier si l'objet est bien une fenêtre
if (GTK_IS_WINDOW(window2)) {
    gtk_widget_destroy(window2); // Fermer la fenêtre
}
}


void
on_buttonannmodif_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window4 = gtk_widget_get_toplevel(GTK_WIDGET(button));

// Vérifier si l'objet est bien une fenêtre
if (GTK_IS_WINDOW(window4)) {
    gtk_widget_destroy(window4); // Fermer la fenêtre
}
}


void
on_buttonchargermod_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *comboboxparkkk = lookup_widget(GTK_WIDGET(button), "comboboxparkkk");
    
    // Ensure ComboBox is populated
    //load_combo_box_from_file(GTK_COMBO_BOX(comboboxpark), "parking.txt");
    load_combo_box_from_file(comboboxparkkk, "parking.txt");
}




void
on_buttonaffmodife_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window4;
    window4=create_window4();
    gtk_widget_show(window4);
}


void
on_buttonespacecitoyen_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window7;
    window7=create_window7();
    gtk_widget_show(window7);
}


void
on_buttonespaceadmin_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window6;
    window6=create_window6();
    gtk_widget_show(window6);
}


void
on_buttongdp_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window8;
    window8=create_window8();
    gtk_widget_show(window8);
}


void
on_buttongdadp_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    
}


void
on_buttongdsdp_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttongdc_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_buttongdr_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window3;
    window3=create_window3();
    gtk_widget_show(window3);
}


void
on_buttongdaer_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_RK_button_ajouter_main_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window9;
    window9=create_window9();
    gtk_widget_show(window9);
}


void
on_RK_button_afficher_main_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window11;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeIter iter;

    window11 = create_window11();
    gtk_widget_show(window11);

    treeview = lookup_widget(window11, "RK_treeview_afficherparking_afficher");

    // Créer un GtkListStore avec les 16 colonnes en G_TYPE_STRING pour afficher correctement les valeurs
    store = gtk_list_store_new(16, 
                               G_TYPE_STRING,  // ID
                               G_TYPE_STRING,  // Nom
                               G_TYPE_STRING,  // Rue
                               G_TYPE_STRING,  // Numéro
                               G_TYPE_STRING,  // Code postal
                               G_TYPE_STRING,  // Ville
                               G_TYPE_STRING,  // Type
                               G_TYPE_STRING,  // Nombre de places disponibles
                               G_TYPE_STRING,  // Nombre de places réservées
                               G_TYPE_STRING,  // Nombre d'étages
                               G_TYPE_STRING,  // Type d'accès
                               G_TYPE_STRING,  // Système de sécurité
                               G_TYPE_STRING,  // Heure de début
                               G_TYPE_STRING,  // Heure de fin
                               G_TYPE_STRING,  // Statut actuel
                               G_TYPE_STRING   // Agent
    );

    // Charger les données des parkings à partir du fichier texte
    parking parkings[100];  // Tableau pour stocker jusqu'à 100 parkings
    int count = charger_parkings(parkings, 100);  // Fonction à ajouter dans parking.c

    // Ajouter les parkings au GtkListStore
    for (int i = 0; i < count; i++) {
        gtk_list_store_append(store, &iter);

        // Conversion des entiers en chaînes de caractères
        char id_str[10], numero_str[10], code_postal_str[10], places_dispo_str[10], places_reserve_str[10], etages_str[10];
        snprintf(id_str, sizeof(id_str), "%d", parkings[i].id);
        snprintf(numero_str, sizeof(numero_str), "%d", parkings[i].numero);
        snprintf(code_postal_str, sizeof(code_postal_str), "%d", parkings[i].code_postal);
        snprintf(places_dispo_str, sizeof(places_dispo_str), "%d", parkings[i].nbr_place_disponible);
        snprintf(places_reserve_str, sizeof(places_reserve_str), "%d", parkings[i].nbr_place_reserve);
        snprintf(etages_str, sizeof(etages_str), "%d", parkings[i].nbr_etages);

        // Ajouter les données converties au store
        gtk_list_store_set(store, &iter,
                           0, id_str,
                           1, parkings[i].nom,
                           2, parkings[i].rue,
                           3, numero_str,
                           4, code_postal_str,
                           5, parkings[i].ville,
                           6, parkings[i].type,
                           7, places_dispo_str,
                           8, places_reserve_str,
                           9, etages_str,
                           10, parkings[i].type_acces,
                           11, parkings[i].systeme_securite,
                           12, parkings[i].heure_debut,
                           13, parkings[i].heure_fin,
                           14, parkings[i].statut_actuel,
                           15, parkings[i].agent,
                           -1);
    }

    // Ajouter les colonnes au TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column;

    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Rue", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Numéro", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Code Postal", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Ville", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Places Dispo", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Places Réservées", renderer, "text", 8, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Étages", renderer, "text", 9, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Accès", renderer, "text", 10, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Sécurité", renderer, "text", 11, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Heure Début", renderer, "text", 12, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Heure Fin", renderer, "text", 13, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Statut", renderer, "text", 14, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Agent", renderer, "text", 15, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Associer le store au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);  // Libérer la mémoire allouée
}


void
on_RK_button_affecter_main_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window12;
    window12=create_window12();
    gtk_widget_show(window12);
    GtkWidget *RK_combobox_affecter_affecter = lookup_widget(window12, "RK_combobox_affecter_affecter");

    // Charger les éléments du fichier texte dans le combobox
    load_combo_box_from_files(RK_combobox_affecter_affecter, "parking.txt");


   // Récupérer le combobox depuis l'interface
    GtkWidget *RK_comboboxentry2_affecter_affecter = lookup_widget(window12, "RK_comboboxentry2_affecter_affecter");

    // Charger les éléments du fichier texte dans le combobox
    load_combo_box_from_files(RK_comboboxentry2_affecter_affecter, "agent.txt");


    // Afficher la fenêtre d'affectation
    gtk_widget_show(window12);
}


void
on_RK_button_modifier_main_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *window10;
    window10=create_window10();
    gtk_widget_show(window10);
}


void
on_RK_button_annuler_ajouter_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer la fenêtre parente (la fenêtre principale qui contient le bouton)
    GtkWidget *window9 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // Vérifier si l'objet est bien une fenêtre
    if (GTK_IS_WINDOW(window9)) {
        gtk_widget_destroy(window9); // Fermer la fenêtre
    }
}


void
on_RK_button_ajouter_ajouter_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *RK_button_ajouter_ajouter;
    RK_button_ajouter_ajouter = lookup_widget(GTK_WIDGET(button), "RK_button_ajouter_ajouter");

    GtkWidget *input_id, *input_nom, *input_agent, *input_rue, *input_numero, *input_code_postal, *input_ville;
    GtkWidget *check_public, *check_prive, *check_residentiel, *check_commercial;
    GtkWidget *spin_nbr_places, *spin_nbr_placesreserve, *spin_nbr_etages, *input_type_acces;
    GtkWidget *check_camera, *check_alarme, *check_gardien, *check_eclairage;
    GtkWidget *spin_heure_debut, *spin_heure_fin, *combo_statut;
    GtkWidget *label_message;
    parking p;

    // Lookup des widgets
    input_id = lookup_widget(GTK_WIDGET(button), "RK_entry_id_ajouter");
    input_nom = lookup_widget(GTK_WIDGET(button), "RK_entry_nomparking_ajouter");
    input_agent = lookup_widget(GTK_WIDGET(button), "RK_entry_nomagent_ajouter");
    input_rue = lookup_widget(GTK_WIDGET(button), "RK_entry_rue_ajouter");
    input_numero = lookup_widget(GTK_WIDGET(button), "RK_entry_num_ajouter");
    input_code_postal = lookup_widget(GTK_WIDGET(button), "RK_entry_codepostal_ajouter");
    input_ville = lookup_widget(GTK_WIDGET(button), "RK_entry_ville_ajouter");

    check_public = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_public_ajouter");
    check_prive = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_prive_ajouter");
    check_residentiel = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_residentiel_ajouter");
    check_commercial = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_commercial_ajouter");

    spin_nbr_places = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbplace_ajouter");
    spin_nbr_placesreserve = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbplacereserve_ajouter");
    spin_nbr_etages = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbetage_ajouter");
    input_type_acces = lookup_widget(GTK_WIDGET(button), "RK_comboboxentry_typeacces_ajouter");

    check_camera = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_camera_ajouter");
    check_alarme = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_alarme_ajouter");
    check_gardien = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_gardien_ajouter");
    check_eclairage = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_eclairage_ajouter");

    spin_heure_debut = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_debut_ajouter");
    spin_heure_fin = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_fin_ajouter");
    combo_statut = lookup_widget(GTK_WIDGET(button), "RK_comboboxentry_statut_ajouter");

    // Remplir la structure `parking`
    p.id = atoi(gtk_entry_get_text(GTK_ENTRY(input_id)));
    strcpy(p.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
    strcpy(p.rue, gtk_entry_get_text(GTK_ENTRY(input_rue)));
    p.numero = atoi(gtk_entry_get_text(GTK_ENTRY(input_numero)));
    p.code_postal = atoi(gtk_entry_get_text(GTK_ENTRY(input_code_postal)));
    strcpy(p.ville, gtk_entry_get_text(GTK_ENTRY(input_ville)));

  

    // Type de parking
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_public))) {
        strcpy(p.type, "Public");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_prive))) {
        strcpy(p.type, "Prive");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_residentiel))) {
        strcpy(p.type, "Residentiel");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_commercial))) {
        strcpy(p.type, "Commercial");
    }

    // Récupération des valeurs depuis les spinbuttons
    p.nbr_place_disponible = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_places));
    p.nbr_place_reserve = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_placesreserve));
    p.nbr_etages = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_etages));



    const gchar *text_acces = gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_type_acces));
    strcpy(p.type_acces, text_acces ? text_acces : "");

    // Système de sécurité
    char systeme_securite[100] = "";
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_camera))) {
        strcat(systeme_securite, "Cameras ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_alarme))) {
        strcat(systeme_securite, "Alarme ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_gardien))) {
        strcat(systeme_securite, "Gardien ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_eclairage))) {
        strcat(systeme_securite, "Eclairage ");
    }
    strcpy(p.systeme_securite, systeme_securite);



    // Heures
    sprintf(p.heure_debut, "%02d:00", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure_debut)));
    sprintf(p.heure_fin, "%02d:00", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure_fin)));

    // Statut
    const gchar *statut = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_statut));
    strcpy(p.statut_actuel, statut ? statut : "");

    // Agent
    strcpy(p.agent, "agent");


    // Ajout et message
    label_message = lookup_widget(GTK_WIDGET(button), "RK_label_message_ajouter");

    if (ajouter_parking("parking.txt", p)) {
        gtk_label_set_text(GTK_LABEL(label_message), "Ajout avec succès !");
        printf("DEBUG - Parking ajouté avec succès.\n");
    } else {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur lors de l'ajout du parking.");
        printf("DEBUG - Échec de l'ajout du parking.\n");
    }
}


void
on_RK_button_annuler_modifier_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer la fenêtre parente (la fenêtre principale qui contient le bouton)
    GtkWidget *window10 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // Vérifier si l'objet est bien une fenêtre
    if (GTK_IS_WINDOW(window10)) {
        gtk_widget_destroy(window10); // Fermer la fenêtre
    }
}


void
on_RK_button_modifier_modifier_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer l'ID du parking depuis le champ de saisie
    GtkWidget *entry_id = lookup_widget(GTK_WIDGET(button), "RK_entry_id_modifier");
    const char *id_recherche = gtk_entry_get_text(GTK_ENTRY(entry_id));

    if (strlen(id_recherche) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Veuillez entrer un ID valide.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Convertir l'ID en entier
    int id_parking = atoi(id_recherche);
    g_print("ID Parking : %d\n", id_parking);  // Débogage : Affichage de l'ID récupéré

    GtkWidget *input_nom, *input_rue, *input_numero, *input_code_postal, *input_ville;
    GtkWidget *check_public, *check_prive, *check_residentiel, *check_commercial;
    GtkWidget *spin_nbr_places,*spin_nbr_placesreserve, *spin_nbr_etages, *input_type_acces;
    GtkWidget *check_camera, *check_alarme, *check_gardien, *check_eclairage;
    GtkWidget *spin_heure_debut, *spin_heure_fin, *combo_statut;
    parking p;

    // Récupération des widgets
    input_nom = lookup_widget(GTK_WIDGET(button), "RK_entry_nomparking_modifier");
    input_rue = lookup_widget(GTK_WIDGET(button), "RK_entry_rue_modifier");
    input_numero = lookup_widget(GTK_WIDGET(button), "RK_entry_num_modifier");
    input_code_postal = lookup_widget(GTK_WIDGET(button), "RK_entry_codepostal_modifier");
    input_ville = lookup_widget(GTK_WIDGET(button), "RK_entry_ville_modifier");

    check_public = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_public_modifier");
    check_prive = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_prive_modifier");
    check_residentiel = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_residentiel_modifier");
    check_commercial = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_commercial_modifier");

    spin_nbr_places = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbplace_modifier");
    spin_nbr_etages = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbetage_modifier");
    input_type_acces = lookup_widget(GTK_WIDGET(button), "RK_comboboxentry_typeacces_modifier");

    spin_nbr_placesreserve =  lookup_widget(GTK_WIDGET(button), "RK_spinbutton_nbplacereservee_modifier");

    check_camera = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_camera_modifier");
    check_alarme = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_alarme_modifier");
    check_gardien = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_gardien_modifier");
    check_eclairage = lookup_widget(GTK_WIDGET(button), "RK_checkbutton_eclairage_modifier");

    spin_heure_debut = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_debut_modifier");
    spin_heure_fin = lookup_widget(GTK_WIDGET(button), "RK_spinbutton_fin_modifier");
    combo_statut = lookup_widget(GTK_WIDGET(button), "RK_comboboxentry_statut_modifier");

    // Récupérer les données des widgets pour remplir la structure parking
    strcpy(p.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
    g_print("Nom du parking : %s\n", p.nom);  // Débogage : Affichage du nom du parking

    strcpy(p.agent, "agent");  // L'agent est maintenant fixé à "agent"
    strcpy(p.rue, gtk_entry_get_text(GTK_ENTRY(input_rue)));
    g_print("Rue : %s\n", p.rue);  // Débogage : Affichage de la rue

    p.numero = atoi(gtk_entry_get_text(GTK_ENTRY(input_numero)));
    g_print("Numéro : %d\n", p.numero);  // Débogage : Affichage du numéro

    p.code_postal = atoi(gtk_entry_get_text(GTK_ENTRY(input_code_postal)));
    g_print("Code postal : %d\n", p.code_postal);  // Débogage : Affichage du code postal

    strcpy(p.ville, gtk_entry_get_text(GTK_ENTRY(input_ville)));
    g_print("Ville : %s\n", p.ville);  // Débogage : Affichage de la ville

    // Vérification des types
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_public))) {
        strcpy(p.type, "Public");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_prive))) {
        strcpy(p.type, "Privé");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_residentiel))) {
        strcpy(p.type, "Résidentiel");
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_commercial))) {
        strcpy(p.type, "Commercial");
    }
    g_print("Type : %s\n", p.type);  // Débogage : Affichage du type de parking

    p.nbr_place_disponible = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_places));
    g_print("Nombre de places disponibles : %d\n", p.nbr_place_disponible);  // Débogage : Nombre de places


    p.nbr_place_reserve = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_placesreserve));

    p.nbr_etages = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_nbr_etages));
    g_print("Nombre d'étages : %d\n", p.nbr_etages);  // Débogage : Nombre d'étages

    const gchar *text_acces = gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_type_acces));
    strcpy(p.type_acces, text_acces);
    g_print("Type d'accès : %s\n", p.type_acces);  // Débogage : Type d'accès

    // Sécurités
    char systeme_securite[100] = "";
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_camera))) {
        strcat(systeme_securite, "Caméras ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_alarme))) {
        strcat(systeme_securite, "Alarme ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_gardien))) {
        strcat(systeme_securite, "Gardien ");
    }
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_eclairage))) {
        strcat(systeme_securite, "Éclairage ");
    }
    strcpy(p.systeme_securite, systeme_securite);
    g_print("Système de sécurité : %s\n", p.systeme_securite);  // Débogage : Affichage du système de sécurité

    sprintf(p.heure_debut, "%02d:00", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure_debut)));
    sprintf(p.heure_fin, "%02d:00", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure_fin)));
    g_print("Heure début : %s, Heure fin : %s\n", p.heure_debut, p.heure_fin);  // Débogage : Heures

    const gchar *statut = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_statut));
    strcpy(p.statut_actuel, statut);
    g_print("Statut actuel : %s\n", p.statut_actuel);  // Débogage : Affichage du statut actuel

    // Appeler la fonction de modification
    if (modifier_parking("parking.txt", id_parking, p)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Modification réussie !");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Erreur lors de la modification.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void
on_RK_button_chercher_modifier_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *entry_id_modifier; 
    GtkWidget *label_result_modifier;
    const char *id_value;
    char buffer[256];
    int found = 0;

    // Récupération des widgets
    entry_id_modifier = lookup_widget(GTK_WIDGET(button), "RK_entry_id_modifier");
    label_result_modifier = lookup_widget(GTK_WIDGET(button), "RK_label_label_modifier");

    if (entry_id_modifier && label_result_modifier) {
        id_value = gtk_entry_get_text(GTK_ENTRY(entry_id_modifier));

        if (id_value && strlen(id_value) > 0) {
            FILE *file = fopen("parking.txt", "r");
            if (file) {
                while (fgets(buffer, sizeof(buffer), file)) {
                    char id[256];
                    sscanf(buffer, "%255[^,]", id); // Lire l'ID de la première colonne
                    if (strcmp(id, id_value) == 0) {
                        found = 1;
                        break;
                    }
                }
                fclose(file);
            } else {
                printf("Erreur: Impossible d'ouvrir le fichier parking.txt\n");
            }

            if (found) {
                // Afficher un message de succès dans le label
                gtk_label_set_text(GTK_LABEL(label_result_modifier), "ID trouvé : Veuillez modifier le parking.");

                printf("ID trouvé : Veuillez modifier le parking.\n");

                // Vérifier si le signal est déjà connecté pour éviter les doublons
                if (!g_signal_handlers_block_matched(button, G_SIGNAL_MATCH_FUNC, 0, 0, NULL, G_CALLBACK(on_RK_button_modifier_modifier_clicked), NULL)) {
                    gpointer user_data_id = g_strdup(id_value);  // Allouer une copie de l'ID
                    g_signal_connect(button, "clicked", G_CALLBACK(on_RK_button_modifier_modifier_clicked), user_data_id);
                }
            } else {
                // Afficher un message d'erreur dans le label
                gtk_label_set_text(GTK_LABEL(label_result_modifier), "ID non trouvé.");
                
                printf("ID non trouvé.\n");
            }
        } else {
            // Afficher un message d'erreur dans le label si l'ID est vide
            gtk_label_set_text(GTK_LABEL(label_result_modifier), "Veuillez entrer un ID valide.");
            
            printf("Veuillez entrer un ID valide.\n");
        }
    } else {
        printf("Erreur: Impossible de récupérer les widgets.\n");
    }
}


void
on_RK_button_fermer_afficher_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer la fenêtre parente (la fenêtre principale qui contient le bouton)
    GtkWidget *window11 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // Vérifier si l'objet est bien une fenêtre
    if (GTK_IS_WINDOW(window11)) {
        gtk_widget_destroy(window11); // Fermer la fenêtre
    }
}


void
on_RK_button_supprimer_afficher_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *window11 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkWidget *treeview = lookup_widget(window11, "RK_treeview_afficherparking_afficher");

    if (!treeview) {
        g_print("Erreur: TreeView non trouvé.\n");
        return;
    }

   
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
       
        char *id_parking_str;
        gtk_tree_model_get(model, &iter, 0, &id_parking_str, -1);
        g_print("Parking sélectionné avec ID: %s\n", id_parking_str);

     
        int id_parking = atoi(id_parking_str);

      
        int result = supprimer_parking("parking.txt", id_parking);

        if (result) {
            g_print("Le parking avec ID %d a été supprimé avec succès.\n", id_parking);

          
            gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
        } else {
            g_print("Erreur: Parking avec ID %d non trouvé.\n", id_parking);



            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window11),
                                                      GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_ERROR,
                                                      GTK_BUTTONS_OK,
                                                      "Le parking avec l'ID sélectionné n'a pas été trouvé.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }

      
        g_free(id_parking_str);
    } else {
 
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window11),
                                                  GTK_DIALOG_MODAL,
                                                  GTK_MESSAGE_ERROR,
                                                  GTK_BUTTONS_OK,
                                                  "Veuillez sélectionner un parking à supprimer.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}


void
on_RK_button_annuler_affecter_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer la fenêtre parente (la fenêtre principale qui contient le bouton)
    GtkWidget *window12 = gtk_widget_get_toplevel(GTK_WIDGET(button));
    
    // Vérifier si l'objet est bien une fenêtre
    if (GTK_IS_WINDOW(window12)) {
        gtk_widget_destroy(window12); // Fermer la fenêtre
    }
}


void
on_RK_button_affecter_affecter_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
// Récupérer les widgets
    GtkComboBox *combo_parking = GTK_COMBO_BOX(lookup_widget(GTK_WIDGET(button), "RK_combobox_affecter_affecter"));
    GtkComboBox *combo_agent = GTK_COMBO_BOX(lookup_widget(GTK_WIDGET(button), "RK_comboboxentry2_affecter_affecter"));
    GtkWidget *label_message = lookup_widget(GTK_WIDGET(button), "RK_label_message_affecter");

    if (combo_parking == NULL || combo_agent == NULL || label_message == NULL) {
        g_print("Erreur: Widgets non récupérés.\n");
        return;
    }

    // Récupérer les sélections
    const char *parking_id = gtk_combo_box_get_active_text(combo_parking);
    const char *agent_name = gtk_combo_box_get_active_text(combo_agent);

    if (parking_id == NULL || agent_name == NULL || strlen(parking_id) == 0 || strlen(agent_name) == 0) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Sélectionnez un parking et un agent.");
        return;
    }

    // Appeler la fonction d'affectation
    int result = affecter_agent_au_parking(parking_id, agent_name);

    // Gestion des résultats
    if (result == 1) {
        gtk_label_set_text(GTK_LABEL(label_message), "Affectation réussie : L'agent a été assigné au parking.");
    } else if (result == -1) {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : L'agent est déjà affecté à un autre parking.");
    } else {
        gtk_label_set_text(GTK_LABEL(label_message), "Erreur : Impossible d'affecter l'agent au parking.");
    }
}


void load_combo_box_from_files(GtkWidget *combobox1, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Supprimer le '\n' de la fin de chaque ligne
        line[strcspn(line, "\n")] = 0;

        // Extraire le premier élément avant la virgule
        char *token = strtok(line, ",");  // On sépare par la virgule

        if (token) {
            // Ajouter la première colonne dans le ComboBox
            gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), token);
        }
    }

    fclose(file);
}
void load_combo_box_from_file_for_agents(GtkWidget *combobox1, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        g_print("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Supprimer le '\n' de la fin de chaque ligne
        line[strcspn(line, "\n")] = 0;

        // Extraire le premier élément avant la virgule
        char *token = strtok(line, ",");  // On sépare par la virgule

        if (token) {
            // Ajouter la première colonne dans le ComboBox
            gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), token);
        }
    }

    fclose(file);
}

