#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO1 "/tmp/mi_fifo"

int main() {
    int fd;
    char saludo[] = "Un saludo !!!!!\n", buffer [10];
    // Creamos el FIFO
    if (mkfifo(FIFO1, 0666) == -1) {
        perror("Error creando el FIFO");
        exit(EXIT_FAILURE);
    }
    // Abrimos el FIFO para escribir
    fd = open(FIFO1, O_WRONLY);
    if (fd == -1) {
        perror("Error abriendo el FIFO");
        exit(EXIT_FAILURE);
    }
    // Escribimos el mensaje en el FIFO
    write(fd, mensaje, strlen(mensaje) + 1);
    printf("Mensaje enviado: %s\n", mensaje);
    close(fd);

    return 0;
}