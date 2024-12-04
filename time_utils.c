#include "time_utils.h"
#include <stdio.h>
#include <time.h>

clock_t start_time, end_time;  // Variables globales pour les temps de début et de fin
long execution_time_ms = 0;    // Temps d'exécution en millisecondes

// Fonction pour démarrer le chronométrage
void start_timer() {
    start_time = clock();  // Démarre le chronomètre
    end_time = 0;          // Réinitialiser end_time à 0 pour chaque nouvelle commande
    execution_time_ms = 0; // Réinitialiser le temps d'exécution
}

// Fonction pour arrêter le chronométrage et calculer le temps écoulé
void stop_timer() {
    end_time = clock();

    // Calcul du temps écoulé en millisecondes
    execution_time_ms = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
}

// Fonction pour récupérer le temps d'exécution
long get_execution_time() {
    return execution_time_ms;
}

