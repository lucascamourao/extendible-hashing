#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;


struct compras{

    int pedido;// vai ser a chave  primaria
    int valor;
    int ano;

    compras(int pedidu,int valorr,int anoo){
        pedido=pedidu;
        valor=valorr;
        ano=anoo;
    }
    void mostrar(){
        cout<<"pedido: "<<pedido<<endl;
        cout<<"valor: "<<valor<<endl;
        cout<<"ano: "<<ano<<endl;
    }
};


struct Bucket {
    string arquivo;        // Nome do arquivo associado ao bucket
    int profundidadeLocal;      // Profundidade local do bucket
    int capacidadeMaxima;      // Capacidade máxima do bucket

    // Construtor    
    Bucket(const string& nomearquivo){
        arquivo=nomearquivo;
        profundidadeLocal=0;
        capacidadeMaxima=3;
    }

    vector<compras> v;
    // Método para adicionar um registro ao bucket
    void adicionarRegistro(int registro) {
        ofstream file(arquivo, ios::app);
        if (file.is_open()) {
            file << registro << "\n";
            file.close();
        } else {
            cout << "Erro ao abrir o arquivo " << arquivo << endl;
        }
    }

    // Método para verificar se o bucket está cheio
    bool estaCheio() {
        std::ifstream file(arquivo);
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            int size = file.tellg();
            file.close();
            return size >= capacidadeMaxima;
        } else {
            std::cerr << "Erro ao abrir o arquivo " << arquivo << std::endl;
            return false;
        }
    }

    // Método para verificar se o bucket está vazio
    bool estaVazio() {
        std::ifstream file(arquivo);
        if (file.is_open()) {
            return file.peek() == std::ifstream::traits_type::eof();
        } else {
            std::cerr << "Erro ao abrir o arquivo " << arquivo << std::endl;
            return false;
        }
    }

    // Método para imprimir os registros do bucket
    void imprimirRegistros() {
        std::ifstream file(arquivo);
        if (file.is_open()) {
            std::string registro;
            while (std::getline(file, registro)) {
                std::cout << registro << " ";
            }
            std::cout << std::endl;
            file.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo " << arquivo << std::endl;
        }
    }
};
