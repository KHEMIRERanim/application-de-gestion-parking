#include <gtk/gtk.h>

typedef struct {
    int id;
    char nom[50];
    char rue[50];
    int numero;
    int code_postal;
    char ville[50];
    char type[20];
    int nbr_place_disponible;
    int nbr_place_reserve;
    int nbr_etages;
    char type_acces[50];
    char systeme_securite[50];
    char heure_debut[6];
    char heure_fin[6];
    char statut_actuel[20];
    char agent[50];
} parking;
int ajouter_parking(char *filename, parking p);
int charger_parkings(parking *parkings, int max_parkings);
void afficher_parkings_in_treeview(GtkTreeView *treeview, parking *parkings, int count);
int supprimer_parking(char *filename, int id_parking) ;
int modifier_parking(char *filename, int id_parking, parking p);

int affecter_agent_au_parking(const char *parking_id, const char *agent_name);

