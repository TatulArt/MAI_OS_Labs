#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE_LEN 4096

ssize_t read_line(int fd, char *buffer, size_t max_len) {
    ssize_t total_read = 0;
    char c;
    
    while (total_read < max_len - 1) {
        ssize_t bytes_read = read(fd, &c, 1);
        if (bytes_read <= 0) {
            return bytes_read; // EOF или ошибка
        }
        if (c == '\n') {
            break;
        }
        buffer[total_read++] = c;
    }
    
    buffer[total_read] = '\0';
    return total_read;
}

int main() {
    char filename1[256], filename2[256];

    printf("Введите имя файла для child1: ");
    fflush(stdout);
    if (fgets(filename1, sizeof(filename1), stdin) == NULL) {
        perror("Ошибка чтения имени файла 1");
        exit(EXIT_FAILURE);
    }

    filename1[strcspn(filename1, "\n")] = '\0';

    printf("Введите имя файла для child2: ");
    fflush(stdout);
    if (fgets(filename2, sizeof(filename2), stdin) == NULL) {
        perror("Ошибка чтения имени файла 2");
        exit(EXIT_FAILURE);
    }
    filename2[strcspn(filename2, "\n")] = '\0';

    // Создаем два канала (pipe)
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Ошибка создания pipe");
        exit(EXIT_FAILURE);
    }

    // child1
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Ошибка fork для child1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Закрываем неиспользуемые концы pipe
        close(pipe1[1]);  // Закрываем конец записи pipe1
        close(pipe2[0]);  // Закрываем оба конца pipe2
        close(pipe2[1]);
        
        // Перенаправляем стандартный ввод на чтение из pipe1
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);
        
        execl("./server", "server", filename1, NULL);

        perror("Ошибка запуска server для child1");
        exit(EXIT_FAILURE);
    }

    // child2
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Ошибка fork для child2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(pipe2[1]);  // Закрываем конец записи pipe2
        close(pipe1[0]);  // Закрываем оба конца pipe1
        close(pipe1[1]);
        
        // Перенаправляем стандартный ввод на чтение из pipe2
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        execl("./server", "server", filename2, NULL);

        perror("Ошибка запуска server для child2");
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]);
    close(pipe2[0]);

    printf("\nВводите строки (Только латинские буквы. Ctrl+D для завершения):\n");
    fflush(stdout);

    char line[MAX_LINE_LEN];
    int line_number = 0;
    ssize_t line_length;

    // Читаем строки от пользователя и отправляем в соответствующие pipe
    while ((line_length = read_line(STDIN_FILENO, line, sizeof(line))) > 0) {
        line_number++;
        
        int target_pipe;
        const char *child_name;
        
        if (line_number % 2 == 1) { // Нечетная строка
            target_pipe = pipe1[1];
            child_name = "child1";
        } else { // Четная строка
            target_pipe = pipe2[1];
            child_name = "child2";
        }
        
        // Отправляем строку в pipe
        write(target_pipe, line, line_length);
        write(target_pipe, "\n", 1);
        
        printf("[Родитель] Строка %d отправлена в %s: %s\n", 
               line_number, child_name, line);
        fflush(stdout);
    }

    // Закрываем концы pipe для записи
    close(pipe1[1]);
    close(pipe2[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Родительский процесс завершен.\n");
    return 0;
}