#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

void print_menu() {
    printf("q - выход\n");
    printf("0 - Переключить библиотеку\n");
    printf("1 [a] [dx]- Расчет производной функции cos(x) в точке a с приращением dx \n");
    printf("2 [a] [b]- Подсчёт количества простых чисел на отрезке [a, b]\n\n");
}

int main(){
    void* handle = NULL;
    float (*Derivative)(float, float) = NULL;
    int (*PrimeCount)(int, int) = NULL;
    handle = dlopen("../libraries/lib1.so", RTLD_LAZY);

    if(!handle){
        perror("Ошибка загрузки библиотреки");
        return 1;
    }

    Derivative = dlsym(handle, "Derivative");
    PrimeCount = dlsym(handle,"PrimeCount");

    if(!Derivative || !PrimeCount){
        perror("Ошибка загрузки функции");
        dlclose(handle);
        return 1;
    }

    char command[256];
    int current_lib = 1;

    print_menu();

    while(1){
        printf("> ");

        if(!fgets(command, sizeof(command), stdin)){
            break;
        }
        command[strcspn(command,"\n")] = 0;

        if(strcmp(command,"q") == 0){
            break;
        }

        if(strcmp(command,"0") == 0){
            dlclose(handle);
            if(current_lib==1){
                handle = dlopen("../libraries/lib2.so", RTLD_LAZY);
                current_lib = 2;
            } else {
                handle = dlopen("../libraries/lib1.so", RTLD_LAZY);
                current_lib = 1;
            }

            if(!handle){
                perror("Ошибка загрузки библиотеки");
                return 1;
            }

            Derivative = dlsym(handle, "Derivative");
            PrimeCount = dlsym(handle,"PrimeCount");

            if(!Derivative || !PrimeCount){
                perror("Ошибка загрузки функции");
                dlclose(handle);
                return 1;
            } else {
                printf("Загружена библиотека %d\n", current_lib);
            }
        }

        if (command[0] == '1') {
            float A, deltaX;
            if (sscanf(command + 2, "%f %f", &A, &deltaX) == 2) {
                float result = Derivative(A, deltaX);
                printf("Производная cos(%.2f) = %.6f (реализация %d)\n", 
                       A, result, current_lib);
            } else {
                printf("Ожидалось 2 аргумента\n");
            }
        } 
        
        else if (command[0] == '2') {
            int A, B;
            if (sscanf(command + 2, "%d %d", &A, &B) == 2) {
                int count = PrimeCount(A, B);
                if (count >= 0) {
                    printf("Простых чисел на отрезке [%d, %d]: %d (реализация %d)\n", 
                           A, B, count, current_lib);
                }
            }
            else {
                printf("Ожидалось 2 аргумента\n");
            }
        } 
        
        else if (command[0] != '0'){
            printf("Неизвестная команда: %d\n", command[0]);
        }
        
    }
    if (handle) dlclose(handle);
    return 0;
}