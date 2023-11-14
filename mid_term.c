#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


struct node {
    int coefficient;
    int exponent;
    struct node *next;
};

struct poly {
    char name[100];
    struct node *Terms;
    
};
typedef struct node Node;
typedef struct poly Poly;

Poly *STORAGE[100];
int polyCounter = 0;

Node *createNode(int c,int e) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->coefficient = c;
    newNode->exponent = e;
    newNode->next = NULL;
    return newNode;
}
Poly *createPoly(char* name) {
    Poly *newPoly = (Poly *) malloc(sizeof(Poly));
    strcpy(newPoly->name, name);
    newPoly->Terms = NULL;
    STORAGE[polyCounter++] = newPoly;
    return newPoly;
}

int find(char* name) {
    for (int i = 0; i < sizeof(STORAGE); i++) {
        if (STORAGE[i] == NULL) {
            continue;
        }
        if (strcmp(STORAGE[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}


int append(Node **head, int c, int e) {
    Node *newNode = createNode(c, e);
    
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node *htemp = *head;
        Node *ptemp = htemp;
        while (htemp->next != NULL) {
            ptemp = htemp;
            htemp = htemp->next;
        }
        htemp->next = newNode;
    }
    return 0;
}

void append_by_name(char* name, int c, int e) {
    int index = find(name);
    if (index != -1) {
        append(&STORAGE[index]->Terms, c, e);
    } else {
        printf("cant found");
    }
    return;
    
}
void checkList(Node **head) {
    Node *htemp = *head;
    while (htemp != NULL) {
        printf("%d %d\n", htemp->coefficient, htemp->exponent);
        htemp = htemp->next;
    }
}

int printPoly(Poly* P) {
    if (P == NULL) return -1;
    printf("%s\n", P->name);
    checkList(&P->Terms);
    return 0;
}

void printPoly_advance(Poly* poly) {
    int flag_first = 0;
    int c = 0, e = 0;
    char name[100]; strcpy(name, poly->name);
    char right_part[1000] = {0};
    Node *htemp = poly->Terms;
    while (htemp != NULL) {
        c = htemp->coefficient;
        e = htemp->exponent;
        if (abs(c) == 1 && e >= 1) { // partten 1 -x^ +x^ 
            if (e >= 2) {
                sprintf(right_part + strlen(right_part), "%cx^%d", c < 0 ? '-' : '+', e);
            }
            if (e == 1) {
                sprintf(right_part + strlen(right_part), "%cx", c < 0 ? '-' : '+');
            }
        }
        if (abs(c) != 1 && e >= 1) {  // partten 2 -3x^ +3x^
            if (e >= 2) {
                sprintf(right_part + strlen(right_part), "%+dx^%d", c, e);
            }
            if (e == 1) {
                sprintf(right_part + strlen(right_part), "%+dx", c);
            }
        }
        if (e == 0) {
            sprintf(right_part + strlen(right_part), "%+d", c);
        }
        
        // partten 3 -3
        htemp = htemp->next;
    }

    printf("%s=%s\n", name, right_part[0] == '+' ? right_part+1 : right_part);   
}




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

Poly* readPoly(char poly[]) {
    int flag = 0;
    preprocessing(poly);
    
    char poly_copy[100] = {0};
    strcpy(poly_copy, poly);
    
    char* ptr = strchr(poly, '=');
    int index = (int) (ptr - poly);
    char name[100] = {0};
    strncpy(name, poly, index);
    name[index] = '\0';
    Poly* P = createPoly(name);
        
    
    char* token = strtok(ptr + 1, "+-");
    int compare = 100000;
    while (token) {
        int c = 0, e = 0;
        getValue(token, &c, &e);
        if (e >= compare) {
            printf("ERROR\n");
            free(P);
            polyCounter--;
            return NULL;
        }
        compare = e;
        
        char sign = poly_copy[token - poly - 1];
        append(&P->Terms, c * (sign == '=' || sign == '+' ? 1 : -1), e);
        
        token = strtok(NULL, "+-");
    }
    return P;
}

void printPoly_by_name(char* name) {
    int index = find(name);
    if (index != -1) {
        Poly* P = STORAGE[index];
        printPoly_advance(P);
    } else {
        printf("404 not found\n");
    }
    return;
}

void printAllPoly(void) {
    for (int i = 0; i < sizeof(STORAGE); i++) {
        if (STORAGE[i] == NULL) {
            continue;
        }
        Poly* P = STORAGE[i];
        printPoly_advance(P);
    }
}

int main(void) {
    int choose = 0;
    char input[100];
    while (choose < 6) {
        scanf("%d", &choose);
        switch (choose) {
        case 1: 
            while (scanf("%99s", input) != EOF && input[0] != '0') {
                Poly* P = readPoly(input);
                printPoly(P);
            }
            printf("quit\n");
            break;
        case 2:
            printAllPoly();
            break;
        default:
            printf("end\n");
            break;
        }
    }
    
    
    
    return 0;
}








//輸出, 選最大,選次大
//加法,
//減法(*-1)
//乘法



