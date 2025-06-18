/*
 * Universidade Católica de Brasília - UCB
 * Estrutura de Dados - 1° semestre de 2025
 * Professor Marcelo Eustáquio
 * Avaliador de Expressões Numéricas
 * 
 * Compilação: gcc expressao.c main.c -o expressao.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("\n=== AVALIADOR DE EXPRESSÕES NUMÉRICAS ===\n");
    printf("1. Converter Infixa para Pós-fixa\n");
    printf("2. Converter Pós-fixa para Infixa\n");
    printf("3. Avaliar Expressão Pós-fixa\n");
    printf("4. Avaliar Expressão Infixa\n");
    printf("5. Executar Testes Automáticos\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");
}

void executarTestesAutomaticos() {
    printf("\n=== EXECUTANDO TESTES AUTOMÁTICOS ===\n");
    
    // Teste 1: 3 4 + 5 * = (3 + 4) * 5 = 35
    printf("Teste 1:\n");
    printf("Pós-fixa: 3 4 + 5 *\n");
    printf("Infixa: %s\n", getFormaInFixa("3 4 + 5 *"));
    printf("Valor: %.2f (esperado: 35)\n\n", getValorPosFixa("3 4 + 5 *"));
    
    // Teste 2: 7 2 * 4 + = 7 * 2 + 4 = 18
    printf("Teste 2:\n");
    printf("Pós-fixa: 7 2 * 4 +\n");
    printf("Infixa: %s\n", getFormaInFixa("7 2 * 4 +"));
    printf("Valor: %.2f (esperado: 18)\n\n", getValorPosFixa("7 2 * 4 +"));
    
    // Teste 3: 8 5 2 4 + * + = 8 + (5 * (2 + 4)) = 38
    printf("Teste 3:\n");
    printf("Pós-fixa: 8 5 2 4 + * +\n");
    printf("Infixa: %s\n", getFormaInFixa("8 5 2 4 + * +"));
    printf("Valor: %.2f (esperado: 38)\n\n", getValorPosFixa("8 5 2 4 + * +"));
    
    // Teste 4: 6 2 / 3 + 4 * = (6 / 2 + 3) * 4 = 24
    printf("Teste 4:\n");
    printf("Pós-fixa: 6 2 / 3 + 4 *\n");
    printf("Infixa: %s\n", getFormaInFixa("6 2 / 3 + 4 *"));
    printf("Valor: %.2f (esperado: 24)\n\n", getValorPosFixa("6 2 / 3 + 4 *"));
    
    // Teste 5: 9 5 2 8 * 4 + * + = 9 + (5 * (2 + 8 * 4)) = 109
    printf("Teste 5:\n");
    printf("Pós-fixa: 9 5 2 8 * 4 + * +\n");
    printf("Infixa: %s\n", getFormaInFixa("9 5 2 8 * 4 + * +"));
    printf("Valor: %.2f (esperado: 109)\n\n", getValorPosFixa("9 5 2 8 * 4 + * +"));
    
    // Teste 6: 2 3 + log 5 / = log(2 + 3) / 5 ≈ 0.14
    printf("Teste 6:\n");
    printf("Pós-fixa: 2 3 + log 5 /\n");
    printf("Infixa: %s\n", getFormaInFixa("2 3 + log 5 /"));
    printf("Valor: %.4f (esperado: ~0.14)\n\n", getValorPosFixa("2 3 + log 5 /"));
    
    // Teste 7: 10 log 3 ^ 2 + = (log10)^3 + 2 = 3
    printf("Teste 7:\n");
    printf("Pós-fixa: 10 log 3 ^ 2 +\n");
    printf("Infixa: %s\n", getFormaInFixa("10 log 3 ^ 2 +"));
    printf("Valor: %.2f (esperado: 3)\n\n", getValorPosFixa("10 log 3 ^ 2 +"));
    
    // Teste 8: 45 60 + 30 cos * = (45 + 60) * cos(30) ≈ 90.93
    printf("Teste 8:\n");
    printf("Pós-fixa: 45 60 + 30 cos *\n");
    printf("Infixa: %s\n", getFormaInFixa("45 60 + 30 cos *"));
    printf("Valor: %.2f (esperado: ~90.93)\n\n", getValorPosFixa("45 60 + 30 cos *"));
    
    // Teste 9: 0.5 45 sen 2 ^ + = sen(45)^2 + 0.5 = 1
    printf("Teste 9:\n");
    printf("Pós-fixa: 0.5 45 sen 2 ^ +\n");
    printf("Infixa: %s\n", getFormaInFixa("0.5 45 sen 2 ^ +"));
    printf("Valor: %.2f (esperado: 1)\n\n", getValorPosFixa("0.5 45 sen 2 ^ +"));
}

int main() {
    int opcao;
    char expressao[512];
    char *resultado;
    float valor;
    
    printf("Universidade Católica de Brasília - UCB\n");
    printf("Estrutura de Dados - 1° semestre de 2025\n");
    printf("Professor Marcelo Eustáquio\n");
    printf("Avaliador de Expressões Numéricas\n");
    
    do {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Digite um número.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        switch (opcao) {
            case 1:
                printf("\nDigite a expressão infixa (ex: 3 + 4 * 5): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    // Remove quebra de linha
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    resultado = getFormaPosFixa(expressao);
                    printf("Expressão pós-fixa: %s\n", resultado);
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 2:
                printf("\nDigite a expressão pós-fixa (ex: 3 4 5 * +): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    resultado = getFormaInFixa(expressao);
                    printf("Expressão infixa: %s\n", resultado);
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 3:
                printf("\nDigite a expressão pós-fixa para avaliar (ex: 3 4 +): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    valor = getValorPosFixa(expressao);
                    printf("Valor da expressão: %.4f\n", valor);
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 4:
                printf("\nDigite a expressão infixa para avaliar (ex: 3 + 4): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    valor = getValorInFixa(expressao);
                    printf("Valor da expressão: %.4f\n", valor);
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 5:
                executarTestesAutomaticos();
                break;
                
            case 6:
                printf("\nEncerrando o programa...\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
        
        if (opcao != 6) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 6);
    
    return 0;
}