#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SALAS 5
#define DIAS_SEMANA 5
#define SLOTS_DIA 10
#define TAM_CODIGO 20

typedef char CodigoSolicitante[TAM_CODIGO];

typedef struct {
    int idx;
    float ocupacao;
} RankItem;

const char *nome_dia[] = {"Seg", "Ter", "Qua", "Qui", "Sex"};

char nomes_salas[MAX_SALAS][50] = {"LAB1", "LAB2", "MAPA1", "LAB3", "LAB4"};
CodigoSolicitante agenda[MAX_SALAS][DIAS_SEMANA][SLOTS_DIA];

void inicializar_agenda();
int esta_vazio(const char *str);
void limpar_buffer();
void reservar(int sala_idx, int dia, int slot, const CodigoSolicitante solicitante);
void cancelar(int sala_idx, int dia, int slot);
void listar_agenda_sala(int sala_idx);
float ocupacao_por_sala(int sala_idx);
void primeiro_horario_livre(int sala_idx);
void ranking_salas();
void buscar_solicitante(const char *substring);
void menu();

int main() {
    inicializar_agenda();
    menu();
    return 0;
}


void inicializar_agenda() {
    for (int i = 0; i < MAX_SALAS; i++) {
        for (int j = 0; j < DIAS_SEMANA; j++) {
            for (int k = 0; k < SLOTS_DIA; k++) {
                strcpy(agenda[i][j][k], "");
            }
        }
    }
}

int esta_vazio(const char *str) {
    return strlen(str) == 0;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void reservar(int sala_idx, int dia, int slot, const CodigoSolicitante solicitante) {
    if (sala_idx < 0 || sala_idx >= MAX_SALAS || dia < 0 || dia >= DIAS_SEMANA || slot < 0 || slot >= SLOTS_DIA) {
        printf("Erro: Indices de sala, dia ou slot invalidos.\n");
        return;
    }

    if (!esta_vazio(agenda[sala_idx][dia][slot])) {
        printf("Erro: O horario ja esta reservado por %s.\n", agenda[sala_idx][dia][slot]);
        return;
    }

    strcpy(agenda[sala_idx][dia][slot], solicitante);
    printf("Reserva realizada com sucesso para %s.\n", solicitante);
}

void cancelar(int sala_idx, int dia, int slot) {
    if (sala_idx < 0 || sala_idx >= MAX_SALAS || dia < 0 || dia >= DIAS_SEMANA || slot < 0 || slot >= SLOTS_DIA) {
        printf("Erro: Indices de sala, dia ou slot invalidos.\n");
        return;
    }

    if (esta_vazio(agenda[sala_idx][dia][slot])) {
        printf("Erro: O horario ja esta livre.\n");
        return;
    }

    strcpy(agenda[sala_idx][dia][slot], "");
    printf("Reserva cancelada com sucesso.\n");
}

void listar_agenda_sala(int sala_idx) {
    if (sala_idx < 0 || sala_idx >= MAX_SALAS) {
        printf("Sala invalida.\n");
        return;
    }

    printf("\n--- Agenda de %s ---\n", nomes_salas[sala_idx]);
    printf("Horario\t");
    for (int d = 0; d < DIAS_SEMANA; d++) {
        printf("%-10s", nome_dia[d]);
    }
    printf("\n");

    for (int s = 0; s < SLOTS_DIA; s++) {
        printf("%d-%d\t", s, s + 1);
        for (int d = 0; d < DIAS_SEMANA; d++) {
            if (esta_vazio(agenda[sala_idx][d][s])) {
                printf("%-10s", "-");
            } else {
                printf("%-10s", agenda[sala_idx][d][s]);
            }
        }
        printf("\n");
    }
    printf("-------------------------\n");
}

float ocupacao_por_sala(int sala_idx) {
    if (sala_idx < 0 || sala_idx >= MAX_SALAS) {
        return 0.0;
    }

    int total_slots = DIAS_SEMANA * SLOTS_DIA;
    int slots_ocupados = 0;

    for (int d = 0; d < DIAS_SEMANA; d++) {
        for (int s = 0; s < SLOTS_DIA; s++) {
            if (!esta_vazio(agenda[sala_idx][d][s])) {
                slots_ocupados++;
            }
        }
    }
    return ((float)slots_ocupados / total_slots) * 100.0;
}

void primeiro_horario_livre(int sala_idx) {
    if (sala_idx < 0 || sala_idx >= MAX_SALAS) {
        printf("Sala invalida.\n");
        return;
    }

    for (int d = 0; d < DIAS_SEMANA; d++) {
        for (int s = 0; s < SLOTS_DIA; s++) {
            if (esta_vazio(agenda[sala_idx][d][s])) {
                printf("O primeiro horario livre em %s e: %s, slot %d-%d\n", nomes_salas[sala_idx], nome_dia[d], s, s + 1);
                return;
            }
        }
    }
    printf("Nao ha horarios livres para %s nesta semana.\n", nomes_salas[sala_idx]);
}


void ranking_salas() {
    RankItem ranking[MAX_SALAS];

    for (int i = 0; i < MAX_SALAS; i++) {
        ranking[i].idx = i;
        ranking[i].ocupacao = ocupacao_por_sala(i);
    }

    for (int i = 0; i < MAX_SALAS - 1; i++) {
        for (int j = 0; j < MAX_SALAS - i - 1; j++) {
            if (ranking[j].ocupacao < ranking[j + 1].ocupacao) {
                RankItem temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    printf("\n--- Ranking de Ocupacao das Salas ---\n");
    for (int i = 0; i < MAX_SALAS; i++) {
        printf("%d. lugar: %s (%.2f%% ocupacao)\n", i + 1, nomes_salas[ranking[i].idx], ranking[i].ocupacao);
    }
}


void buscar_solicitante(const char *substring) {
    int encontrado = 0;
    printf("\n--- Buscando por '%s' ---\n", substring);

    char substring_lower[TAM_CODIGO];
    strcpy(substring_lower, substring);
    for (int c = 0; substring_lower[c]; c++) {
        substring_lower[c] = tolower(substring_lower[c]);
    }

    for (int i = 0; i < MAX_SALAS; i++) {
        for (int d = 0; d < DIAS_SEMANA; d++) {
            for (int s = 0; s < SLOTS_DIA; s++) {
                if (!esta_vazio(agenda[i][d][s])) {
                    char codigo_lower[TAM_CODIGO];
                    strcpy(codigo_lower, agenda[i][d][s]);
                    for (int c = 0; codigo_lower[c]; c++) {
                        codigo_lower[c] = tolower(codigo_lower[c]);
                    }

                    if (strstr(codigo_lower, substring_lower) != NULL) {
                        printf("Encontrado em %s, %s, slot %d-%d. Solicitante: %s\n",
                               nomes_salas[i], nome_dia[d], s, s + 1, agenda[i][d][s]);
                        encontrado = 1;
                    }
                }
            }
        }
    }

    if (!encontrado) {
        printf("Nenhum resultado encontrado.\n");
    }
}


void menu() {
    int opcao, sala_idx, dia, slot;
    CodigoSolicitante solicitante;
    char substring[TAM_CODIGO];

    do {
        printf("\n--- Sistema de Reserva de Laboratorios ---\n");
        printf("1. Reservar laboratorio\n");
        printf("2. Cancelar reserva\n");
        printf("3. Listar agenda de uma sala\n");
        printf("4. Porcentagem de ocupacao de uma sala\n");
        printf("5. Primeiro horario livre de uma sala\n");
        printf("6. Ranking de salas mais utilizadas\n");
        printf("7. Buscar por solicitante\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                printf("Indice da sala (0 a %d): ", MAX_SALAS - 1);
                scanf("%d", &sala_idx);
                printf("Dia (0-Seg a 4-Sex): ");
                scanf("%d", &dia);
                printf("Slot (0 a %d): ", SLOTS_DIA - 1);
                scanf("%d", &slot);
                limpar_buffer();
                printf("Codigo do solicitante: ");
                fgets(solicitante, TAM_CODIGO, stdin);
                solicitante[strcspn(solicitante, "\n")] = 0;
                reservar(sala_idx, dia, slot, solicitante);
                break;
            case 2:
                printf("Indice da sala (0 a %d): ", MAX_SALAS - 1);
                scanf("%d", &sala_idx);
                printf("Dia (0-Seg a 4-Sex): ");
                scanf("%d", &dia);
                printf("Slot (0 a %d): ", SLOTS_DIA - 1);
                scanf("%d", &slot);
                cancelar(sala_idx, dia, slot);
                break;
            // --- CORREÇÃO A PARTIR DAQUI ---
            case 3:
                printf("Indice da sala (0 a %d): ", MAX_SALAS - 1);
                scanf("%d", &sala_idx); // Linha completada
                listar_agenda_sala(sala_idx); // Código restaurado
                break;                    // Código restaurado
            case 4:
                printf("Indice da sala (0 a %d): ", MAX_SALAS - 1);
                scanf("%d", &sala_idx);
                printf("A ocupacao de %s e de %.2f%%\n", nomes_salas[sala_idx], ocupacao_por_sala(sala_idx));
                break;
            case 5:
                printf("Indice da sala (0 a %d): ", MAX_SALAS - 1);
                scanf("%d", &sala_idx);
                primeiro_horario_livre(sala_idx);
                break;
            case 6:
                ranking_salas();
                break;
            case 7:
                printf("Digite a substring do solicitante: ");
                fgets(substring, TAM_CODIGO, stdin);
                substring[strcspn(substring, "\n")] = 0;
                buscar_solicitante(substring);
                break;
            case 0:
                printf("Saindo do sistema. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
}