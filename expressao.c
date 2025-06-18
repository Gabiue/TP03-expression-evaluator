#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_STACK 100
#define PI 3.14159265358979323846
#define EPSILON 1e-10

// ====================== ESTRUTURAS DE PILHA ======================
typedef struct {
    char items[MAX_STACK][256];
    int top;
} CharStack;

typedef struct {
    float items[MAX_STACK];
    int top;
} FloatStack;

// ====================== FUNÇÕES DE PILHA ======================
void initCharStack(CharStack *stack) {
    stack->top = -1;
}

void initFloatStack(FloatStack *stack) {
    stack->top = -1;
}

int isCharStackEmpty(CharStack *stack) {
    return stack->top == -1;
}

int isFloatStackEmpty(FloatStack *stack) {
    return stack->top == -1;
}

int isCharStackFull(CharStack *stack) {
    return stack->top >= MAX_STACK - 1;
}

int isFloatStackFull(FloatStack *stack) {
    return stack->top >= MAX_STACK - 1;
}

void pushChar(CharStack *stack, const char *item) {
    if (!isCharStackFull(stack)) {
        stack->top++;
        strcpy(stack->items[stack->top], item);
    }
}

void pushFloat(FloatStack *stack, float item) {
    if (!isFloatStackFull(stack)) {
        stack->top++;
        stack->items[stack->top] = item;
    }
}

char *popChar(CharStack *stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return NULL;
}

float popFloat(FloatStack *stack) {
    if (!isFloatStackEmpty(stack)) {
        return stack->items[stack->top--];
    }
    return 0.0f;
}

char *topChar(CharStack *stack) {
    if (!isCharStackEmpty(stack)) {
        return stack->items[stack->top];
    }
    return NULL;
}

// ====================== IMPLEMENTAÇÕES MATEMÁTICAS MELHORADAS ======================
double myPow(double base, double exp) {
    if (exp == 0.0) return 1.0;
    if (base == 0.0) return 0.0;
    if (exp == 1.0) return base;
    if (base == 1.0) return 1.0;
    
    if (exp < 0) return 1.0 / myPow(base, -exp);
    
    // Para expoentes inteiros, usar exponenciação binária
    if (exp == (int)exp) {
        double result = 1.0;
        double current = base;
        int n = (int)exp;
        
        while (n > 0) {
            if (n % 2 == 1) result *= current;
            current *= current;
            n /= 2;
        }
        return result;
    }
    
    // Para expoentes fracionários, aproximação simples
    double intPart = (int)exp;
    double fracPart = exp - intPart;
    
    double result = myPow(base, intPart);
    
    // Aproximação da parte fracionária: x^f ≈ 1 + f*ln(x)
    if (fracPart != 0.0 && base > 0) {
        double lnApprox = (base - 1.0) - (base - 1.0) * (base - 1.0) / 2.0; // ln(x) ≈ (x-1) - (x-1)²/2
        double fracResult = 1.0 + fracPart * lnApprox;
        result *= fracResult;
    }
    
    return result;
}

double mySqrt(double x) {
    if (x < 0) return 0.0;
    if (x == 0.0 || x == 1.0) return x;
    
    // Método de Newton-Raphson
    double guess = x > 1 ? x / 2.0 : x;
    double prev;
    
    for (int i = 0; i < 50; i++) {
        prev = guess;
        guess = (guess + x / guess) * 0.5;
        if (prev - guess < EPSILON && guess - prev < EPSILON) break;
    }
    
    return guess;
}

double myLog10(double x) {
    if (x <= 0) return 0.0;
    if (x == 1.0) return 0.0;
    if (x == 10.0) return 1.0;
    
    // Casos especiais para maior precisão
    if (x == 2.0) return 0.30102999566;
    if (x == 3.0) return 0.47712125472;
    if (x == 5.0) return 0.69897000433;
    
    // Normalizar para o intervalo [1, 10)
    int exponent = 0;
    while (x >= 10.0) {
        x /= 10.0;
        exponent++;
    }
    while (x < 1.0) {
        x *= 10.0;
        exponent--;
    }
    
    // Aproximação de ln(x) usando série de Taylor para ln(1+u) onde x = 1+u
    double u = x - 1.0;
    double ln_x = 0.0;
    double term = u;
    
    // ln(1+u) = u - u²/2 + u³/3 - u⁴/4 + ...
    for (int n = 1; n <= 20; n++) {
        ln_x += (n % 2 == 1 ? 1.0 : -1.0) * term / n;
        term *= u;
        if (term < EPSILON && term > -EPSILON) break;
    }
    
    // Converter ln para log10: log10(x) = ln(x) / ln(10)
    double log10_x = ln_x / 2.302585092994046; // ln(10)
    
    return log10_x + exponent;
}

double degToRad(double degrees) {
    return degrees * PI / 180.0;
}

double mySin(double degrees) {
    double x = degToRad(degrees);
    
    // Normalizar para [-2π, 2π]
    while (x > 2 * PI) x -= 2 * PI;
    while (x < -2 * PI) x += 2 * PI;
    
    // Série de Taylor: sin(x) = x - x³/3! + x⁵/5! - x⁷/7! + ...
    double result = 0.0;
    double term = x;
    
    for (int n = 1; n <= 20; n += 2) {
        result += term;
        term *= -x * x / ((n + 1) * (n + 2));
    }
    
    return result;
}

double myCos(double degrees) {
    double x = degToRad(degrees);
    
    // Normalizar para [-2π, 2π]
    while (x > 2 * PI) x -= 2 * PI;
    while (x < -2 * PI) x += 2 * PI;
    
    // Série de Taylor: cos(x) = 1 - x²/2! + x⁴/4! - x⁶/6! + ...
    double result = 1.0;
    double term = 1.0;
    
    for (int n = 2; n <= 22; n += 2) {
        term *= -x * x / ((n - 1) * n);
        result += term;
    }
    
    return result;
}

double myTan(double degrees) {
    double cosVal = myCos(degrees);
    if (cosVal == 0.0) return 0.0;
    return mySin(degrees) / cosVal;
}

double myMod(double x, double y) {
    if (y == 0.0) return 0.0;
    return x - ((int)(x / y)) * y;
}

// ====================== FUNÇÕES AUXILIARES ======================
int isNumber(const char *token) {
    if (!token || strlen(token) == 0) return 0;
    
    int i = 0;
    if (token[0] == '-' || token[0] == '+') i = 1;
    if (i == strlen(token)) return 0;
    
    int hasDecimal = 0;
    for (; i < strlen(token); i++) {
        if (isdigit(token[i])) continue;
        if ((token[i] == '.' || token[i] == ',') && !hasDecimal) {
            hasDecimal = 1;
            continue;
        }
        return 0;
    }
    
    return 1;
}

int isOperator(const char *token) {
    return token && (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
                     strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
                     strcmp(token, "%") == 0 || strcmp(token, "^") == 0);
}

int isFunction(const char *token) {
    return token && (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
                     strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
                     strcmp(token, "log") == 0);
}

int getPrecedence(const char *op) {
    if (!op) return 0;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    if (isFunction(op)) return 4;
    return 0;
}

int isLeftAssociative(const char *op) {
    return strcmp(op, "^") != 0;
}

// ====================== TOKENIZAÇÃO ======================
void tokenize(char *str, char tokens[][50], int *tokenCount) {
    *tokenCount = 0;
    if (!str) return;
    
    int i = 0;
    int len = strlen(str);
    
    while (i < len && *tokenCount < MAX_STACK) {
        while (i < len && isspace(str[i])) i++;
        if (i >= len) break;
        
        char token[50] = "";
        int tokenPos = 0;
        
        // Verificar funções
        if (i + 4 <= len && strncmp(&str[i], "raiz", 4) == 0 && 
            (i + 4 == len || !isalnum(str[i+4]))) {
            strcpy(tokens[*tokenCount], "raiz");
            (*tokenCount)++;
            i += 4;
        }
        else if (i + 3 <= len && strncmp(&str[i], "sen", 3) == 0 && 
                (i + 3 == len || !isalnum(str[i+3]))) {
            strcpy(tokens[*tokenCount], "sen");
            (*tokenCount)++;
            i += 3;
        }
        else if (i + 3 <= len && strncmp(&str[i], "cos", 3) == 0 && 
                (i + 3 == len || !isalnum(str[i+3]))) {
            strcpy(tokens[*tokenCount], "cos");
            (*tokenCount)++;
            i += 3;
        }
        else if (i + 3 <= len && strncmp(&str[i], "log", 3) == 0 && 
                (i + 3 == len || !isalnum(str[i+3]))) {
            strcpy(tokens[*tokenCount], "log");
            (*tokenCount)++;
            i += 3;
        }
        else if (i + 2 <= len && strncmp(&str[i], "tg", 2) == 0 && 
                (i + 2 == len || !isalnum(str[i+2]))) {
            strcpy(tokens[*tokenCount], "tg");
            (*tokenCount)++;
            i += 2;
        }
        else if (strchr("+-*/%^()", str[i])) {
            token[0] = str[i];
            token[1] = '\0';
            strcpy(tokens[*tokenCount], token);
            (*tokenCount)++;
            i++;
        }
        else if (isdigit(str[i]) || str[i] == '.') {
            while (i < len && (isdigit(str[i]) || str[i] == '.' || str[i] == ',')) {
                if (str[i] == ',') {
                    token[tokenPos++] = '.';
                } else {
                    token[tokenPos++] = str[i];
                }
                i++;
            }
            token[tokenPos] = '\0';
            strcpy(tokens[*tokenCount], token);
            (*tokenCount)++;
        }
        else {
            i++;
        }
    }
}

// ====================== OPERAÇÕES ======================
float applyBinaryOperation(const char *op, float left, float right) {
    if (strcmp(op, "+") == 0) return left + right;
    if (strcmp(op, "-") == 0) return left - right;
    if (strcmp(op, "*") == 0) return left * right;
    if (strcmp(op, "/") == 0) return (right != 0) ? left / right : 0;
    if (strcmp(op, "%") == 0) return (right != 0) ? myMod(left, right) : 0;
    if (strcmp(op, "^") == 0) return myPow(left, right);
    return 0;
}

float applyUnaryFunction(const char *func, float operand) {
    if (strcmp(func, "raiz") == 0) return mySqrt(operand);
    if (strcmp(func, "sen") == 0) return mySin(operand);
    if (strcmp(func, "cos") == 0) return myCos(operand);
    if (strcmp(func, "tg") == 0) return myTan(operand);
    if (strcmp(func, "log") == 0) return myLog10(operand);
    return 0;
}

// ====================== VALIDAÇÃO ======================
int validateTokens(char tokens[][50], int tokenCount) {
    if (tokenCount == 0) return 0;
    
    int operandCount = 0;
    int operatorCount = 0;
    
    for (int i = 0; i < tokenCount; i++) {
        if (isNumber(tokens[i])) {
            operandCount++;
        } else if (isOperator(tokens[i])) {
            operatorCount++;
        }
    }
    
    return (operandCount == operatorCount + 1);
}

// ====================== CONVERSÕES ======================
char *getFormaPosFixa(char *Str) {
    if (!Str) return NULL;
    
    char *result = malloc(512);
    if (!result) return NULL;
    result[0] = '\0';
    
    char temp[512];
    strncpy(temp, Str, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    if (tokenCount == 0) {
        free(result);
        return NULL;
    }
    
    CharStack stack;
    initCharStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            if (strlen(result) > 0) strcat(result, " ");
            strcat(result, token);
        }
        else if (isFunction(token)) {
            pushChar(&stack, token);
        }
        else if (strcmp(token, "(") == 0) {
            pushChar(&stack, token);
        }
        else if (strcmp(token, ")") == 0) {
            while (!isCharStackEmpty(&stack) && strcmp(topChar(&stack), "(") != 0) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
            if (!isCharStackEmpty(&stack)) {
                popChar(&stack);
            }
            if (!isCharStackEmpty(&stack) && isFunction(topChar(&stack))) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
        }
        else if (isOperator(token)) {
            while (!isCharStackEmpty(&stack) && 
                   strcmp(topChar(&stack), "(") != 0 &&
                   (getPrecedence(topChar(&stack)) > getPrecedence(token) ||
                    (getPrecedence(topChar(&stack)) == getPrecedence(token) && 
                     isLeftAssociative(token)))) {
                if (strlen(result) > 0) strcat(result, " ");
                strcat(result, popChar(&stack));
            }
            pushChar(&stack, token);
        }
    }
    
    while (!isCharStackEmpty(&stack)) {
        if (strlen(result) > 0) strcat(result, " ");
        strcat(result, popChar(&stack));
    }
    
    return result;
}

int needsParentheses(const char *expr, const char *operator, int isLeftOperand) {
    if (!expr || !operator) return 0;
    
    // Se é número simples, não precisa parênteses
    if (isNumber(expr)) return 0;
    
    // Se já tem parênteses externos, não precisa mais
    if (expr[0] == '(' && expr[strlen(expr)-1] == ')') {
        // Verificar se são parênteses balanceados externos
        int count = 0;
        for (int i = 0; i < strlen(expr); i++) {
            if (expr[i] == '(') count++;
            if (expr[i] == ')') count--;
            if (count == 0 && i < strlen(expr)-1) {
                break; // Parênteses não são externos
            }
        }
        if (count == 0) return 0;
    }
    
    // Se é função simples, não precisa parênteses
    if ((strncmp(expr, "sen(", 4) == 0 || strncmp(expr, "cos(", 4) == 0 ||
         strncmp(expr, "tg(", 3) == 0 || strncmp(expr, "log(", 4) == 0 ||
         strncmp(expr, "raiz(", 5) == 0) && expr[strlen(expr)-1] == ')') {
        return 0;
    }
    
    int currentPrec = getPrecedence(operator);
    
    // Verificar se contém operadores de menor precedência
    if (currentPrec >= 2 && (strstr(expr, " + ") || strstr(expr, " - "))) return 1;
    if (currentPrec >= 3 && (strstr(expr, " * ") || strstr(expr, " / ") || strstr(expr, " % "))) return 1;
    
    return 0;
}

char *getFormaInFixa(char *Str) {
    if (!Str) return NULL;
    
    char *result = malloc(512);
    if (!result) return NULL;
    result[0] = '\0';
    
    char temp[512];
    strncpy(temp, Str, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    if (!validateTokens(tokens, tokenCount)) {
        free(result);
        return NULL;
    }
    
    CharStack stack;
    initCharStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            pushChar(&stack, token);
        }
        else if (isOperator(token)) {
            if (stack.top >= 1) {
                char *right = popChar(&stack);
                char *left = popChar(&stack);
                
                char expression[512];  // Aumentado para evitar truncamento
                char leftPart[200], rightPart[200];  // Aumentado
                
                // Aplicar regras de parênteses
                if (needsParentheses(left, token, 1)) {
                    snprintf(leftPart, sizeof(leftPart), "(%s)", left);
                } else {
                    strncpy(leftPart, left, sizeof(leftPart) - 1);
                    leftPart[sizeof(leftPart) - 1] = '\0';
                }
                
                if (needsParentheses(right, token, 0)) {
                    snprintf(rightPart, sizeof(rightPart), "(%s)", right);
                } else {
                    strncpy(rightPart, right, sizeof(rightPart) - 1);
                    rightPart[sizeof(rightPart) - 1] = '\0';
                }
                
                if (strcmp(token, "^") == 0) {
                    snprintf(expression, sizeof(expression), "%s^%s", leftPart, rightPart);
                } else {
                    snprintf(expression, sizeof(expression), "%s %s %s", leftPart, token, rightPart);
                }
                
                pushChar(&stack, expression);
            }
        }
        else if (isFunction(token)) {
            if (stack.top >= 0) {
                char *operand = popChar(&stack);
                char expression[256];  // Aumentado para segurança
                
                snprintf(expression, sizeof(expression), "%s(%s)", token, operand);
                pushChar(&stack, expression);
            }
        }
    }
    
    if (!isCharStackEmpty(&stack)) {
        strncpy(result, topChar(&stack), 511);
        result[511] = '\0';
    } else {
        strcpy(result, "");
    }
    
    return result;
}

// ====================== AVALIAÇÃO ======================
float getValorPosFixa(char *StrPosFixa) {
    if (!StrPosFixa) return 0.0f;
    
    char temp[512];
    strncpy(temp, StrPosFixa, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char tokens[MAX_STACK][50];
    int tokenCount;
    tokenize(temp, tokens, &tokenCount);
    
    if (!validateTokens(tokens, tokenCount)) return 0.0f;
    
    FloatStack stack;
    initFloatStack(&stack);
    
    for (int i = 0; i < tokenCount; i++) {
        char *token = tokens[i];
        
        if (isNumber(token)) {
            char cleanToken[50];
            strncpy(cleanToken, token, sizeof(cleanToken) - 1);
            cleanToken[sizeof(cleanToken) - 1] = '\0';
            
            for (int j = 0; j < strlen(cleanToken); j++) {
                if (cleanToken[j] == ',') cleanToken[j] = '.';
            }
            
            float num = atof(cleanToken);
            pushFloat(&stack, num);
        }
        else if (isOperator(token)) {
            if (stack.top >= 1) {
                float right = popFloat(&stack);
                float left = popFloat(&stack);
                float result = applyBinaryOperation(token, left, right);
                pushFloat(&stack, result);
            }
        }
        else if (isFunction(token)) {
            if (stack.top >= 0) {
                float operand = popFloat(&stack);
                float result = applyUnaryFunction(token, operand);
                pushFloat(&stack, result);
            }
        }
    }
    
    return isFloatStackEmpty(&stack) ? 0.0f : popFloat(&stack);
}

float getValorInFixa(char *StrInFixa) {
    if (!StrInFixa) return 0.0f;
    
    char *posFixa = getFormaPosFixa(StrInFixa);
    if (!posFixa) return 0.0f;
    
    float result = getValorPosFixa(posFixa);
    free(posFixa);
    
    return result;
}