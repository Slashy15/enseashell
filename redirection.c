#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Fonction pour gérer les redirections de sortie (>)
int handle_output_redirection(char **args) {
    int i = 0;
    int output_file = -1;

    // Chercher le symbole ">" et traiter la redirection de sortie
    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            // Si nous avons trouvé ">", obtenir le nom du fichier
            if (args[i + 1] != NULL) {
                // Ouvrir le fichier en mode écriture (création ou écrasement)
                output_file = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_file == -1) {
                    perror("Error opening output file");
                    return -1;  // Erreur d'ouverture du fichier
                }

                // Fermer le symbole ">" et réarranger les arguments
                args[i] = NULL;
                break;
            } else {
                fprintf(stderr, "Syntax error: missing output file after '>'\n");
                return -1;
            }
        }
        i++;
    }

    // Rediriger la sortie standard vers le fichier
    if (output_file != -1) {
        if (dup2(output_file, STDOUT_FILENO) == -1) {
            perror("Error redirecting stdout");
            close(output_file);
            return -1;
        }
        close(output_file);
    }
    return 0;
}

// Fonction pour gérer les redirections d'entrée (<)
int handle_input_redirection(char **args) {
    int i = 0;
    int input_file = -1;

    // Chercher le symbole "<" et traiter la redirection d'entrée
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // Si nous avons trouvé "<", obtenir le nom du fichier
            if (args[i + 1] != NULL) {
                // Ouvrir le fichier en mode lecture
                input_file = open(args[i + 1], O_RDONLY);
                if (input_file == -1) {
                    perror("Error opening input file");
                    return -1;  // Erreur d'ouverture du fichier
                }

                // Fermer le symbole "<" et réarranger les arguments
                args[i] = NULL;
                break;
            } else {
                fprintf(stderr, "Syntax error: missing input file after '<'\n");
                return -1;
            }
        }
        i++;
    }

    // Rediriger l'entrée standard depuis le fichier
    if (input_file != -1) {
        if (dup2(input_file, STDIN_FILENO) == -1) {
            perror("Error redirecting stdin");
            close(input_file);
            return -1;
        }
        close(input_file);
    }
    return 0;
}

// Fonction pour gérer les redirections (entrée et sortie)
int handle_redirections(char **args) {
    int result = 0;

    // Gérer la redirection de la sortie (>)
    result = handle_output_redirection(args);
    if (result == -1) {
        return -1;
    }

    // Gérer la redirection de l'entrée (<)
    result = handle_input_redirection(args);
    if (result == -1) {
        return -1;
    }

    return 0;
}

