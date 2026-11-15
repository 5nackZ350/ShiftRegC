/*
Shift Register and Counter Simulator in C
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SR_BITS 8

typedef struct { unsigned char q[SR_BITS]; } Sipo;

void sipo_clock(Sipo *r, int serial_in) {
    int i;
    for (i = SR_BITS - 1; i > 0; i--) r->q[i] = r->q[i - 1];
    r->q[0] = (unsigned char)serial_in;
}

void print_q(const unsigned char *q, int n) {
    int i;
    for (i = n - 1; i >= 0; i--) putchar('0' + q[i]);
}

void run_sipo(const char *bits) {
    Sipo r;
    int t, len = (int)strlen(bits);
    memset(&r, 0, sizeof r);
    printf("\nSIPO shift register (Q7...Q0)\n clk in Q\n");
    for (t = 0; t < len; t++) {
        sipo_clock(&r, bits[t] - '0');
        printf(" %3d   %c ", t + 1, bits[t]);
        print_q(r.q, SR_BITS);
        putchar('\n');
    }
}

void run_counter(int ticks) {
    unsigned char q[4];
    int t, value = 0, i;
    printf("\n4-bit up counter (Q3---Q0)\n clk   Q   dec\n");
    for (t = 0; t <= ticks; t++) {
        for (i = 0; i < 4; i++) q[i] = (unsigned char)((value >> i) & 1);
        printf(" %3d  ", t);
        print_q(q, 4);
        printf("   %3d\n", value);
        value = (value + 1) % 16;
    }
}

int main(void) {
    char bits[64], buf[32];
    int i, ok = 1, ticks;
    char *end;
    long parsed_ticks;

    printf("Enter a serial bit pattern (e.g. 10110011): ");
    if (fgets(bits, sizeof bits, stdin) == NULL) return 1;
    bits[strcspn(bits, "\n")] = '\0';
    for (i = 0; bits[i]; i++) {
        if (bits[i] != '0' && bits[i] != '1') ok = 0;
    }
    if (!ok || bits[0] == '\0') {
        printf("Use a nonempty pattern containing only 0 and 1.\n");
        return 1;
    }
    run_sipo(bits);

    printf("\nHow many clock ticks for the counter? ");
    if (fgets(buf, sizeof buf, stdin) == NULL) return 1;
    parsed_ticks = strtol(buf, &end, 10);
    while (*end == ' ' || *end == '\t' || *end == '\n') end++;
    if (end == buf || *end != '\0' || parsed_ticks < 0 || parsed_ticks > 64) {
        printf("Choose a whole number from 0 to 64.\n");
        return 1;
    }
    ticks = (int)parsed_ticks;
    run_counter(ticks);
    return 0;
}
