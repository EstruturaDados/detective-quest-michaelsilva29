#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura da sala (árvore binária do mapa)
typedef struct Sala {
    char nome[50];
    char pista[100];  // Pista associada à sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Definição da estrutura para a árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/**
 * Função: criarSala
 * Propósito: Cria uma nova sala dinamicamente com nome e pista
 * Parâmetros: 
 *   - nome: string com o nome da sala
 *   - pista: string com a pista encontrada na sala (pode ser vazia)
 * Retorno: Ponteiro para a sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: não foi possível alocar memória para a sala.\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * Função: criarNoPista
 * Propósito: Cria um novo nó para a árvore BST de pistas
 * Parâmetros: pista - string com o conteúdo da pista
 * Retorno: Ponteiro para o nó de pista criado
 */
PistaNode* criarNoPista(const char* pista) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro: não foi possível alocar memória para a pista.\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    
    return novoNo;
}

/**
 * Função: inserirPista
 * Propósito: Insere uma nova pista na árvore BST mantendo ordem alfabética
 * Parâmetros: 
 *   - raiz: ponteiro para a raiz da árvore de pistas
 *   - pista: string com a pista a ser inserida
 * Retorno: Ponteiro para a raiz da árvore atualizada
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    // Se a árvore está vazia, cria um novo nó
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    // Compara as pistas para decidir se vai para esquerda ou direita
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        // Pista menor (alfabeticamente) - vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Pista maior (alfabeticamente) - vai para a direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se as pistas são iguais, não insere duplicatas
    
    return raiz;
}

/**
 * Função: exibirPistas
 * Propósito: Exibe todas as pistas coletadas em ordem alfabética (travessia in-order)
 * Parâmetros: raiz - ponteiro para a raiz da árvore de pistas
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        // Travessia in-order: esquerda -> raiz -> direita
        exibirPistas(raiz->esquerda);
        printf("🔍 %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * Função: coletarPista
 * Propósito: Coleta uma pista de uma sala e a adiciona à árvore BST
 * Parâmetros:
 *   - sala: ponteiro para a sala atual
 *   - raizPistas: ponteiro para a raiz da árvore de pistas
 * Retorno: Ponteiro para a raiz da árvore de pistas atualizada
 */
PistaNode* coletarPista(Sala* sala, PistaNode* raizPistas) {
    if (sala == NULL || strlen(sala->pista) == 0) {
        return raizPistas;  // Não há pista para coletar
    }
    
    printf("\n🎯 PISTA ENCONTRADA: \"%s\"\n", sala->pista);
    printf("Pista adicionada ao seu caderno de investigação!\n");
    
    // Insere a pista na árvore BST
    raizPistas = inserirPista(raizPistas, sala->pista);
    
    return raizPistas;
}

/**
 * Função: explorarSalasComPistas
 * Propósito: Controla a navegação entre salas e coleta de pistas
 * Parâmetros:
 *   - salaAtual: ponteiro para a sala atual na exploração
 *   - raizPistas: ponteiro para a raiz da árvore de pistas
 * Retorno: Ponteiro para a raiz da árvore de pistas atualizada
 */
PistaNode* explorarSalasComPistas(Sala* salaAtual, PistaNode* raizPistas) {
    char opcao;
    
    printf("\n=== DETECTIVE QUEST ===\n");
    printf("📍 Você está na: %s\n", salaAtual->nome);
    
    // Coleta pista da sala atual (se houver)
    raizPistas = coletarPista(salaAtual, raizPistas);
    
    // Mostra opções disponíveis
    printf("\nOpções de exploração:\n");
    if (salaAtual->esquerda != NULL) {
        printf("[e] - Ir para ESQUERDA (%s)\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("[d] - Ir para DIREITA (%s)\n", salaAtual->direita->nome);
    }
    printf("[v] - Visualizar pistas coletadas\n");
    printf("[s] - Sair da investigação\n");
    
    // Loop para obter uma opção válida
    while (1) {
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    printf("\n➡️  Indo para a esquerda...\n");
                    raizPistas = explorarSalasComPistas(salaAtual->esquerda, raizPistas);
                    return raizPistas;
                } else {
                    printf("❌ Não há caminho à esquerda! Escolha outra opção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    printf("\n➡️  Indo para a direita...\n");
                    raizPistas = explorarSalasComPistas(salaAtual->direita, raizPistas);
                    return raizPistas;
                } else {
                    printf("❌ Não há caminho à direita! Escolha outra opção.\n");
                }
                break;
                
            case 'v':
            case 'V':
                printf("\n📓 === CADERNO DE INVESTIGAÇÃO ===\n");
                if (raizPistas == NULL) {
                    printf("Nenhuma pista coletada ainda.\n");
                } else {
                    printf("Pistas coletadas (ordem alfabética):\n");
                    exibirPistas(raizPistas);
                }
                break;
                
            case 's':
            case 'S':
                printf("\n👋 Encerrando investigação...\n");
                return raizPistas;
                
            default:
                printf("❌ Opção inválida! Use 'e' (esquerda), 'd' (direita), 'v' (ver pistas) ou 's' (sair).\n");
                break;
        }
    }
}

/**
 * Função: liberarArvoreSalas
 * Propósito: Libera toda a memória alocada para a árvore de salas
 * Parâmetros: raiz - ponteiro para a raiz da árvore de salas
 */
void liberarArvoreSalas(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvoreSalas(raiz->esquerda);
        liberarArvoreSalas(raiz->direita);
        free(raiz);
    }
}

/**
 * Função: liberarArvorePistas
 * Propósito: Libera toda a memória alocada para a árvore BST de pistas
 * Parâmetros: raiz - ponteiro para a raiz da árvore de pistas
 */
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

/**
 * Função: main
 * Propósito: Monta o mapa da mansão com pistas e inicia a exploração
 */
int main() {
    printf("🕵️  Bem-vindo ao Detective Quest - Sistema de Coleta de Pistas! 🕵️\n");
    printf("Explore a mansão, colete pistas e desvende o mistério!\n");
    
    // Montagem do mapa da mansão (árvore binária) com pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho pela metade na mesa");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco de facas");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa");
    Sala* jardim = criarSala("Jardim", "Pegadas de barro levando à cerca");
    Sala* escritorio = criarSala("Escritório", "Documento importante sumiu da gaveta");
    Sala* quarto = criarSala("Quarto Principal", "Relógio de parede parado às 23:47");
    Sala* porao = criarSala("Porão", "Baú antigo com documentos secretos");
    
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
    
    // Cozinha
    cozinha->esquerda = porao;
    
    // Inicializa a árvore BST de pistas (vazia no início)
    PistaNode* raizPistas = NULL;
    
    // Inicia a exploração a partir do Hall de Entrada
    raizPistas = explorarSalasComPistas(hallEntrada, raizPistas);
    
    // Exibe relatório final de todas as pistas coletadas
    printf("\n📋 === RELATÓRIO FINAL DA INVESTIGAÇÃO ===\n");
    printf("Total de pistas coletadas: \n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        printf("Pistas organizadas alfabeticamente:\n");
        exibirPistas(raizPistas);
    }
    
    // Libera a memória alocada para as árvores
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
    
    printf("\n🎮 Investigação encerrada. Obrigado por jogar Detective Quest! 🎮\n");
    
    return 0;
}