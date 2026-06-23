#include <iostream>
#include <string>
#include <fstream>

const int MAX_CONTAS = 50;
const int MAX_EXTRATO = 100;

std::string titulares[MAX_CONTAS];
double       saldos[MAX_CONTAS];
std::string  extrato[MAX_CONTAS][MAX_EXTRATO];
int          qtdExtrato[MAX_CONTAS];
int          totalContas = 0;



int buscarConta(const std::string& titular) {
    for (int i = 0; i < totalContas; i++) {
        if (titulares[i] == titular) return i;
    }
    return -1;
}

void registrarExtrato(int idx, const std::string& operacao) {
    if (qtdExtrato[idx] < MAX_EXTRATO) {
        extrato[idx][qtdExtrato[idx]++] = operacao;
    }
}


std::string formatarValor(double v) {
    int inteiro = (int)v;
    int centavos = (int)((v - inteiro) * 100 + 0.5);
    std::string s = std::to_string(inteiro) + ".";
    if (centavos < 10) s += "0";
    s += std::to_string(centavos);
    return s;
}


void criarConta(const std::string& titular, double saldoInicial) {
    if (totalContas >= MAX_CONTAS) {
        std::cout << "[erro] Limite de contas atingido.\n"; return;
    }
    if (buscarConta(titular) != -1) {
        std::cout << "[erro] Titular '" << titular << "' ja possui conta.\n"; return;
    }
    if (saldoInicial < 0) {
        std::cout << "[erro] Saldo inicial nao pode ser negativo.\n"; return;
    }
    titulares[totalContas]   = titular;
    saldos[totalContas]      = saldoInicial;
    qtdExtrato[totalContas]  = 0;
    registrarExtrato(totalContas, "Conta criada | Saldo inicial: R$ " + formatarValor(saldoInicial));
    totalContas++;
    std::cout << "[OK] Conta de '" << titular << "' criada. Saldo: R$ " << formatarValor(saldoInicial) << "\n";
}

void depositar(const std::string& titular, double valor) {
    int idx = buscarConta(titular);
    if (idx == -1) { std::cout << "[ERRO] Conta nao encontrada.\n"; return; }
    if (valor <= 0) { std::cout << "[ERRO] Valor de deposito deve ser positivo.\n"; return; }
    saldos[idx] += valor;
    registrarExtrato(idx, "+ Deposito    $ " + formatarValor(valor) + " | Saldo: $ " + formatarValor(saldos[idx]));
    std::cout << "[BLZ] Deposito de R$ " << formatarValor(valor) << " realizado. Novo saldo: $ " << formatarValor(saldos[idx]) << "\n";
}

void sacar(const std::string& titular, double valor) {
    int idx = buscarConta(titular);
    if (idx == -1) { std::cout << "[erro] Conta nao encontrada.\n"; return; }
    if (valor <= 0) { std::cout << "[erro] Valor de saque deve ser positivo.\n"; return; }
    if (valor > saldos[idx]) { std::cout << "[erro] Saldo insuficiente. Saldo atual: $ " << formatarValor(saldos[idx]) << "\n"; return; }
    saldos[idx] -= valor;
    registrarExtrato(idx, "- Saque       $ " + formatarValor(valor) + " | Saldo: $ " + formatarValor(saldos[idx]));
    std::cout << "[BLZ] Saque de R$ " << formatarValor(valor) << " realizado. Novo saldo: $ " << formatarValor(saldos[idx]) << "\n";
}

void transferir(const std::string& origem, const std::string& destino, double valor) {
    int idxO = buscarConta(origem);
    int idxD = buscarConta(destino);
    if (idxO == -1) { std::cout << "[erro] Conta de origem nao encontrada.\n"; return; }
    if (idxD == -1) { std::cout << "[erro] Conta de destino nao encontrada.\n"; return; }
    if (valor <= 0) { std::cout << "[erro] Valor deve ser positivo.\n"; return; }
    if (valor > saldos[idxO]) { std::cout << "[erro] Saldo insuficiente.\n"; return; }
    saldos[idxO] -= valor;
    saldos[idxD] += valor;
    registrarExtrato(idxO, "- Transf. p/ " + destino + "  $ " + formatarValor(valor) + " | Saldo: $ " + formatarValor(saldos[idxO]));
    registrarExtrato(idxD, "+ Transf. de " + origem  + "  $ " + formatarValor(valor) + " | Saldo: $ " + formatarValor(saldos[idxD]));
    std::cout << "[BLZ] Transferencia de $ " << formatarValor(valor) << " de '" << origem << "' para '" << destino << "' realizada.\n";
}

void exibirExtrato(const std::string& titular) {
    int idx = buscarConta(titular);
    if (idx == -1) { std::cout << "[erro ]Conta nao encontrada.\n"; return; }
    std::cout << "\n EXTRATO " << titulares[idx] << "=\n";
    if (qtdExtrato[idx] == 0) {
        std::cout << "  Sem movimentacoes.\n";
    } else {
        for (int i = 0; i < qtdExtrato[idx]; i++) {
            std::cout << "  " << i+1 << ". " << extrato[idx][i] << "\n";
        }
    }
    std::cout << "  Saldo atual: $ " << formatarValor(saldos[idx]) << "\n";
    std::cout << "LINHAAAAAAAAAAAAAAAAAA\n";
}

void listarContas() {
    if (totalContas == 0) { std::cout << "[info] Nenhuma conta cadastrada.\n"; return; }
    std::cout << "\n CONTAS CADASTRADAS \n";
    for (int i = 0; i < totalContas; i++) {
        std::cout << i+1 << ". " << titulares[i]
                  << " | Saldo: $ " << formatarValor(saldos[i]) << "\n";
    }
    std::cout << "\n";
}

void salvarArquivo(const std::string& arquivo) {
    std::ofstream f(arquivo);
    if (!f.is_open()) { std::cout << "[erro] nao foi possivel abrir o arquivo.\n"; return; }
    f << totalContas << "\n";
    for (int i = 0; i < totalContas; i++) {
        f << titulares[i] << "\n" << saldos[i] << "\n" << qtdExtrato[i] << "\n";
        for (int j = 0; j < qtdExtrato[i]; j++) f << extrato[i][j] << "\n";
    }
    f.close();
    std::cout << "[BLZ] Dados salvos em '" << arquivo << "'.\n";
}

void carregarArquivo(const std::string& arquivo) {
    std::ifstream f(arquivo);
    if (!f.is_open()) { std::cout << "[erro] Arquivo '" << arquivo << "' nao encontrado.\n"; return; }
    f >> totalContas; f.ignore();
    for (int i = 0; i < totalContas; i++) {
        std::getline(f, titulares[i]);
        f >> saldos[i] >> qtdExtrato[i]; f.ignore();
        for (int j = 0; j < qtdExtrato[i]; j++) std::getline(f, extrato[i][j]);
    }
    f.close();
    std::cout << "[BLZ] Dados carregados de '" << arquivo << "'.\n";
}


// CAIXA REGISTRADORA


void exibirMenu() {
    std::cout << " DADOS DA CAIXA REGISTRADORA \n";
    std::cout << "1. Entrar na conta\n";
    std::cout << "2. Depositar\n";
    std::cout << "3. Sacar\n";
    std::cout << "4. Transferir\n";
    std::cout << "5. Exibir extrato\n";
    std::cout << "6. Dados Recentes \n";
    std::cout << "0. Sair\n";
    std::cout << "Opcao: ";
}



int main() {
    int opcao;

    do {
        exibirMenu();
        std::cin >> opcao;
        std::cin.ignore();

        if (opcao == 1) {
            std::string titular;
            double saldo;
            std::cout << "Nome do titular: ";
            std::getline(std::cin, titular);
            std::cout << "Saldo inicial: $ ";
            std::cin >> saldo; std::cin.ignore();
            criarConta(titular, saldo);

        } else if (opcao == 2) {
            std::string titular;
            double valor;
            std::cout << ": ";
            std::getline(std::cin, titular);
            std::cout << "Valor: $ ";
            std::cin >> valor; std::cin.ignore();
            depositar(titular, valor);

        } else if (opcao == 3) {
            std::string titular;
            double valor;
            std::cout << "pessoa vinculando  : ";
            std::getline(std::cin, titular);
            std::cout << "Valora: $ ";
            std::cin >> valor; std::cin.ignore();
            sacar(titular, valor);

        } else if (opcao == 4) {
            std::string origem, destino;
            double valor;
            std::cout << ":pessoa vinculando ";
            std::getline(std::cin, origem);
            std::cout << "pessoa vinculando: ";
            std::getline(std::cin, destino);
            std::cout << "Valor: $ ";
            std::cin >> valor; std::cin.ignore();
            transferir(origem, destino, valor);

        } else if (opcao == 5) {
            std::string titular;
            std::cout << ": ";
            std::getline(std::cin, titular);
            exibirExtrato(titular);

        } else if (opcao == 6) {
            listarContas();

      

        } else if (opcao != 0) {
            std::cout << "[error] Opcao invalida.\n";
        }

    } while (opcao != 0);

    std::cout << "Encerrando!!!\n";
    return 0;
}