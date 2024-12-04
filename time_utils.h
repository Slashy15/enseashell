#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <time.h>

// Déclaration de la variable externe
extern long execution_time_ms;

// Fonction pour démarrer le chronométrage
void start_timer();

// Fonction pour arrêter le chronométrage et calculer le temps écoulé
void stop_timer();

// Fonction pour récupérer le temps d'exécution
long get_execution_time();

#endif


