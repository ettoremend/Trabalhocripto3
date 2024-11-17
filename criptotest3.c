#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_TRANSACOES 100
#define MAX_CRIPTOMOEDAS 10

// Estruturas
typedef struct {
    char data[20];
    char tipoOperacao[10]; // compra ou venda
    char criptomoeda[20];
    float valor;
    float taxa;
} Transacao;

typedef struct {
    char cpf[12];
    char senha[20];
    float saldoReais;
    float saldoCriptomoedas[MAX_CRIPTOMOEDAS]; // Saldo para cada criptomoeda
    Transacao transacoes[MAX_TRANSACOES];
    int numTransacoes;
} Usuario;

typedef struct {
    char nome[20];
    float cotacaoInicial;
    float taxaCompra;
    float taxaVenda;
} Criptomoeda;

// Dados globais
Usuario usuarios[MAX_USERS];
int totalUsuarios = 0;

Criptomoeda criptomoedas[MAX_CRIPTOMOEDAS];
int totalCriptomoedas = 0;

// Funções gerais
void obterDataAtual(char *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(data, "%02d/%02d/%04d %02d:%02d:%02d",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void carregarDados() {
    FILE *file = fopen("dados.dat", "rb");
    if (file != NULL) {
        fread(&totalUsuarios, sizeof(int), 1, file);
        fread(usuarios, sizeof(Usuario), totalUsuarios, file);
        fread(&totalCriptomoedas, sizeof(int), 1, file);
        fread(criptomoedas, sizeof(Criptomoeda), totalCriptomoedas, file);
        fclose(file);
    }
}

void salvarDados() {
    FILE *file = fopen("dados.dat", "wb");
    fwrite(&totalUsuarios, sizeof(int), 1, file);
    fwrite(usuarios, sizeof(Usuario), totalUsuarios, file);
    fwrite(&totalCriptomoedas, sizeof(int), 1, file);
    fwrite(criptomoedas, sizeof(Criptomoeda), totalCriptomoedas, file);
    fclose(file);
}

// Funções para administrador
int efetuarLoginAdministrador() {
    char senha[20];
    printf("Digite a senha do administrador: ");
    scanf("%s", senha);

    // Senha fixa para exemplo
    if (strcmp(senha, "admin123") == 0) {
        printf("Login como administrador realizado com sucesso!\n");
        return 1;
    }

    printf("Senha incorreta!\n");
    return 0;
}

void cadastrarInvestidor() {
    if (totalUsuarios >= MAX_USERS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    Usuario novoUsuario;
    printf("Digite o CPF: ");
    scanf("%s", novoUsuario.cpf);

    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].cpf, novoUsuario.cpf) == 0) {
            printf("Este CPF já está cadastrado!\n");
            return;
        }
    }

    printf("Digite a senha: ");
    scanf("%s", novoUsuario.senha);

    novoUsuario.saldoReais = 0.0;
    memset(novoUsuario.saldoCriptomoedas, 0, sizeof(novoUsuario.saldoCriptomoedas));
    novoUsuario.numTransacoes = 0;

    usuarios[totalUsuarios++] = novoUsuario;
    printf("Investidor cadastrado com sucesso!\n");
}

void excluirInvestidor() {
    char cpf[12];
    printf("Digite o CPF do investidor a ser excluído: ");
    scanf("%s", cpf);

    for (int i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            printf("CPF: %s, Saldo em Reais: %.2f\n", usuarios[i].cpf, usuarios[i].saldoReais);
            printf("Confirma exclusão? (1-Sim / 0-Não): ");
            int confirmacao;
            scanf("%d", &confirmacao);
            if (confirmacao == 1) {
                for (int j = i; j < totalUsuarios - 1; j++) {
                    usuarios[j] = usuarios[j + 1];
                }
                totalUsuarios--;
                printf("Investidor excluído com sucesso!\n");
                return;
            } else {
                printf("Exclusão cancelada.\n");
                return;
            }
        }
    }
    printf("Investidor não encontrado!\n");
}

void cadastrarCriptomoeda() {
    if (totalCriptomoedas >= MAX_CRIPTOMOEDAS) {
        printf("Limite de criptomoedas atingido!\n");
        return;
    }

    Criptomoeda novaCriptomoeda;
    printf("Digite o nome da criptomoeda: ");
    scanf("%s", novaCriptomoeda.nome);
    printf("Digite a cotação inicial: ");
    scanf("%f", &novaCriptomoeda.cotacaoInicial);
    printf("Digite a taxa de compra: ");
    scanf("%f", &novaCriptomoeda.taxaCompra);
    printf("Digite a taxa de venda: ");
    scanf("%f", &novaCriptomoeda.taxaVenda);

    criptomoedas[totalCriptomoedas++] = novaCriptomoeda;
    printf("Criptomoeda cadastrada com sucesso!\n");
}

void excluirCriptomoeda() {
    char nome[20];
    printf("Digite o nome da criptomoeda a ser excluída: ");
    scanf("%s", nome);

    for (int i = 0; i < totalCriptomoedas; i++) {
        if (strcmp(criptomoedas[i].nome, nome) == 0) {
            printf("Nome: %s, Cotação: %.2f\n", criptomoedas[i].nome, criptomoedas[i].cotacaoInicial);
            printf("Confirma exclusão? (1-Sim / 0-Não): ");
            int confirmacao;
            scanf("%d", &confirmacao);
            if (confirmacao == 1) {
                for (int j = i; j < totalCriptomoedas - 1; j++) {
                    criptomoedas[j] = criptomoedas[j + 1];
                }
                totalCriptomoedas--;
                printf("Criptomoeda excluída com sucesso!\n");
                return;
            } else {
                printf("Exclusão cancelada.\n");
                return;
            }
        }
    }
    printf("Criptomoeda não encontrada!\n");
}

// Menu principal
void menuAdministrador() {
    int opcao;

    do {
        printf("\n--- Menu Administrador ---\n");
        printf("1. Cadastrar Investidor\n");
        printf("2. Excluir Investidor\n");
        printf("3. Cadastrar Criptomoeda\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarInvestidor();
                break;
            case 2:
                excluirInvestidor();
                break;
            case 3:
                cadastrarCriptomoeda();
                break;
            case 4:
                excluirCriptomoeda();
                break;
            case 5:
                salvarDados();
                printf("Saindo do modo administrador...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 5);
}

void menuPrincipal() {
    int escolha;

    do {
        printf("\n--- Sistema Exchange de Criptomoedas ---\n");
        printf("1. Entrar como Administrador\n");
        printf("2. Entrar como Investidor\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                if (efetuarLoginAdministrador()) {
                    menuAdministrador();
                }
                break;
            case 2:
                printf("Funcionalidade de investidor ainda não implementada nesta unificação.\n");
                break;
            case 3:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 3);
}

int main() {
    carregarDados();
    menuPrincipal();
    return 0;
}