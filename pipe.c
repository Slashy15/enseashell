#include "pipe.h"
#include "utils.h"
#include "shell.h"
#include "time_utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 128

// Fonction pour séparer les commandes avant et après un pipe
void handle_pipe(char *input) {
    char *cmd1[BUFFER_SIZE];
    char *cmd2[BUFFER_SIZE];
    int i = 0;

    // Diviser l'entrée en deux commandes en utilisant le pipe
    char *pipe_pos = strchr(input, '|');
    if (pipe_pos == NULL) {
        write_message("Error: No pipe found.\n");
        return;
    }

    // Séparer les deux parties de la commande
    *pipe_pos = '\0';  // Remplacer le pipe par un caractère nul
    char *cmd1_str = input;
    char *cmd2_str = pipe_pos + 1;

    // Parse la première commande
    parse_input(cmd1_str, cmd1);
    // Parse la deuxième commande
    parse_input(cmd2_str, cmd2);

    // Création du pipe
    int pipefds[2];
    if (pipe(pipefds) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1, pid2;

    // Premier processus (avant le pipe)
    if ((pid1 = fork()) == 0) {
        // Rediriger la sortie standard vers le pipe
        dup2(pipefds[1], STDOUT_FILENO);
        close(pipefds[0]);
        close(pipefds[1]);
        execvp(cmd1[0], cmd1);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Deuxième processus (après le pipe)
    if ((pid2 = fork()) == 0) {
        // Rediriger l'entrée standard depuis le pipe
        dup2(pipefds[0], STDIN_FILENO);
        close(pipefds[0]);
        close(pipefds[1]);
        execvp(cmd2[0], cmd2);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    // Fermer les descripteurs de pipe dans le parent
    close(pipefds[0]);
    close(pipefds[1]);

    // Attendre que les deux processus se terminent
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

