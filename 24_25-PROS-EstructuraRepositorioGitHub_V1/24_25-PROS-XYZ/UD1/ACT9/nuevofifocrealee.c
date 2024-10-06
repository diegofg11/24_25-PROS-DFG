#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO1 "/tmp/mi_fifo"

int main() {
    int fd;
    char buffer[100];
    // Abrimos el FIFO para leerlo
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el FIFO");
        exit(EXIT_FAILURE);
    }
    // Leemos el mensaje del FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Mensaje recibido: %s\n", buffer);
    close(fd);

    return 0;
}
