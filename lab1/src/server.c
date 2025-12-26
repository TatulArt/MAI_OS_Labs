#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LEN 4096

void reverse_string(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }
    
    int len = strlen(str);
    
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    const char *filename = argv[1];
    
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }
    
    // Получаем PID для идентификации процесса
    pid_t my_pid = getpid();
    printf("[Server PID=%d] Начал работу. Файл: %s\n", my_pid, filename);
    fflush(stdout);
    
    char buffer[MAX_LEN];
    ssize_t bytes_read;
    
    // Читаем строки из стандартного ввода (который перенаправлен из pipe)
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0) {
        // Завершаем строку нулевым символом
        buffer[bytes_read] = '\0';
        
        if (buffer[bytes_read - 1] == '\n') {
            buffer[bytes_read - 1] = '\0';
            bytes_read--;
        }

        if (bytes_read == 0) {
            continue;
        }
        
        char original[MAX_LEN];
        strncpy(original, buffer, sizeof(original) - 1);
        original[sizeof(original) - 1] = '\0';
        
        reverse_string(buffer);

        char output[MAX_LEN * 2];
        int output_len = snprintf(output, sizeof(output),
                                 "[Server PID=%d] '%s'\n",
                                 my_pid, buffer);

        if (write(fd, output, output_len) == -1) {
            perror("Ошибка записи в файл");
        }
        
        if (write(STDOUT_FILENO, output, output_len) == -1) {
            perror("Ошибка вывода на экран");
        }
    }
    
    if (bytes_read == -1) {
        perror("Ошибка чтения из pipe");
    }
    
    close(fd);
    
    printf("[Server PID=%d] Завершил работу.\n", my_pid);
    fflush(stdout);
    
    return 0;
}