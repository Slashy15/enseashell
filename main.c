#include "shell.h"
#include "utils.h"

int main() {
    // Message d'accueil
    write_message("Welcome to the Shell of ENSEA.\nTo leave, write 'exit'.\n");

    // Boucle principale
    shell_loop();

    write_message("Bye bye!\n");
    return 0;
}