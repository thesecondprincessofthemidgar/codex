#include <stdio.h>
#include <math.h>
#include <time.h>

#define ITERATIONS 100000000

static double timed_float_ops() {
    float a = 1.234f, b = 5.678f, res = 0.f;
    clock_t start = clock();
    for (long i = 0; i < ITERATIONS; ++i)
        res += a * b;
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static double timed_double_ops() {
    double a = 1.234, b = 5.678, res = 0.0;
    clock_t start = clock();
    for (long i = 0; i < ITERATIONS; ++i)
        res += a * b;
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static void sin_accuracy() {
    double pi_fpu;
    __asm__ volatile ("fldpi\n\t" "fstpl %0" : "=m"(pi_fpu));
    double approx_pi[] = {3.14, 3.141596, pi_fpu};
    for (int i = 0; i < 3; ++i) {
        double pi = approx_pi[i];
        printf("sin(%f) = %.12f\n", pi, sin(pi));
        printf("sin(%f/2) = %.12f\n", pi, sin(pi/2));
    }
}

/*
 * Variant 23:
 *   Function: 2 * sin(x^2 - 5)
 *   Method:   bisection ("half division")
 */
static double func(double x) {
    return 2.0 * sin(x * x - 5.0);
}

static double bisection(double a, double b, int iters) {
    for (int i = 0; i < iters; ++i) {
        double mid = 0.5*(a+b);
        double fmid = func(mid);
        if (func(a)*fmid <= 0)
            b = mid;
        else
            a = mid;
    }
    return 0.5*(a+b);
}

int main() {
    printf("Float ops time: %f seconds\n", timed_float_ops());
    printf("Double ops time: %f seconds\n", timed_double_ops());

    printf("\nAccuracy tests:\n");
    sin_accuracy();

    /* find a root on [1,3] using 30 bisection iterations */
    double root = bisection(1.0, 3.0, 30);
    printf("\nRoot approximation: %.6f\n", root);
    return 0;
}
