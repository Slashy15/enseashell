#include "utils.h"
#include <unistd.h>
#include <string.h>

void write_message(const char *message) {
    write(STDOUT_FILENO, message, strnlen(message, BUFFER_SIZE));
}