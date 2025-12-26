#include <math.h>
#include <stdlib.h>
#include <string.h>


float Derivative(float A, float deltaX) {
    return (cos(A + deltaX) - cos(A - deltaX)) / (2 * deltaX);
}

int PrimeCount(int A, int B) {
    if (A < 2) A = 2;
    if (B < 2) return 0;
    
    char *is_prime = (char*)malloc((B + 1) * sizeof(char));
    if (!is_prime) return -1;
    
    memset(is_prime, 1, (B + 1) * sizeof(char));
    is_prime[0] = is_prime[1] = 0;
    
    for (int i = 2; i * i <= B; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= B; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    
    int count = 0;
    for (int i = A; i <= B; i++) {
        if (is_prime[i]) {
            count++;
        }
    }
    
    free(is_prime);
    return count;
}