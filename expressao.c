#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "expressao.h"

#define MAX_SIZE 512
#define PI 3.14159265359

// Estrutura para pilha de caracteres
typedef struct {
    char data[MAX_SIZE];
    int top;
} CharStack;

// Estrutura para pilha de floats
typedef struct {
    float data[MAX_SIZE];
    int top;
} FloatStack;

// Estrutura para pilha de strings (para conversão pós-fixa para infixa)
typedef struct {
    char data[MAX_SIZE][MAX_SIZE];
    int top;
} StringStack;

// Funções auxiliares para pilha de caracteres
void charStackInit(CharStack *s) {
    s->top = -1;
}

int charStackIsEmpty(CharStack *s) {
    return s->top == -1;
}

int charStackIsFull(CharStack *s) {
    return s->top == MAX_SIZE - 1;
}

void charStackPush(CharStack *s, char c) {
    if (!charStackIsFull(s)) {
        s->data[++s->top] = c;
    }
}

char charStackPop(CharStack *s) {
    if (!charStackIsEmpty(s)) {
        return s->data[s->top--];
    }
    return '\0';
}

char charStackPeek(CharStack *s) {
    if (!charStackIsEmpty(s)) {
        return s->data[s->top];
    }
    return '\0';
}

// Funções auxiliares para pilha de floats
void floatStackInit(FloatStack *s) {
    s->top = -1;
}

int floatStackIsEmpty(FloatStack *s) {
    return s->top == -1;
}

void floatStackPush(FloatStack *s, float f) {
    if (s->top < MAX_SIZE - 1) {
        s->data[++s->top] = f;
    }
}

float floatStackPop(FloatStack *s) {
    if (!floatStackIsEmpty(s)) {
        return s->data[s->top--];
    }
    return 0.0;
}

// Funções auxiliares para pilha de strings
void stringStackInit(StringStack *s) {
    s->top = -1;
}

int stringStackIsEmpty(StringStack *s) {
    return s->top == -1;
}

void stringStackPush(StringStack *s, char *str) {
    if (s->top < MAX_SIZE - 1) {
        strcpy(s->data[++s->top], str);
    }
}

void stringStackPop(StringStack *s, char *result) {
    if (!stringStackIsEmpty(s)) {
        strcpy(result, s->data[s->top--]);
    }
}

// Função para verificar se é operador
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

// Função para verificar se é função matemática
int isFunction(char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || 
            strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || 
            strcmp(token, "raiz") == 0);
}

// Função para obter precedência do operador
int getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Função para verificar se operador é associativo à direita
int isRightAssociative(char op) {
    return (op == '^');
}

// Converter graus para radianos
double degreesToRadians(double degrees) {
    return degrees * PI / 180.0;
}

// Aplicar função matemática
float applyFunction(char *func, float operand) {
    if (strcmp(func, "sen") == 0) {
        return sin(degreesToRadians(operand));
    } else if (strcmp(func, "cos") == 0) {
        return cos(degreesToRadians(operand));
    } else if (strcmp(func, "tg") == 0) {
        return tan(degreesToRadians(operand));
    } else if (strcmp(func, "log") == 0) {
        return log10(operand);
    } else if (strcmp(func, "raiz") == 0) {
        return sqrt(operand);
    }
    return 0.0;
}

// Aplicar operação binária
float applyOperation(char op, float b, float a) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
        case '%': return fmod(a, b);
        case '^': return pow(a, b);
        default: return 0;
    }
}

// Converter infixa para pós-fixa
char *getFormaPosFixa(char *Str) {
    static char result[MAX_SIZE];
    CharStack stack;
    charStackInit(&stack);
    
    int resultIndex = 0;
    int i = 0;
    int len = strlen(Str);
    
    memset(result, 0, MAX_SIZE);
    
    while (i < len) {
        // Pular espaços
        while (i < len && isspace(Str[i])) i++;
        if (i >= len) break;
        
        // Se é número
        if (isdigit(Str[i]) || Str[i] == '.') {
            while (i < len && (isdigit(Str[i]) || Str[i] == '.')) {
                result[resultIndex++] = Str[i++];
            }
            result[resultIndex++] = ' ';
        }
        // Se é parêntese de abertura
        else if (Str[i] == '(') {
            charStackPush(&stack, Str[i]);
            i++;
        }
        // Se é parêntese de fechamento
        else if (Str[i] == ')') {
            while (!charStackIsEmpty(&stack) && charStackPeek(&stack) != '(') {
                result[resultIndex++] = charStackPop(&stack);
                result[resultIndex++] = ' ';
            }
            charStackPop(&stack); // Remove '('
            i++;
        }
        // Se é operador
        else if (isOperator(Str[i])) {
            while (!charStackIsEmpty(&stack) && charStackPeek(&stack) != '(' &&
                   ((getPrecedence(charStackPeek(&stack)) > getPrecedence(Str[i])) ||
                    (getPrecedence(charStackPeek(&stack)) == getPrecedence(Str[i]) && 
                     !isRightAssociative(Str[i])))) {
                result[resultIndex++] = charStackPop(&stack);
                result[resultIndex++] = ' ';
            }
            charStackPush(&stack, Str[i]);
            i++;
        }
        // Se é função
        else if (isalpha(Str[i])) {
            char func[10];
            int j = 0;
            while (i < len && isalpha(Str[i])) {
                func[j++] = Str[i++];
            }
            func[j] = '\0';
            
            if (isFunction(func)) {
                // Adiciona função ao resultado
                strcat(result + resultIndex, func);
                resultIndex += strlen(func);
                result[resultIndex++] = ' ';
            }
        }
        else {
            i++;
        }
    }
    
    // Desempilha operadores restantes
    while (!charStackIsEmpty(&stack)) {
        result[resultIndex++] = charStackPop(&stack);
        result[resultIndex++] = ' ';
    }
    
    // Remove espaço final
    if (resultIndex > 0 && result[resultIndex-1] == ' ') {
        result[resultIndex-1] = '\0';
    } else {
        result[resultIndex] = '\0';
    }
    
    return result;
}

// Converter pós-fixa para infixa
char *getFormaInFixa(char *Str) {
    static char result[MAX_SIZE];
    StringStack stack;
    stringStackInit(&stack);
    
    char *token = strtok(strdup(Str), " ");
    
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
            // É número
            stringStackPush(&stack, token);
        }
        else if (isOperator(token[0]) && strlen(token) == 1) {
            // É operador binário
            char operand2[MAX_SIZE], operand1[MAX_SIZE];
            char expression[MAX_SIZE];
            
            stringStackPop(&stack, operand2);
            stringStackPop(&stack, operand1);
            
            sprintf(expression, "(%s %c %s)", operand1, token[0], operand2);
            stringStackPush(&stack, expression);
        }
        else if (isFunction(token)) {
            // É função unária
            char operand[MAX_SIZE];
            char expression[MAX_SIZE];
            
            stringStackPop(&stack, operand);
            sprintf(expression, "%s(%s)", token, operand);
            stringStackPush(&stack, expression);
        }
        
        token = strtok(NULL, " ");
    }
    
    stringStackPop(&stack, result);
    return result;
}

// Avaliar expressão pós-fixa
float getValorPosFixa(char *StrPosFixa) {
    FloatStack stack;
    floatStackInit(&stack);
    
    char *token = strtok(strdup(StrPosFixa), " ");
    
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
            // É número
            floatStackPush(&stack, atof(token));
        }
        else if (isOperator(token[0]) && strlen(token) == 1) {
            // É operador binário
            float b = floatStackPop(&stack);
            float a = floatStackPop(&stack);
            float result = applyOperation(token[0], b, a);
            floatStackPush(&stack, result);
        }
        else if (isFunction(token)) {
            // É função unária
            float operand = floatStackPop(&stack);
            float result = applyFunction(token, operand);
            floatStackPush(&stack, result);
        }
        
        token = strtok(NULL, " ");
    }
    
    return floatStackPop(&stack);
}

// Avaliar expressão infixa
float getValorInFixa(char *StrInFixa) {
    char *posfixa = getFormaPosFixa(StrInFixa);
    return getValorPosFixa(posfixa);
}