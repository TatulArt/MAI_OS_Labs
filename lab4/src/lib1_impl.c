#include <math.h>


float Derivative(float A, float deltaX) {
    return (cos(A + deltaX) - cos(A)) / deltaX;
}

int PrimeCount(int A, int B) {
    if (A < 2) A = 2;
    if (B < 2) return 0;
    
    int count = 0;
    for (int i = A; i <= B; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            count++;
        }
    }
    return count;
}