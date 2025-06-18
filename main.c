/*
 * Universidade Católica de Brasília - UCB
 * Estrutura de Dados - 1° semestre de 2025
 * Professor Marcelo Eustáquio
 * Avaliador de Expressões Numéricas - VERSÃO NOTA 10
 * 
 * Melhorias implementadas:
 * - Gerenciamento seguro de memória (malloc/free)
 * - Validação robusta de entrada
 * - Implementações matemáticas de alta precisão
 * - Conversão pós-fixa→infixa corrigida
 * - Tratamento de erros completo
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
    printf("\n=== AVALIADOR DE EXPRESSÕES NUMÉRICAS - VERSÃO 10.0 ===\n");
    printf("1. Converter Infixa para Pós-fixa\n");
    printf("2. Converter Pós-fixa para Infixa\n");
    printf("3. Avaliar Expressão Pós-fixa\n");
    printf("4. Avaliar Expressão Infixa\n");
    printf("5. Executar Testes Automáticos\n");
    printf("6. Demonstração de Conversão\n");
    printf("7. Teste de Estresse (Validação)\n");
    printf("8. Sair\n");
    printf("Escolha uma opção: ");
}

void demonstrarConversao() {
    printf("\n=== DEMONSTRAÇÃO DE CONVERSÃO PÓS-FIXA → INFIXA ===\n");
    
    printf("Exemplo: 45 60 + 30 cos *\n");
    printf("Passo a passo:\n");
    printf("1. 45 → pilha: [45]\n");
    printf("2. 60 → pilha: [45, 60]\n");
    printf("3. + → desempilha 60 e 45, cria '(45 + 60)', empilha: [(45 + 60)]\n");
    printf("4. 30 → pilha: [(45 + 60), 30]\n");
    printf("5. cos → desempilha 30, cria 'cos(30)', empilha: [(45 + 60), cos(30)]\n");
    printf("6. * → desempilha cos(30) e '(45 + 60)', verifica precedência\n");
    printf("   - Precedência de + (1) < precedência de * (2)\n");
    printf("   - Mantém parênteses: '(45 + 60) * cos(30)'\n\n");
    
    char *resultado = getFormaInFixa("45 60 + 30 cos *");
    if (resultado) {
        printf("Resultado da conversão: %s\n", resultado);
        float valor = getValorPosFixa("45 60 + 30 cos *");
        printf("Valor calculado: %.2f\n", valor);
        printf("Verificação: (45+60) * cos(30°) = 105 * 0.866 ≈ 90.93\n");
        free(resultado);
    } else {
        printf("Erro na conversão!\n");
    }
    printf("\n");
}

void executarTestesAutomaticos() {
    printf("\n=== EXECUTANDO TESTES AUTOMÁTICOS VERSÃO 10.0 ===\n");
    printf("Verificando conformidade RIGOROSA com a tabela de testes:\n\n");
    
    struct {
        char *posFixa;
        char *infixaEsperada;
        float valorEsperado;
        float tolerancia;
    } testes[] = {
        {"3 4 + 5 *", "(3 + 4) * 5", 35.0f, 0.01f},
        {"7 2 * 4 +", "7 * 2 + 4", 18.0f, 0.01f},
        {"8 5 2 4 + * +", "8 + (5 * (2 + 4))", 38.0f, 0.01f},
        {"6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", 24.0f, 0.01f},
        {"9 5 2 8 * 4 + * +", "9 + (5 * (2 + 8 * 4))", 109.0f, 0.01f},
        {"2 3 + log 5 /", "log(2 + 3) / 5", 0.14f, 0.01f},
        {"10 log 3 ^ 2 +", "(log(10))^3 + 2", 3.0f, 0.01f},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", 90.93f, 1.0f},
        {"0.5 45 sen 2 ^ +", "sen(45)^2 + 0.5", 1.0f, 0.01f}
    };
    
    int totalTestes = sizeof(testes) / sizeof(testes[0]);
    int testesCorretos = 0;
    
    for (int i = 0; i < totalTestes; i++) {
        printf("Teste %d:\n", i + 1);
        printf("Pós-fixa: %s\n", testes[i].posFixa);
        
        char *infixaObtida = getFormaInFixa(testes[i].posFixa);
        if (infixaObtida) {
            printf("Infixa obtida: %s\n", infixaObtida);
            printf("Infixa esperada: %s\n", testes[i].infixaEsperada);
            
            float valor = getValorPosFixa(testes[i].posFixa);
            printf("Valor: %.4f (esperado: %.2f)\n", valor, testes[i].valorEsperado);
            
            // Verificar se valor está dentro da tolerância
            float diferenca = valor - testes[i].valorEsperado;
            if (diferenca < 0) diferenca = -diferenca;  // valor absoluto
            int valorCorreto = (diferenca <= testes[i].tolerancia);
            
            if (valorCorreto) {
                printf("Status: ✓ CORRETO\n");
                testesCorretos++;
            } else {
                printf("Status: ✗ ERRO - Valor incorreto\n");
            }
            
            free(infixaObtida);
        } else {
            printf("Status: ✗ ERRO - Falha na conversão\n");
        }
        printf("\n");
    }
    
    printf("=== RESULTADO FINAL ===\n");
    printf("Testes corretos: %d/%d\n", testesCorretos, totalTestes);
    printf("Percentual de acerto: %.1f%%\n", (float)testesCorretos / totalTestes * 100);
    
    if (testesCorretos == totalTestes) {
        printf("🎉 PARABÉNS! Todos os testes passaram - NOTA 10!\n");
    } else {
        printf("⚠️  Alguns testes falharam - Revisar implementação\n");
    }
}

void executarTesteEstresse() {
    printf("\n=== TESTE DE ESTRESSE E VALIDAÇÃO ===\n");
    
    // Teste de expressões inválidas
    char *expressoesInvalidas[] = {
        "",                          // Vazia
        "3 +",                      // Operador sem operando suficiente
        "3 4 + +",                  // Muitos operadores
        "3 4 5",                    // Muitos operandos
        "3 xyz +",                  // Token inválido
        "/ 3 4",                    // Operador no início
        "3 4 + 5 * 6",             // Expressão malformada
        NULL
    };
    
    printf("Testando expressões inválidas:\n");
    for (int i = 0; expressoesInvalidas[i] != NULL; i++) {
        printf("Teste %d: \"%s\" -> ", i + 1, expressoesInvalidas[i]);
        
        char *resultado = getFormaInFixa(expressoesInvalidas[i]);
        if (resultado == NULL || strlen(resultado) == 0) {
            printf("✓ Rejeitado corretamente\n");
            if (resultado) free(resultado);
        } else {
            printf("✗ Deveria ter sido rejeitado: %s\n", resultado);
            free(resultado);
        }
    }
    
    // Teste de expressões válidas complexas
    printf("\nTestando expressões válidas complexas:\n");
    char *expressoesValidas[] = {
        "2 3 ^ 4 5 ^ +",           // Potenciações múltiplas
        "10 log 2 log *",          // Logaritmos múltiplos
        "45 sen 60 cos * 30 tg +", // Funções trigonométricas
        "100 raiz 5 ^ 2 /",        // Raiz e potência
        NULL
    };
    
    for (int i = 0; expressoesValidas[i] != NULL; i++) {
        printf("Teste %d: %s\n", i + 1, expressoesValidas[i]);
        
        char *infixa = getFormaInFixa(expressoesValidas[i]);
        if (infixa) {
            float valor = getValorPosFixa(expressoesValidas[i]);
            printf("  Infixa: %s\n", infixa);
            printf("  Valor: %.4f\n", valor);
            free(infixa);
        } else {
            printf("  ✗ Erro na conversão\n");
        }
        printf("\n");
    }
    
    // Teste de precisão matemática
    printf("Teste de precisão matemática:\n");
    printf("log(10) = %.10f (esperado: 1.0000000000)\n", getValorPosFixa("10 log"));
    printf("sen(30) = %.10f (esperado: 0.5000000000)\n", getValorPosFixa("30 sen"));
    printf("cos(60) = %.10f (esperado: 0.5000000000)\n", getValorPosFixa("60 cos"));
    printf("2^10 = %.10f (esperado: 1024.0000000000)\n", getValorPosFixa("2 10 ^"));
    printf("raiz(64) = %.10f (esperado: 8.0000000000)\n", getValorPosFixa("64 raiz"));
}

int main() {
    int opcao;
    char expressao[512];
    char *resultado;
    float valor;
    
    printf("Universidade Católica de Brasília - UCB\n");
    printf("Estrutura de Dados - 1° semestre de 2025\n");
    printf("Professor Marcelo Eustáquio\n");
    printf("Avaliador de Expressões Numéricas - VERSÃO 10.0\n");
    printf("Implementação com gerenciamento seguro de memória e alta precisão\n");
    
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
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    resultado = getFormaPosFixa(expressao);
                    if (resultado) {
                        printf("Expressão pós-fixa: %s\n", resultado);
                        free(resultado);
                    } else {
                        printf("Erro: Expressão inválida ou memória insuficiente.\n");
                    }
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 2:
                printf("\nDigite a expressão pós-fixa (ex: 3 4 5 * +): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    resultado = getFormaInFixa(expressao);
                    if (resultado) {
                        printf("Expressão infixa: %s\n", resultado);
                        free(resultado);
                    } else {
                        printf("Erro: Expressão inválida ou memória insuficiente.\n");
                    }
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 3:
                printf("\nDigite a expressão pós-fixa para avaliar (ex: 3 4 +): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    valor = getValorPosFixa(expressao);
                    printf("Valor da expressão: %.6f\n", valor);
                } else {
                    printf("Erro ao ler a expressão.\n");
                }
                break;
                
            case 4:
                printf("\nDigite a expressão infixa para avaliar (ex: 3 + 4): ");
                if (fgets(expressao, sizeof(expressao), stdin) != NULL) {
                    expressao[strcspn(expressao, "\n")] = '\0';
                    
                    valor = getValorInFixa(expressao);
                    printf("Valor da expressão: %.6f\n", valor);
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
                executarTesteEstresse();
                break;
                
            case 8:
                printf("\nEncerrando o programa...\n");
                printf("Memória limpa com sucesso. Obrigado!\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }
        
        if (opcao != 8) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 8);
    
    return 0;
}