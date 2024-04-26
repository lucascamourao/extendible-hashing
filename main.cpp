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
    string line;
    readTxt inTxt("in.txt"); // struct do hash.cpp
    ofstream outTxt("out.txt");
    string OP, YYYY;

    if (!outTxt.is_open()) {
        cout << "Erro ao abrir um o arquivo \n";
        return 0;
    }

    int PG = inTxt.PG();
    cout << PG << '\n';

    Directory dir = Directory(PG);

    vector<OperationYear> vector_OpYear = inTxt.vector_OpYear();

    for (int i = 0; i < vector_OpYear.size(); i++){
        tie(OP,YYYY) = vector_OpYear[i];
        if (OP == "INC") {
            for (const auto& compra : compras_vetor) {
                if (compra.ano == YYYY) {
                    pair<int, int> resultadoInsercao = dir.adicionar(compra);
                    
                    
                    outTxt << "INC:" << YYYY << "/" << dir.profundidadeGlobal << "," << resultadoInsercao.first << endl;

                    if (resultadoInsercao.second) {
                        outTxt << "DUP_DIR:/" << dir.profundidadeGlobal << "," << resultadoInsercao.first << endl;
                    }
                }
            }
        }   
        if (OP == "BUS=") {
            int numTuples = dir.busca(stoi(YYYY));

            outTxt << "BUS:" << YYYY << "/" << numTuples << endl;
            
        }
        if (OP == "REM") {
            pair<int, int> numTuples = dir.remover_registro(stoi(YYYY));
            outTxt << "REM:" << YYYY << "/" << numTuples.first << "," << dir.profundidadeGlobal << "," << numTuples.second << endl;
            
        }
    }

    outTxt.close();

    return 0;
}