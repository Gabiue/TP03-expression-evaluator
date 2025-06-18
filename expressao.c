#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_STACK 100
#define PI 3.14159265359

// Implementações matemáticas manuais para evitar dependência de math.h
double mypow(double base, double exp) {
    if (exp == 0) return 1;
    if (exp == 1) return base;
    if (exp < 0) return 1.0 / mypow(base, -exp);
    
    double result = 1;
    int i;
    for (i = 0; i < (int)exp; i++) {
        result *= base;
    }
    return result;
}

double mysqrt(double x) {
    if (x < 0) return 0;
    if (x == 0 || x == 1) return x;
    
    double guess = x / 2.0;
    double prev_guess;
    int iterations = 0;
    
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2.0;
        iterations++;
    } while (iterations < 50 && (guess - prev_guess > 0.000001 || prev_guess - guess > 0.000001));
    
    return guess;
}

double mysin(double x) {
    // Converter para radianos e normalizar
    x = x * PI / 180.0;
    while (x > 2 * PI) x -= 2 * PI;
    while (x < 0) x += 2 * PI;
    
    // Série de Taylor para sin(x)
    double result = 0;
    double term = x;
    int i;
    
    for (i = 1; i <= 15; i += 2) {
        result += term;
        term *= -x * x / ((i + 1) * (i + 2));
    }
    
    return result;
}

double mycos(double x) {
    // Converter para radianos e normalizar
    x = x * PI / 180.0;
    while (x > 2 * PI) x -= 2 * PI;
    while (x < 0) x += 2 * PI;
    
    // Série de Taylor para cos(x)
    double result = 1;
    double term = 1;
    int i;
    
    for (i = 2; i <= 16; i += 2) {
        term *= -x * x / ((i - 1) * i);
        result += term;
    }
    
    return result;
}

double mytan(double x) {
    double cosVal = mycos(x);
    if (cosVal == 0) return 0; // Evitar divisão por zero
    return mysin(x) / cosVal;
}

double mylog10(double x) {
    if (x <= 0) return 0;
    if (x == 1) return 0;
    if (x == 10) return 1;
    
    // Aproximação usando log natural via série de Taylor
    // log(1+x) = x - x²/2 + x³/3 - x⁴/4 + ...
    
    // Normalizar x para estar próximo de 1
    double exp = 0;
    while (x >= 10) {
        x /= 10;
        exp++;
    }
    while (x < 1) {
        x *= 10;
        exp--;
    }
    
    // Agora x está entre 1 e 10
    x = x - 1; // Para usar na série log(1+x)
    
    double ln_result = 0;
    double term = x;
    int i;
    
    for (i = 1; i <= 20; i++) {
        if (i % 2 == 1) {
            ln_result += term / i;
        } else {
            ln_result -= term / i;
        }
        term *= x;
    }
    
    // Converter ln para log10: log10(x) = ln(x) / ln(10)
    // ln(10) ≈ 2.302585
    return (ln_result + exp * 2.302585) / 2.302585;
}

double myfmod(double x, double y) {
    if (y == 0) return 0;
    return x - ((int)(x/y)) * y;
}

// Estrutura para pilha de caracteres
typedef struct {
    char items[MAX_STACK][50];
    int top;
} CharStack;

// Estrutura para pilha de números
typedef struct {
    float items[MAX_STACK];
    int top;
} FloatStack;

// Funções auxiliares para pilha de caracteres
void initCharStack(CharStack *stack) {
    stack->top = -1;
}

int isCharStackEmpty(CharStack *stack) {
    return stack->top == -1;
}

int isCharStackFull(CharStack *stack) {
    return stack->top == MAX_STACK - 1;
}

void pushChar(CharStack *stack, char *item) {
    if (!isCharStackFull(stack)) {
        stack->top++;
        strcpy(stack->items[stack->top], item);
    }
}

char *popChar(CharStack *stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return NULL;
}

char *topChar(CharStack *stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top];
    }
    return NULL;
}

// Funções auxiliares para pilha de números
void initFloatStack(FloatStack *stack) {
    stack->top = -1;
}

int isFloatStackEmpty(FloatStack *stack) {
    return stack->top == -1;
}

int isFloatStackFull(FloatStack *stack) {
    return stack->top == MAX_STACK - 1;
}

void pushFloat(FloatStack *stack, float item) {
    if (!isFloatStackFull(stack)) {
        stack->top++;
        stack->items[stack->top] = item;
    }
}

float popFloat(FloatStack *stack) {
    if (!isFloatStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return 0.0;
}

// Função para verificar se é operador binário
int isOperator(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || 
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 || 
            strcmp(token, "%") == 0 || strcmp(token, "^") == 0);
}

// Função para verificar se é função unária
int isFunction(char *token) {
    return (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 || 
            strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || 
            strcmp(token, "log") == 0);
}

// Função para obter precedência de operadores
int getPrecedence(char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    if (isFunction(op)) return 4;
    return 0;
}

// Função para verificar se operador é associativo à esquerda
int isLeftAssociative(char *op) {
    return strcmp(op, "^") != 0; // Apenas ^ é associativo à direita
}

// Função para verificar se é número
int isNumber(char *token) {
    char *endptr;
    strtod(token, &endptr);
    return *endptr == '\0';
}

// Função para aplicar operação binária
float applyBinaryOperation(char *op, float a, float b) {
    if (strcmp(op, "+") == 0) return a + b;
    if (strcmp(op, "-") == 0) return a - b;
    if (strcmp(op, "*") == 0) return a * b;
    if (strcmp(op, "/") == 0) return (b != 0) ? a / b : 0;
    if (strcmp(op, "%") == 0) return (b != 0) ? myfmod(a, b) : 0;
    if (strcmp(op, "^") == 0) return mypow(a, b);
    return 0;
}

// Função para aplicar função unária
float applyUnaryFunction(char *func, float a) {
    if (strcmp(func, "raiz") == 0) return mysqrt(a);
    if (strcmp(func, "sen") == 0) return mysin(a);
    if (strcmp(func, "cos") == 0) return mycos(a);
    if (strcmp(func, "tg") == 0) return mytan(a);
    if (strcmp(func, "log") == 0) return mylog10(a);
    return 0;
}

// Função para tokenizar string
void tokenize(char *str, char tokens[][50], int *tokenCount) {
    *tokenCount = 0;
    char *token = strtok(str, " ");
    while (token != NULL && *tokenCount < MAX_STACK) {
        strcpy(tokens[*tokenCount], token);
        (*tokenCount)++;
        token = strtok(NULL, " ");
    }
}

// Implementação da conversão de infixa para pós-fixa
char *getFormaPosFixa(char *Str) {
    static char result[512];
    result[0] = '\0';
    
    char temp[512];
    strcpy(temp, Str);
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    CharStack stack;
    initCharStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            // Número: adicionar à saída
            if (strlen(result) > 0) strcat(result, " ");
            strcat(result, token);
        }
        else if (isFunction(token)) {
            // Função: empilhar
            pushChar(&stack, token);
        }
        else if (strcmp(token, "(") == 0) {
            // Parêntese esquerdo: empilhar
            pushChar(&stack, token);
        }
        else if (strcmp(token, ")") == 0) {
            // Parêntese direito: desempilhar até encontrar (
            while (!isCharStackEmpty(&stack) && strcmp(topChar(&stack), "(") != 0) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
            if (!isCharStackEmpty(&stack)) {
                popChar(&stack); // Remove o (
            }
            // Se há função no topo, desempilhar
            if (!isCharStackEmpty(&stack) && isFunction(topChar(&stack))) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
        }
        else if (isOperator(token)) {
            // Operador: desempilhar operadores com precedência maior ou igual
            while (!isCharStackEmpty(&stack) && 
                   strcmp(topChar(&stack), "(") != 0 &&
                   (getPrecedence(topChar(&stack)) > getPrecedence(token) ||
                    (getPrecedence(topChar(&stack)) == getPrecedence(token) && isLeftAssociative(token)))) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
            pushChar(&stack, token);
        }
    }
    
    // Desempilhar operadores restantes
    while (!isCharStackEmpty(&stack)) {
        if (strlen(result) > 0) strcat(result, " ");
        strcat(result, popChar(&stack));
    }
    
    return result;
}

// Implementação da conversão de pós-fixa para infixa
char *getFormaInFixa(char *Str) {
    static char result[512];
    result[0] = '\0';
    
    char temp[512];
    strcpy(temp, Str);
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    CharStack stack;
    initCharStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            // Número: empilhar
            pushChar(&stack, token);
        }
        else if (isOperator(token)) {
            // Operador binário: desempilhar dois operandos
            if (stack.top >= 1) {
                char *operand2 = popChar(&stack);
                char *operand1 = popChar(&stack);
                
                char expression[100];
                snprintf(expression, sizeof(expression), "(%s %s %s)", operand1, token, operand2);
                pushChar(&stack, expression);
            }
        }
        else if (isFunction(token)) {
            // Função unária: desempilhar um operando
            if (stack.top >= 0) {
                char *operand = popChar(&stack);
                
                char expression[100];
                snprintf(expression, sizeof(expression), "%s(%s)", token, operand);
                pushChar(&stack, expression);
            }
        }
    }
    
    if (!isCharStackEmpty(&stack)) {
        strcpy(result, topChar(&stack));
    }
    
    return result;
}

// Implementação da avaliação de expressão pós-fixa
float getValorPosFixa(char *StrPosFixa) {
    char temp[512];
    strcpy(temp, StrPosFixa);
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    FloatStack stack;
    initFloatStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            // Número: empilhar
            float num = atof(token);
            pushFloat(&stack, num);
        }
        else if (isOperator(token)) {
            // Operador binário: desempilhar dois operandos
            if (stack.top >= 1) {
                float operand2 = popFloat(&stack);
                float operand1 = popFloat(&stack);
                float result = applyBinaryOperation(token, operand1, operand2);
                pushFloat(&stack, result);
            }
        }
        else if (isFunction(token)) {
            // Função unária: desempilhar um operando
            if (stack.top >= 0) {
                float operand = popFloat(&stack);
                float result = applyUnaryFunction(token, operand);
                pushFloat(&stack, result);
            }
        }
    }
    
    return isFloatStackEmpty(&stack) ? 0.0 : popFloat(&stack);
}

// Implementação da avaliação de expressão infixa
float getValorInFixa(char *StrInFixa) {
    char *posFixa = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(posFixa);
}