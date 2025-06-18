#include <stdio.h>
#include <string.h>
#include "expressao.h"

void testarCasosEspecificos() {
    printf("=== TESTES DOS CASOS ESPECIFICADOS ===\n\n");
    
    // Teste 1: 3 4 + 5 * = (3 + 4) * 5 = 35
    printf("Teste 1:\n");
    printf("Pós-fixa: 3 4 + 5 *\n");
    printf("Valor: %.2f (esperado: 35)\n", getValorPosFixa("3 4 + 5 *"));
    printf("Infixa: %s\n\n", getFormaInFixa("3 4 + 5 *"));
    
    // Teste 2: 7 2 * 4 + = 7 * 2 + 4 = 18
    printf("Teste 2:\n");
    printf("Pós-fixa: 7 2 * 4 +\n");
    printf("Valor: %.2f (esperado: 18)\n", getValorPosFixa("7 2 * 4 +"));
    printf("Infixa: %s\n\n", getFormaInFixa("7 2 * 4 +"));
    
    // Teste 3: 8 5 2 4 + * + = 8 + (5 * (2 + 4)) = 38
    printf("Teste 3:\n");
    printf("Pós-fixa: 8 5 2 4 + * +\n");
    printf("Valor: %.2f (esperado: 38)\n", getValorPosFixa("8 5 2 4 + * +"));
    printf("Infixa: %s\n\n", getFormaInFixa("8 5 2 4 + * +"));
    
    // Teste 4: 6 2 / 3 + 4 * = (6 / 2 + 3) * 4 = 24
    printf("Teste 4:\n");
    printf("Pós-fixa: 6 2 / 3 + 4 *\n");
    printf("Valor: %.2f (esperado: 24)\n", getValorPosFixa("6 2 / 3 + 4 *"));
    printf("Infixa: %s\n\n", getFormaInFixa("6 2 / 3 + 4 *"));
    
    // Teste 5: 9 5 2 8 * 4 + * + = 9 + (5 * (2 + 8 * 4)) = 109
    printf("Teste 5:\n");
    printf("Pós-fixa: 9 5 2 8 * 4 + * +\n");
    printf("Valor: %.2f (esperado: 109)\n", getValorPosFixa("9 5 2 8 * 4 + * +"));
    printf("Infixa: %s\n\n", getFormaInFixa("9 5 2 8 * 4 + * +"));
    
    // Teste 6: 2 3 + log 5 / = log(2 + 3) / 5 ≈ 0.14
    printf("Teste 6:\n");
    printf("Pós-fixa: 2 3 + log 5 /\n");
    printf("Valor: %.4f (esperado: ~0.14)\n", getValorPosFixa("2 3 + log 5 /"));
    printf("Infixa: %s\n\n", getFormaInFixa("2 3 + log 5 /"));
    
    // Teste 7: 10 log 3 ^ 2 + = (log10)^3 + 2 = 3
    printf("Teste 7:\n");
    printf("Pós-fixa: 10 log 3 ^ 2 +\n");
    printf("Valor: %.2f (esperado: 3)\n", getValorPosFixa("10 log 3 ^ 2 +"));
    printf("Infixa: %s\n\n", getFormaInFixa("10 log 3 ^ 2 +"));
    
    // Teste 8: 45 60 + 30 cos * = (45 + 60) * cos(30) ≈ 90.93
    printf("Teste 8:\n");
    printf("Pós-fixa: 45 60 + 30 cos *\n");
    printf("Valor: %.2f (esperado: ~90.93)\n", getValorPosFixa("45 60 + 30 cos *"));
    printf("Infixa: %s\n\n", getFormaInFixa("45 60 + 30 cos *"));
    
    // Teste 9: 0.5 45 sen 2 ^ + = sen(45)^2 + 0.5 = 1
    printf("Teste 9:\n");
    printf("Pós-fixa: 0.5 45 sen 2 ^ +\n");
    printf("Valor: %.2f (esperado: 1)\n", getValorPosFixa("0.5 45 sen 2 ^ +"));
    printf("Infixa: %s\n\n", getFormaInFixa("0.5 45 sen 2 ^ +"));
}

void testarConversoes() {
    printf("=== TESTES DE CONVERSÃO ===\n\n");
    
    // Teste infixa -> pós-fixa
    printf("Conversão Infixa -> Pós-fixa:\n");
    printf("Infixa: (3 + 4) * 5\n");
    printf("Pós-fixa: %s\n", getFormaPosFixa("(3 + 4) * 5"));
    printf("Valor: %.2f\n\n", getValorInFixa("(3 + 4) * 5"));
    
    printf("Infixa: 7 * 2 + 4\n");
    printf("Pós-fixa: %s\n", getFormaPosFixa("7 * 2 + 4"));
    printf("Valor: %.2f\n\n", getValorInFixa("7 * 2 + 4"));
    
    printf("Infixa: log(2 + 3) / 5\n");
    printf("Pós-fixa: %s\n", getFormaPosFixa("log(2 + 3) / 5"));
    printf("Valor: %.4f\n\n", getValorInFixa("log(2 + 3) / 5"));
}

void menuInterativo() {
    int opcao;
    char expressao[512];
    
    do {
        printf("\n=== AVALIADOR DE EXPRESSÕES ===\n");
        printf("1. Converter Infixa -> Pós-fixa\n");
        printf("2. Converter Pós-fixa -> Infixa\n");
        printf("3. Avaliar expressão Infixa\n");
        printf("4. Avaliar expressão Pós-fixa\n");
        printf("5. Executar testes automáticos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        
        switch(opcao) {
            case 1:
                printf("Digite a expressão infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = 0; // Remove \n
                printf("Pós-fixa: %s\n", getFormaPosFixa(expressao));
                break;
                
            case 2:
                printf("Digite a expressão pós-fixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = 0; // Remove \n
                printf("Infixa: %s\n", getFormaInFixa(expressao));
                break;
                
            case 3:
                printf("Digite a expressão infixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = 0; // Remove \n
                printf("Valor: %.4f\n", getValorInFixa(expressao));
                break;
                
            case 4:
                printf("Digite a expressão pós-fixa: ");
                fgets(expressao, sizeof(expressao), stdin);
                expressao[strcspn(expressao, "\n")] = 0; // Remove \n
                printf("Valor: %.4f\n", getValorPosFixa(expressao));
                break;
                
            case 5:
                testarCasosEspecificos();
                testarConversoes();
                break;
                
            case 0:
                printf("Encerrando programa...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while(opcao != 0);
}

int main() {
    printf("AVALIADOR DE EXPRESSÕES NUMÉRICAS\n");
    printf("Universidade Católica de Brasília - UCB\n");
    printf("Estrutura de Dados - 1º semestre 2025\n\n");
    
    // Executar testes automáticos primeiro
    testarCasosEspecificos();
    testarConversoes();
    
    // Menu interativo
    menuInterativo();
    
    return 0;
}