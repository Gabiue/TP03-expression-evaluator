/*
 * Universidade Católica de Brasília - UCB
 * Estrutura de Dados - 1° semestre de 2025
 * Professor Marcelo Eustáquio
 * Avaliador de Expressões Numéricas
 * 
 * Baseado no Algoritmo Shunting Yard para conversão infixa -> pós-fixa
 * e algoritmo de pilha para avaliação de expressões pós-fixas.
 * 
 * Precedência de operadores:
 * - Funções (sen, cos, tg, log, raiz): precedência 4
 * - Potenciação (^): precedência 3 (associativo à direita)
 * - Multiplicação, divisão, módulo (*, /, %): precedência 2
 * - Adição e subtração (+, -): precedência 1
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
    printf("6. Demonstração de Conversão (Problema dos Parênteses)\n");
    printf("7. Sair\n");
    printf("Escolha uma opção: ");
}

void demonstrarConversao() {
    printf("\n=== DEMONSTRAÇÃO DE CONVERSÃO PÓS-FIXA → INFIXA ===\n");
    
    printf("Exemplo: 45 60 + 30 cos *\n");
    printf("Passo a passo:\n");
    printf("1. 45 → pilha: [45]\n");
    printf("2. 60 → pilha: [45, 60]\n");
    printf("3. + → desempilha 60 e 45, cria '45 + 60', empilha: [45 + 60]\n");
    printf("4. 30 → pilha: [45 + 60, 30]\n");
    printf("5. cos → desempilha 30, cria 'cos(30)', empilha: [45 + 60, cos(30)]\n");
    printf("6. * → desempilha cos(30) e '45 + 60', verifica parênteses\n");
    printf("   - '45 + 60' contém +, então precisa parênteses para *\n");
    printf("   - Resultado: '(45 + 60) * cos(30)'\n\n");
    
    printf("Resultado da conversão: %s\n", getFormaInFixa("45 60 + 30 cos *"));
    printf("Valor calculado: %.2f\n", getValorPosFixa("45 60 + 30 cos *"));
    printf("Verificação: (45+60) * cos(30°) = 105 * 0.866 ≈ 90.93\n\n");
}

void executarTestesAutomaticos() {
    printf("\n=== EXECUTANDO TESTES AUTOMÁTICOS ===\n");
    printf("Verificando conformidade com a tabela de testes do professor:\n\n");
    
    // Teste 1: 3 4 + 5 * = (3 + 4) * 5 = 35
    printf("Teste 1:\n");
    printf("Pós-fixa: 3 4 + 5 *\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("3 4 + 5 *"));
    printf("Infixa esperada: (3 + 4) * 5\n");
    float val1 = getValorPosFixa("3 4 + 5 *");
    printf("Valor: %.2f (esperado: 35)\n", val1);
    printf("Status: %s\n\n", (val1 == 35) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 2: 7 2 * 4 + = 7 * 2 + 4 = 18
    printf("Teste 2:\n");
    printf("Pós-fixa: 7 2 * 4 +\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("7 2 * 4 +"));
    printf("Infixa esperada: 7 * 2 + 4\n");
    float val2 = getValorPosFixa("7 2 * 4 +");
    printf("Valor: %.2f (esperado: 18)\n", val2);
    printf("Status: %s\n\n", (val2 == 18) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 3: 8 5 2 4 + * + = 8 + (5 * (2 + 4)) = 38
    printf("Teste 3:\n");
    printf("Pós-fixa: 8 5 2 4 + * +\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("8 5 2 4 + * +"));
    printf("Infixa esperada: 8 + (5 * (2 + 4))\n");
    float val3 = getValorPosFixa("8 5 2 4 + * +");
    printf("Valor: %.2f (esperado: 38)\n", val3);
    printf("Status: %s\n\n", (val3 == 38) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 4: 6 2 / 3 + 4 * = (6 / 2 + 3) * 4 = 24
    printf("Teste 4:\n");
    printf("Pós-fixa: 6 2 / 3 + 4 *\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("6 2 / 3 + 4 *"));
    printf("Infixa esperada: (6 / 2 + 3) * 4\n");
    float val4 = getValorPosFixa("6 2 / 3 + 4 *");
    printf("Valor: %.2f (esperado: 24)\n", val4);
    printf("Status: %s\n\n", (val4 == 24) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 5: 9 5 2 8 * 4 + * + = 9 + (5 * (2 + 8 * 4)) = 109
    printf("Teste 5:\n");
    printf("Pós-fixa: 9 5 2 8 * 4 + * +\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("9 5 2 8 * 4 + * +"));
    printf("Infixa esperada: 9 + (5 * (2 + 8 * 4))\n");
    float val5 = getValorPosFixa("9 5 2 8 * 4 + * +");
    printf("Valor: %.2f (esperado: 109)\n", val5);
    printf("Status: %s\n\n", (val5 == 109) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 6: 2 3 + log 5 / = log(2 + 3) / 5 ≈ 0.14
    printf("Teste 6:\n");
    printf("Pós-fixa: 2 3 + log 5 /\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("2 3 + log 5 /"));
    printf("Infixa esperada: log(2 + 3) / 5\n");
    float val6 = getValorPosFixa("2 3 + log 5 /");
    printf("Valor: %.4f (esperado: ~0.14)\n", val6);
    printf("Status: %s\n\n", (val6 > 0.13 && val6 < 0.15) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 7: 10 log 3 ^ 2 + = (log10)^3 + 2 = 3
    printf("Teste 7:\n");
    printf("Pós-fixa: 10 log 3 ^ 2 +\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("10 log 3 ^ 2 +"));
    printf("Infixa esperada: (log10)^3 + 2\n");
    float val7 = getValorPosFixa("10 log 3 ^ 2 +");
    printf("Valor: %.2f (esperado: 3)\n", val7);
    printf("Status: %s\n\n", (val7 == 3) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 8: 45 60 + 30 cos * = (45 + 60) * cos(30) ≈ 90.93
    printf("Teste 8:\n");
    printf("Pós-fixa: 45 60 + 30 cos *\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("45 60 + 30 cos *"));
    printf("Infixa esperada: (45 + 60) * cos(30)\n");
    float val8 = getValorPosFixa("45 60 + 30 cos *");
    printf("Valor: %.2f (esperado: ~90.93)\n", val8);
    printf("Status: %s\n\n", (val8 > 90 && val8 < 92) ? "✓ CORRETO" : "✗ ERRO");
    
    // Teste 9: 0.5 45 sen 2 ^ + = sen(45)^2 + 0.5 = 1
    printf("Teste 9:\n");
    printf("Pós-fixa: 0.5 45 sen 2 ^ +\n");
    printf("Infixa obtida: %s\n", getFormaInFixa("0.5 45 sen 2 ^ +"));
    printf("Infixa esperada: sen(45)^2 + 0.5\n");
    float val9 = getValorPosFixa("0.5 45 sen 2 ^ +");
    printf("Valor: %.2f (esperado: 1)\n", val9);
    printf("Status: %s\n\n", (val9 == 1) ? "✓ CORRETO" : "✗ ERRO");
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
                demonstrarConversao();
                break;
                
            case 7:
                printf("\nEncerrando o programa...\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
        
        if (opcao != 7) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 7);
    
    return 0;
}