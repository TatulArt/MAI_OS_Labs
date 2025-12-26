#include <stdio.h>
#include <string.h>

void print_menu() {
    printf("q - выход\n");
    printf("1 [a] [dx]- Расчет производной функции cos(x) в точке a с приращением dx \n");
    printf("2 [a] [b]- Подсчёт количества простых чисел на отрезке [a, b]\n\n");
}

int main() {
    char command[256];
    print_menu();
    
    while (1) {
        printf("> ");
        if (!fgets(command, sizeof(command), stdin)) break;
        
        command[strcspn(command, "\n")] = 0;
        
        if (strcmp(command, "q") == 0) {
            break;
        }
        
        if (command[0] == '1') {
            float A, deltaX;
            if (sscanf(command + 2, "%f %f", &A, &deltaX) == 2) {
                float result = Derivative(A, deltaX);
                printf("Производная cos(%.2f) = %.6f\n", A, result);
            } else {
                printf("Ожидалось 2 аргумента\n");
            }
        } else if (command[0] == '2') {
            int A, B;
            if (sscanf(command + 2, "%d %d", &A, &B) == 2) {
                int count = PrimeCount(A, B);
                if (count >= 0) {
                    printf("Простых чисел на отрезке [%d, %d]: %d\n", A, B, count);
                } else {
                    printf("Ошибка вычисления\n");
                }
            } else {
                printf("Ожидалось 2 аргумента\n");
            }
        } else {
            printf("Неизвестная команда\n");
        }
    }
    
    return 0;
}