#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "hash.cpp"
#include <tuple>
using namespace std; 

int main() {
    readCsv ComprasCSV("compras.csv");
    vector<Compras> compras_vetor = ComprasCSV.lerArquivo();
    // struct do hash.cpp
    ofstream outTxt("out.txt");
    ifstream inTxt("in.txt");
    string OP, YYYY, linha, line;

    if (!inTxt.is_open() || !outTxt.is_open()) {
        cerr << "Erro ao abrir um dos arquivos!" << endl;
        return 1;
    }

    getline(inTxt, linha);
    int PG = stoi(linha.substr(3));
    outTxt << linha << endl;

    cout << PG << '\n';

    Directory dir = Directory(PG);

    while(getline(inTxt, line)) {
        stringstream ss(line);
        int ano;
        string operacao = line.substr(0, 3);
        cout<<operacao;
        

        if (operacao == "INC") {
            ano = stoi(line.substr(4));
            cout<<ano<<endl;
            outTxt << "INC:" << ano << "/" << endl;
            for (const auto& compra : compras_vetor) {
                if (compra.ano == to_string(ano)) {
                    cout<<compra.ano<<endl;
                    pair<int, int> resultadoInsercao = dir.adicionar(compra);
                    
                    outTxt << "INC:" << ano << "/" << dir.profundidadeGlobal << "," << resultadoInsercao.first << endl;

                    if (resultadoInsercao.second) {
                        outTxt << "DUP_DIR:/" << dir.profundidadeGlobal << "," << resultadoInsercao.first << endl;
                    }
                }
            }
        } else if (operacao == "REM") {
            ano = stoi(line.substr(4));
            outTxt << "REM:" << ano << "/";
            // <quantidade de tuplas removidas, profundidade local>
            pair<int, int> resultadoRemocao = dir.remover_registro(ano);

            outTxt << "REM:" << ano << "/" << resultadoRemocao.first << "," << dir.profundidadeGlobal << "," << resultadoRemocao.second << endl;
        } else if (operacao == "BUS") {
            outTxt << "BUS:" << ano << "/";
            ano = stoi(line.substr(5));
            int quantidadeDeTuplas = dir.busca(ano);
            outTxt << "BUS:" << ano << "/" << quantidadeDeTuplas << endl;
        } else {
            outTxt << "NÃO DEU CERTO";
        }
    
    }

    outTxt << "P:/" << (dir.profundidadeGlobal) << endl;

    inTxt.close(); 
    outTxt.close();

    return 0;
}