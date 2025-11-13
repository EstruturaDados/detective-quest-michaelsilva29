#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_HASH 10
#define MAX_SUSPEITOS 5
#define MIN_PISTAS_CULPADO 2

// Definição da estrutura da sala (árvore binária do mapa)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Definição da estrutura para a árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Definição da estrutura para a tabela hash
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// Definição da estrutura para contagem de suspeitos
typedef struct {
    char nome[50];
    int contagem;
} SuspeitoContagem;

/**
 * Função: criarSala
 * Propósito: Cria uma nova sala dinamicamente com nome e pista
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
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

/**
 * Função: funcaoHash
 * Propósito: Calcula o índice hash para uma pista
 */
int funcaoHash(const char* pista) {
    int hash = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        hash += tolower(pista[i]);
    }
    return hash % TAMANHO_HASH;
}

/**
 * Função: criarNoHash
 * Propósito: Cria um novo nó para a tabela hash
 */
HashNode* criarNoHash(const char* pista, const char* suspeito) {
    HashNode* novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro: não foi possível alocar memória para o nó hash.\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;
    
    return novoNo;
}

/**
 * Função: inserirNaHash
 * Propósito: Insere associação pista/suspeito na tabela hash
 */
void inserirNaHash(HashNode** tabelaHash, const char* pista, const char* suspeito) {
    int indice = funcaoHash(pista);
    HashNode* novoNo = criarNoHash(pista, suspeito);
    
    // Inserção no início da lista encadeada
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}

/**
 * Função: encontrarSuspeito
 * Propósito: Consulta o suspeito correspondente a uma pista na tabela hash
 */
char* encontrarSuspeito(HashNode** tabelaHash, const char* pista) {
    int indice = funcaoHash(pista);
    HashNode* atual = tabelaHash[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido";
}

/**
 * Função: exibirPistas
 * Propósito: Exibe todas as pistas coletadas em ordem alfabética
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("🔍 %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/**
 * Função: coletarPista
 * Propósito: Coleta uma pista de uma sala e a adiciona à árvore BST
 */
PistaNode* coletarPista(Sala* sala, PistaNode* raizPistas, HashNode** tabelaHash) {
    if (sala == NULL || strlen(sala->pista) == 0) {
        return raizPistas;
    }
    
    printf("\n🎯 PISTA ENCONTRADA: \"%s\"\n", sala->pista);
    
    // Verifica se a pista já foi coletada
    char* suspeito = encontrarSuspeito(tabelaHash, sala->pista);
    if (strcmp(suspeito, "Desconhecido") != 0) {
        printf("📋 Esta pista aponta para: %s\n", suspeito);
    }
    
    printf("Pista adicionada ao seu caderno de investigação!\n");
    
    raizPistas = inserirPista(raizPistas, sala->pista);
    return raizPistas;
}

/**
 * Função: inicializarTabelaHash
 * Propósito: Inicializa a tabela hash com as associações pista-suspeito
 */
void inicializarTabelaHash(HashNode** tabelaHash) {
    // Inicializa todas as posições como NULL
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
    
    // Insere associações pista-suspeito
    inserirNaHash(tabelaHash, "Porta principal arrombada", "Carlos");
    inserirNaHash(tabelaHash, "Copo de vinho pela metade na mesa", "Ana");
    inserirNaHash(tabelaHash, "Faca desaparecida do bloco de facas", "Roberto");
    inserirNaHash(tabelaHash, "Livro sobre venenos aberto na mesa", "Maria");
    inserirNaHash(tabelaHash, "Pegadas de barro levando à cerca", "Carlos");
    inserirNaHash(tabelaHash, "Documento importante sumiu da gaveta", "Roberto");
    inserirNaHash(tabelaHash, "Relógio de parede parado às 23:47", "Ana");
    inserirNaHash(tabelaHash, "Baú antigo com documentos secretos", "Maria");
    inserirNaHash(tabelaHash, "Carta de amor rasgada no cesto", "Pedro");
    inserirNaHash(tabelaHash, "Manchas de tinta na carpete", "Pedro");
}

/**
 * Função: contarPistasPorSuspeito
 * Propósito: Conta quantas pistas apontam para cada suspeito
 */
void contarPistasPorSuspeito(PistaNode* raizPistas, HashNode** tabelaHash, SuspeitoContagem* contagens, int* numSuspeitos) {
    if (raizPistas == NULL) return;
    
    // Travessia in-order para processar todas as pistas
    contarPistasPorSuspeito(raizPistas->esquerda, tabelaHash, contagens, numSuspeitos);
    
    char* suspeito = encontrarSuspeito(tabelaHash, raizPistas->pista);
    int encontrado = 0;
    
    // Procura se o suspeito já está no array
    for (int i = 0; i < *numSuspeitos; i++) {
        if (strcmp(contagens[i].nome, suspeito) == 0) {
            contagens[i].contagem++;
            encontrado = 1;
            break;
        }
    }
    
    // Se não encontrou, adiciona novo suspeito
    if (!encontrado && *numSuspeitos < MAX_SUSPEITOS) {
        strcpy(contagens[*numSuspeitos].nome, suspeito);
        contagens[*numSuspeitos].contagem = 1;
        (*numSuspeitos)++;
    }
    
    contarPistasPorSuspeito(raizPistas->direita, tabelaHash, contagens, numSuspeitos);
}

/**
 * Função: verificarSuspeitoFinal
 * Propósito: Conduz à fase de julgamento final e verifica a acusação
 */
void verificarSuspeitoFinal(PistaNode* raizPistas, HashNode** tabelaHash) {
    printf("\n⚖️  === FASE DE JULGAMENTO FINAL === ⚖️\n");
    
    if (raizPistas == NULL) {
        printf("❌ Você não coletou nenhuma pista! Não é possível acusar ninguém.\n");
        return;
    }
    
    // Exibe pistas coletadas
    printf("\n📓 Pistas coletadas durante a investigação:\n");
    exibirPistas(raizPistas);
    
    // Conta pistas por suspeito
    SuspeitoContagem contagens[MAX_SUSPEITOS];
    int numSuspeitos = 0;
    contarPistasPorSuspeito(raizPistas, tabelaHash, contagens, &numSuspeitos);
    
    // Exibe análise das pistas
    printf("\n🔎 Análise das pistas por suspeito:\n");
    for (int i = 0; i < numSuspeitos; i++) {
        printf("   %s: %d pista(s)\n", contagens[i].nome, contagens[i].contagem);
    }
    
    // Solicita acusação do jogador
    char acusacao[50];
    printf("\n🗣️  Quem você acusa como culpado? ");
    scanf(" %[^\n]", acusacao);
    
    // Verifica se há pistas suficientes para a acusação
    int pistasParaAcusacao = 0;
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcasecmp(contagens[i].nome, acusacao) == 0) {
            pistasParaAcusacao = contagens[i].contagem;
            break;
        }
    }
    
    printf("\n=== VEREDITO ===\n");
    if (pistasParaAcusacao >= MIN_PISTAS_CULPADO) {
        printf("✅ ACUSAÇÃO SUSTENTADA!\n");
        printf("Você apresentou %d pistas contra %s.\n", pistasParaAcusacao, acusacao);
        printf("🔒 %s foi preso pelo crime! Caso resolvido! 🕵️‍♂️\n", acusacao);
    } else if (pistasParaAcusacao > 0) {
        printf("⚠️  ACUSAÇÃO FRACA!\n");
        printf("Você tem apenas %d pista(s) contra %s.\n", pistasParaAcusacao, acusacao);
        printf("São necessárias pelo menos %d pistas para uma condenação.\n", MIN_PISTAS_CULPADO);
        printf("O suspeito foi liberado por falta de provas.\n");
    } else {
        printf("❌ ACUSAÇÃO INCONSISTENTE!\n");
        printf("Não há nenhuma pista ligando %s ao crime.\n", acusacao);
        printf("O caso permanece aberto...\n");
    }
}

/**
 * Função: explorarSalas
 * Propósito: Navega pela árvore e ativa o sistema de pistas
 */
PistaNode* explorarSalas(Sala* salaAtual, PistaNode* raizPistas, HashNode** tabelaHash) {
    char opcao;
    
    printf("\n=== DETECTIVE QUEST ===\n");
    printf("📍 Você está na: %s\n", salaAtual->nome);
    
    // Coleta pista da sala atual (se houver)
    raizPistas = coletarPista(salaAtual, raizPistas, tabelaHash);
    
    // Mostra opções disponíveis
    printf("\nOpções de exploração:\n");
    if (salaAtual->esquerda != NULL) {
        printf("[e] - Ir para ESQUERDA (%s)\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("[d] - Ir para DIREITA (%s)\n", salaAtual->direita->nome);
    }
    printf("[v] - Visualizar pistas coletadas\n");
    printf("[s] - Sair da investigação e fazer acusação\n");
    
    // Loop para obter uma opção válida
    while (1) {
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    printf("\n➡️  Indo para a esquerda...\n");
                    raizPistas = explorarSalas(salaAtual->esquerda, raizPistas, tabelaHash);
                    return raizPistas;
                } else {
                    printf("❌ Não há caminho à esquerda! Escolha outra opção.\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    printf("\n➡️  Indo para a direita...\n");
                    raizPistas = explorarSalas(salaAtual->direita, raizPistas, tabelaHash);
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
 * Propósito: Libera memória da árvore de salas
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
 * Propósito: Libera memória da árvore BST de pistas
 */
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarArvorePistas(raiz->esquerda);
        liberarArvorePistas(raiz->direita);
        free(raiz);
    }
}

/**
 * Função: liberarTabelaHash
 * Propósito: Libera memória da tabela hash
 */
void liberarTabelaHash(HashNode** tabelaHash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}

/**
 * Função: main
 * Propósito: Monta o mapa da mansão e inicia a exploração
 */
int main() {
    printf("🕵️  Bem-vindo ao Detective Quest - Sistema Completo de Investigação! 🕵️\n");
    printf("Explore a mansão, colete pistas, relacione suspeitos e descubra o culpado!\n");
    printf("Suspeitos: Carlos, Ana, Roberto, Maria, Pedro\n\n");
    
    // Montagem do mapa da mansão
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho pela metade na mesa");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco de facas");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto na mesa");
    Sala* jardim = criarSala("Jardim", "Pegadas de barro levando à cerca");
    Sala* escritorio = criarSala("Escritório", "Documento importante sumiu da gaveta");
    Sala* quarto = criarSala("Quarto Principal", "Relógio de parede parado às 23:47");
    Sala* porao = criarSala("Porão", "Baú antigo com documentos secretos");
    Sala* lavanderia = criarSala("Lavanderia", "Carta de amor rasgada no cesto");
    Sala* salaMusica = criarSala("Sala de Música", "Manchas de tinta na carpete");
    
    // Construção da estrutura da árvore
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = biblioteca;
    
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    
    biblioteca->esquerda = escritorio;
    biblioteca->direita = quarto;
    
    cozinha->esquerda = porao;
    jardim->direita = lavanderia;
    
    quarto->esquerda = salaMusica;
    
    // Inicializa sistemas
    HashNode* tabelaHash[TAMANHO_HASH];
    inicializarTabelaHash(tabelaHash);
    PistaNode* raizPistas = NULL;
    
    // Inicia exploração
    raizPistas = explorarSalas(hallEntrada, raizPistas, tabelaHash);
    
    // Fase final de julgamento
    verificarSuspeitoFinal(raizPistas, tabelaHash);
    
    // Libera memória
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
    liberarTabelaHash(tabelaHash);
    
    printf("\n🎮 Obrigado por jogar Detective Quest! 🎮\n");
    
    return 0;
}