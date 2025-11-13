#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da sala
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * Função: criarSala
 * Propósito: Cria uma nova sala dinamicamente com o nome especificado
 * Parâmetros: nome - string com o nome da sala
 * Retorno: Ponteiro para a sala criada
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: não foi possível alocar memória para a sala.\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Função: explorarSalas
 * Propósito: Permite a navegação do jogador pela árvore de salas
 * Parâmetros: salaAtual - ponteiro para a sala atual na exploração
 */
void explorarSalas(Sala* salaAtual) {
    char opcao;
    
    printf("\n=== DETECTIVE QUEST ===\n");
    printf("Você está na: %s\n", salaAtual->nome);
    
    // Verifica se é uma sala final (sem saídas)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("🚨 Esta sala não tem mais saídas! Exploração concluída.\n");
        return;
    }
    
    // Mostra opções disponíveis
    printf("\nOpções de exploração:\n");
    if (salaAtual->esquerda != NULL) {
        printf("[e] - Ir para ESQUERDA (%s)\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("[d] - Ir para DIREITA (%s)\n", salaAtual->direita->nome);
    }
    printf("[s] - Sair do jogo\n");
    
    // Loop para obter uma opção válida
    while (1) {
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    printf("\n➡️  Indo para a esquerda...\n");
                    explorarSalas(salaAtual->esquerda);
                    return;
                } else {
                    printf("❌ Não há caminho à esquerda! Escolha outra opção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    printf("\n➡️  Indo para a direita...\n");
                    explorarSalas(salaAtual->direita);
                    return;
                } else {
                    printf("❌ Não há caminho à direita! Escolha outra opção.\n");
                }
                break;
                
            case 's':
            case 'S':
                printf("\n👋 Saindo do jogo... Até a próxima!\n");
                return;
                
            default:
                printf("❌ Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para sair.\n");
                break;
        }
    }
}

/**
 * Função: liberarArvore
 * Propósito: Libera toda a memória alocada para a árvore (pós-ordem)
 * Parâmetros: raiz - ponteiro para a raiz da árvore
 */
void liberarArvore(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

/**
 * Função: main
 * Propósito: Monta o mapa da mansão e inicia a exploração
 */
int main() {
    printf("🕵️  Bem-vindo ao Detective Quest! 🕵️\n");
    printf("Explore a mansão para encontrar pistas e desvendar o mistério!\n");
    
    // Montagem do mapa da mansão (árvore binária)
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cofre Secreto");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim de Inverno");
    Sala* escritorio = criarSala("Escritório do Detetive");
    Sala* quarto = criarSala("Quarto Principal");
    
    // Construção da estrutura da árvore
    // Hall de Entrada
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = biblioteca;
    
    // Sala de Estar
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    
    // Biblioteca
    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;
    
    // As outras salas são folhas (sem filhos)
    
    // Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hallEntrada);
    
    // Libera a memória alocada para a árvore
    liberarArvore(hallEntrada);
    
    printf("\n🎮 Jogo finalizado. Obrigado por jogar Detective Quest! 🎮\n");
    
    return 0;
}