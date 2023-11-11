#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void preprocessing(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == 'X') {
            string[i] = 'x';
        }
    }
}

void getValue(char term[], int* c, int* e) {
    int have_x = strchr(term, 'x') ? 1 : 0;
    int have_exp = strchr(term, '^') ? 1 : 0;
//     logic:
//    'x' and '^' => power >= 2
//    'x' and no '^' => pow == 1;
//    no 'x' and no '^' => pow == 0
    if (have_x && have_exp) {
        if (term[0] == 'x') {
            *c = 1;
            sscanf(term+2, "%d", e);
        } else {
            sscanf(term, "%dx^%d", c, e);
        }
    } else if (have_x && have_exp == 0) {
        *e = 1;
        if (term[0] == 'x') {
            *c = 1;
        } else {
            sscanf(term, "%d", c);
        }
    } else {
        *e = 0;
        sscanf(term, "%d", c);
    }
    
}

void readPoly(char poly[]) {
    preprocessing(poly);
    
    char poly_copy[100];
    strcpy(poly_copy, poly);
    
    char* token = strtok(poly, "+-");
    while (token) {
        int c = 0, e = 0;
        getValue(token, &c, &e);
        //if you have "A=" please develop yourself;
        char sign = token == poly ? '+' : poly_copy[token - poly - 1];
        printf("%d %d\n", c * (sign == '+' ? 1 : -1) , e);
        token = strtok(NULL, "+-");
    }
}

int main(void) {
    char input[100] = "x^3+2x^2+1";
    readPoly(input);
}