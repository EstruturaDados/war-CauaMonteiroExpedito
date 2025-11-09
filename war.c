#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos das funções
void inicializarMapa(Territorio** mapa, int* tamanho);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char corJogador[]);
void exibirMapa(Territorio* mapa, int tamanho);
void exibirMissao(char* missao, char corJogador[]);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2);
int jogadorPodeAtacar(Territorio* mapa, int tamanho, char corJogador[]);
void realizarTurno(Territorio* mapa, int tamanho, char corJogador[], char* missao);

int main() {
    srand(time(NULL));
    
    printf("=== WAR ESTRATEGICO - MISSÕES INDIVIDUAIS ===\n\n");
    
    // Vetor de missões estratégicas
    char* missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar 5 territorios simultaneamente", 
        "Destruir 10 tropas inimigas",
        "Manter 2 territorios por 3 turnos consecutivos",
        "Conquistar todos os territorios da borda do mapa"
    };
    int totalMissoes = 6;
    
    // Inicializar mapa
    Territorio* mapa;
    int tamanhoMapa;
    inicializarMapa(&mapa, &tamanhoMapa);
    
    // Atribuir missões aos jogadores
    char* missaoJogador1 = (char*)malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*)malloc(100 * sizeof(char));
    
    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);
    
    printf("=== MISSÕES ATRIBUÍDAS ===\n");
    exibirMissao(missaoJogador1, "Azul");
    exibirMissao(missaoJogador2, "Vermelho");
    printf("\n");
    
    int turno = 1;
    int vencedor = 0;
    
    // Loop principal do jogo
    while (!vencedor) {
        printf("=== TURNO %d ===\n", turno);
        
        // Turno do Jogador 1 (Azul)
        printf("\n--- Jogador Azul ---\n");
        exibirMapa(mapa, tamanhoMapa);
        realizarTurno(mapa, tamanhoMapa, "Azul", missaoJogador1);
        
        // Verificar se Jogador 1 venceu
        if (verificarMissao(missaoJogador1, mapa, tamanhoMapa, "Azul")) {
            printf("\n🎉 JOGADOR AZUL VENCEU! Missão cumprida: %s\n", missaoJogador1);
            vencedor = 1;
            break;
        }
        
        // Turno do Jogador 2 (Vermelho)
        printf("\n--- Jogador Vermelho ---\n");
        exibirMapa(mapa, tamanhoMapa);
        realizarTurno(mapa, tamanhoMapa, "Vermelho", missaoJogador2);
        
        // Verificar se Jogador 2 venceu
        if (verificarMissao(missaoJogador2, mapa, tamanhoMapa, "Vermelho")) {
            printf("\n🎉 JOGADOR VERMELHO VENCEU! Missão cumprida: %s\n", missaoJogador2);
            vencedor = 2;
            break;
        }
        
        turno++;
        
        // Limite de turnos para evitar loop infinito
        if (turno > 20) {
            printf("\n⚡ Limite de turnos atingido! Jogo empatado.\n");
            break;
        }
    }
    
    // Liberar memória
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    
    return 0;
}

// Função para inicializar o mapa com os 7 continentes
void inicializarMapa(Territorio** mapa, int* tamanho) {
    *tamanho = 7;
    *mapa = (Territorio*)malloc(*tamanho * sizeof(Territorio));
    
    // Configuração dos 7 continentes
    strcpy((*mapa)[0].nome, "America do Norte");
    strcpy((*mapa)[0].cor, "Azul");
    (*mapa)[0].tropas = 6;
    
    strcpy((*mapa)[1].nome, "America Central");
    strcpy((*mapa)[1].cor, "Vermelho");
    (*mapa)[1].tropas = 4;
    
    strcpy((*mapa)[2].nome, "America do Sul");
    strcpy((*mapa)[2].cor, "Azul");
    (*mapa)[2].tropas = 5;
    
    strcpy((*mapa)[3].nome, "Europa");
    strcpy((*mapa)[3].cor, "Vermelho");
    (*mapa)[3].tropas = 5;
    
    strcpy((*mapa)[4].nome, "Africa");
    strcpy((*mapa)[4].cor, "Azul");
    (*mapa)[4].tropas = 6;
    
    strcpy((*mapa)[5].nome, "Asia");
    strcpy((*mapa)[5].cor, "Vermelho");
    (*mapa)[5].tropas = 7;
    
    strcpy((*mapa)[6].nome, "Oceania");
    strcpy((*mapa)[6].cor, "Azul");
    (*mapa)[6].tropas = 4;
    
    // Antártida não é incluída como território jogável (opcional)
    printf("Nota: A Antartida e considerada territorio neutro neste jogo.\n");
}

// Função para atribuir missão aleatória a um jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Função para verificar se a missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char corJogador[]) {
    int territoriosConquistados = 0;
    int territoriosJogador = 0;
    int territoriosVermelhos = 0;
    static int turnosMantidosAzul = 0;
    static int turnosMantidosVermelho = 0;
    
    // Contar estatísticas do mapa
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosJogador++;
        }
        if (strcmp(mapa[i].cor, "Vermelho") == 0) {
            territoriosVermelhos++;
        }
    }
    
    // Verificar diferentes tipos de missão
    if (strstr(missao, "3 territorios seguidos") != NULL) {
        // Missão: Conquistar 3 territórios seguidos
        for (int i = 0; i < tamanho - 2; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 &&
                strcmp(mapa[i+1].cor, corJogador) == 0 &&
                strcmp(mapa[i+2].cor, corJogador) == 0) {
                return 1;
            }
        }
    }
    else if (strstr(missao, "tropas da cor vermelha") != NULL) {
        // Missão: Eliminar todas as tropas vermelhas
        if (strcmp(corJogador, "Azul") == 0) {
            return territoriosVermelhos == 0;
        }
        return 0; // Vermelho não pode ter esta missão contra si mesmo
    }
    else if (strstr(missao, "5 territorios") != NULL) {
        // Missão: Controlar 5 territórios
        return territoriosJogador >= 5;
    }
    else if (strstr(missao, "10 tropas inimigas") != NULL) {
        // Missão: Destruir 10 tropas inimigas (simulação baseada em turnos)
        static int tropasDestruidasAzul = 0;
        static int tropasDestruidasVermelho = 0;
        
        if (strcmp(corJogador, "Azul") == 0) {
            tropasDestruidasAzul += rand() % 3;
            return tropasDestruidasAzul >= 10;
        } else {
            tropasDestruidasVermelho += rand() % 3;
            return tropasDestruidasVermelho >= 10;
        }
    }
    else if (strstr(missao, "2 territorios por 3 turnos") != NULL) {
        // Missão: Manter territórios
        if (strcmp(corJogador, "Azul") == 0) {
            if (territoriosJogador >= 2) {
                turnosMantidosAzul++;
            } else {
                turnosMantidosAzul = 0;
            }
            return turnosMantidosAzul >= 3;
        } else {
            if (territoriosJogador >= 2) {
                turnosMantidosVermelho++;
            } else {
                turnosMantidosVermelho = 0;
            }
            return turnosMantidosVermelho >= 3;
        }
    }
    else if (strstr(missao, "territorios da borda") != NULL) {
        // Missão: Conquistar territórios da borda (primeiro e último)
        return strcmp(mapa[0].cor, corJogador) == 0 && 
               strcmp(mapa[tamanho-1].cor, corJogador) == 0;
    }
    
    return 0;
}

// Função para exibir o mapa atual
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- MAPA ATUAL (7 CONTINENTES) ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s - %s (%d tropas)\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("-----------------------------------\n");
}

// Função para exibir a missão de um jogador
void exibirMissao(char* missao, char corJogador[]) {
    printf("Jogador %s: %s\n", corJogador, missao);
}

// Função de ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n⚔️  %s (%s) ataca %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    
    // Simulação de dados (valores entre 1-6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("Dados: Atacante %d x Defensor %d\n", dadoAtacante, dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        // Atacante vence
        printf("✅ Vitoria! %s conquista %s\n", atacante->cor, defensor->nome);
        
        // Transferir cor e metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= atacante->tropas / 2;
        
        printf("Novo dono: %s (%d tropas)\n", defensor->cor, defensor->tropas);
    } else {
        // Defensor vence
        printf("❌ Derrota! %s mantem %s\n", defensor->cor, defensor->nome);
        atacante->tropas--; // Atacante perde uma tropa
        printf("%s perde 1 tropa (%d restantes)\n", atacante->cor, atacante->tropas);
    }
}

// Função para verificar se jogador pode atacar
int jogadorPodeAtacar(Territorio* mapa, int tamanho, char corJogador[]) {
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas > 1) {
            // Verificar se há território inimigo adjacente (simplificado)
            for (int j = 0; j < tamanho; j++) {
                if (strcmp(mapa[j].cor, corJogador) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Função para realizar um turno completo
void realizarTurno(Territorio* mapa, int tamanho, char corJogador[], char* missao) {
    // Verificar se pode atacar
    if (!jogadorPodeAtacar(mapa, tamanho, corJogador)) {
        printf("❌ Nenhum ataque possivel neste turno.\n");
        return;
    }
    
    // Escolher território atacante
    int idxAtacante;
    do {
        printf("Escolha territorio ATACANTE (1-%d): ", tamanho);
        scanf("%d", &idxAtacante);
        idxAtacante--;
        
        if (idxAtacante < 0 || idxAtacante >= tamanho) {
            printf("Territorio invalido!\n");
            continue;
        }
        
        if (strcmp(mapa[idxAtacante].cor, corJogador) != 0) {
            printf("Voce so pode atacar com seus proprios territorios!\n");
            continue;
        }
        
        if (mapa[idxAtacante].tropas <= 1) {
            printf("Territorio precisa de pelo menos 2 tropas para atacar!\n");
            continue;
        }
        
        break;
    } while (1);
    
    // Escolher território defensor
    int idxDefensor;
    do {
        printf("Escolha territorio DEFENSOR (1-%d): ", tamanho);
        scanf("%d", &idxDefensor);
        idxDefensor--;
        
        if (idxDefensor < 0 || idxDefensor >= tamanho) {
            printf("Territorio invalido!\n");
            continue;
        }
        
        if (strcmp(mapa[idxDefensor].cor, corJogador) == 0) {
            printf("Voce nao pode atacar seus proprios territorios!\n");
            continue;
        }
        
        if (idxDefensor == idxAtacante) {
            printf("Nao pode atacar o mesmo territorio!\n");
            continue;
        }
        
        break;
    } while (1);
    
    // Realizar ataque
    atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
}

// Função para liberar memória alocada
void liberarMemoria(Territorio* mapa, char* missaoJogador1, char* missaoJogador2) {
    free(mapa);
    free(missaoJogador1);
    free(missaoJogador2);
    printf("\n✅ Memoria liberada com sucesso!\n");
}