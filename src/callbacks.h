#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

void on_buttonval_clicked(GtkButton *button, gpointer user_data);

#endif // CALLBACKS_H

void
on_buttonmodif_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonsupp_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonajout_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonvoir_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobuttonok_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobuttonnon_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_buttonchercher_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonmodife_clicked                (GtkButton       *button,
                                        gpointer         user_data);


// Update the function declaration to accept a GtkComboBox pointer
void load_combo_box_from_file(GtkWidget *combobox, const char *filename);



void
on_buttoncharge_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonannaj_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonquittsupp_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonannmodif_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonchargermod_clicked            (GtkButton       *button,
                                        gpointer         user_data);



void
on_buttonaffmodife_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonespacecitoyen_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttonespaceadmin_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdp_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdadp_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdsdp_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdc_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdr_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_buttongdaer_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_ajouter_main_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_afficher_main_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_affecter_main_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_modifier_main_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_annuler_ajouter_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_ajouter_ajouter_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_annuler_modifier_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_modifier_modifier_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_chercher_modifier_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_fermer_afficher_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_supprimer_afficher_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_annuler_affecter_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_RK_button_affecter_affecter_clicked (GtkButton       *button,
                                        gpointer         user_data);
void load_combo_box_from_files(GtkWidget *combobox1, const char *filename);
void load_combo_box_from_file_for_agents(GtkWidget *combobox1, const char *filename);
