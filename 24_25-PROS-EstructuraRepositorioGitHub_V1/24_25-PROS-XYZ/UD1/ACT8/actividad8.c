#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd1[2], fd2[2];
    pid_t pid_hijo, pid_nieto;
    char mensaje_abuelo[] = "Mensaje del abuelo";
    char buffer[100];

    //Utilizamos fd1 para la comunicación del abuelo con el hijo, y fd2 para la comunicación del hijo con el nieto

    // Creamos las tuberías (fd1, fd2)
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Error creando las tuberías");
        exit(EXIT_FAILURE);
    }

    // Creamos el proceso hijo
    pid_hijo = fork();

    if (pid_hijo < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid_hijo == 0) {

        // Creamos el proceso nieto
        pid_nieto = fork();

        if (pid_nieto < 0) {
            perror("Error al crear el proceso nieto");
            exit(EXIT_FAILURE);
        }

        if (pid_nieto == 0) {
            close(fd2[1]);
            // Para leer el mensaje del hijo
            read(fd2[0], buffer, sizeof(buffer));
            // e imprimirlo 
            printf("Nieto recibe: %s\n", buffer);
            strcat(buffer, " y modificado por el nieto");
            close(fd2[0]);

            // DEvolvemos el mensaje al hijo
            close(fd1[0]);
            write(fd1[1], buffer, strlen(buffer) + 1); 
            close(fd1[1]); 
            exit(0);
        } else {
            close(fd1[1]);
            // Para leer el mensaje del abuelo
            read(fd1[0], buffer, sizeof(buffer));
            // e imprimirlo
            printf("Hijo recibe del abuelo: %s\n", buffer);
            strcat(buffer, " y modificado por el hijo");
            close(fd1[0]);

            // Enviamos el mensaje al nieto
            close(fd2[0]);
            write(fd2[1], buffer, strlen(buffer) + 1);
            close(fd2[1]);

            // Esperamos la respuesta del nieto
            wait(NULL);

            // Leemos la respuesta
            close(fd1[1]);
            read(fd1[0], buffer, sizeof(buffer));
            printf("Hijo recibe del nieto: %s\n", buffer);

            // La enviamos al abuelo
            close(fd2[0]);
            write(fd2[1], buffer, strlen(buffer) + 1);
            close(fd2[1]);
            exit(0);
        }
    } else {
        close(fd1[0]);
        // Enviamos el mensaje al hijo
        write(fd1[1], mensaje_abuelo, strlen(mensaje_abuelo) + 1);
        close(fd1[1]);
        wait(NULL);

        // Leemos la respuesta del hijo
        close(fd2[1]);
        read(fd2[0], buffer, sizeof(buffer));
        printf("Abuelo recibe la respuesta: %s\n", buffer);
        close(fd2[0]);
    }

    return 0;
}

/*
1. El abuelo crea al hijo mediante fork().
2. El hijo hace lo mismo para crear al nieto.
Para comunicarse:
1. El abuelo envía un mensaje al hijo a través de fd1.
2. El hijo lee el mensaje, lo modifica y se lo pasa al nieto a través de fd2.
3. El nieto recibe el mensaje, lo modifica y lo reenvía al hijo.
4. El hijo reenvía la respuesta al abuelo.
*/