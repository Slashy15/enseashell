#include "shell.h"
#include "pipe.h"
#include "utils.h"
#include "time_utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128

int last_exit_status = -1;  // Code de sortie de la dernière commande
int last_signal = -1;       // Signal de la dernière commande

// Fonction pour afficher le prompt avec le code de retour, le signal et le temps d'exécution
void display_prompt() {
    if (last_exit_status >= 0) {
        write_message("enseash [exit:");
        char exit_code[16];
        sprintf(exit_code, "%d", last_exit_status);
        write_message(exit_code);
        write_message("] %% ");
    } else if (last_signal >= 0) {
        write_message("enseash [sign:");
        char signal_code[16];
        sprintf(signal_code, "%d", last_signal);
        write_message(signal_code);
        write_message("] %% ");
    } else {
        write_message("enseash %% ");
    }
}

// Fonction pour découper la commande en arguments
int parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;  // Le dernier argument doit être NULL pour execvp
    return i;
}

void shell_loop() {
    char input[BUFFER_SIZE];
    char *args[BUFFER_SIZE];

    while (1) {
        display_prompt();

        ssize_t bytes_read = read(STDIN_FILENO, input, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            write_message("Error of reading.\n");
            continue;
        }

        input[bytes_read - 1] = '\0';

        if (strncmp(input, "exit", BUFFER_SIZE) == 0) {
            break;  // Quitter le shell
        }

        if (bytes_read == 1) {
            continue;  // Ignorer les entrées vides
        }

        int num_args = parse_input(input, args);

        if (num_args == 0) {
            continue;
        }

        // Vérifier si la commande contient un pipe
        for (int i = 0; i < num_args; i++) {
            if (strcmp(args[i], "|") == 0) {
                handle_pipe(input);  // Gérer les pipes
                break;
            }
        }

        // Si la commande ne contient pas de pipe, l'exécuter normalement
        pid_t pid = fork();
        if (pid < 0) {
            write_message("Error to create the process.\n");
            continue;
        } else if (pid == 0) {
            start_timer();  // Démarre le chronomètre

            if (execvp(args[0], args) == -1) {
                write_message("Error: Command doesn't exist, please retry.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);
            stop_timer();  // Arrête le chronomètre

            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);
                last_signal = -1;
            } else if (WIFSIGNALED(status)) {
                last_exit_status = -1;
                last_signal = WTERMSIG(status);
            }
        }
    }
}
