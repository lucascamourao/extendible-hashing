#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
using namespace std;

typedef tuple<string, string> OperationYear;

struct Compras{

    int pedido;// vai ser a chave  primaria
    double valor;
    string ano;

    Compras(int pedidu, double valorr, string anoo){
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

struct readTxt{
    string arquivo_txt;

    vector<OperationYear> operation_year; // vector operation:year

    readTxt(const string& nomearquivo){
        arquivo_txt=nomearquivo;
    }

    int PG() { // retorna a profundidade global, de acordo com o in.txt
        ifstream file(arquivo_txt);    
        int PG = 0; 
        
        if (file.is_open()) {
            string line;
            if (getline(file, line)) {
                try {
                    PG = stoi(line); // Converte a linha para inteiro
                } catch (const invalid_argument& e) {
                    cerr << "Erro: Não foi possível converter para inteiro. " << e.what() << endl;
                }
            } else {
                cout << "O arquivo está vazio." << endl;
            }
            file.close();
        } else {
            cout << "Erro: Não foi possível abrir o arquivo." << endl;
        }

        return PG; // 
    }

    vector<OperationYear> vector_OpYear() {
        ifstream file(arquivo_txt);
        string Op;
        string Year;

        if (file.is_open()) { // Verifica se o arquivo foi aberto corretamente
            string line;
        
            while (getline(file, line)) { // Lê a primeira linha do arquivo
                Op = line.substr(0, 2);
                
                if (line.substr(0, 2) == "BUS"){
                    Year = line.substr(5, 7);
                }
                else {
                    Year = line.substr(4, 7);
                }

                OperationYear op_year(Op, Year);
                operation_year.push_back(op_year); // coloca a tupla no vetor de Operações:Ano

            }
            file.close(); // Fecha o arquivo
            return operation_year;

        } else {
            cout << "Erro: Nao foi possivel abrir o arquivo" << endl;
        }

        file.close();   
    }    
};

struct readCsv{
    string arquivo;

    readCsv(const string& nomearquivo){
        arquivo=nomearquivo;
    }
  
   vector<Compras> lerArquivo(){
        ifstream file;
        file.open(arquivo);
        vector<Compras> compra;
        string linha;
        if (file.is_open()){
            while(getline(file,linha)){
                istringstream iss(linha);
                string pedido,valor,ano;
                getline(iss,pedido,',');
                getline(iss,valor,',');
                getline(iss,ano,',');

                int p=stoi(pedido);
                double v=stod(valor);
               

                compra.push_back(Compras(p,v,ano));

            }
            file.close();
            return compra;
        }else{
            cout << "Erro: Nao foi possivel abrir o arquivo" << endl;
        }
    }

};

struct Directory{
    int profundidadeGlobal;
    readTxt Txt;
    vector<Bucket*> buckets;
    int leastSigBits;
    
    Directory() : Txt("in.txt") { // Inicialize Txt no construtor
        profundidadeGlobal = Txt.PG(); // Certifique-se de que Txt esteja inicializado
    }

    //leastSigBits = pegar o log PG base 2

    ~Directory() {
        // Limpeza de memória dos ponteiros Bucket se necessário
        for (auto& bucket_ptr : buckets) {
            delete bucket_ptr;
        }
    }

    int funcaoHash(int ano, int profundidade) const {
        return ano & ((1 << profundidade) - 1);
    }

    bool doubleDir(Bucket bucket) {
        int profundidadeLocal = bucket.profundidadeLocal;
       

        if (profundidadeGlobal == profundidadeLocal){
            profundidadeGlobal *= 2;
            buckets.resize(buckets.size()*2);
            // DUPLICA

            return true; 
        } else {
            return false; // não duplicou o diretório
        }

    }
    
};

struct Bucket {
    string arquivo;
    vector<Compras> compras;
    int profundidadeLocal;      // Profundidade local do bucket
    int capacidadeMaxima;  // Capacidade máxima do bucket

    // Construtor    

    Bucket(const string& nomearquivo){
        ofstream arquivo(nomearquivo);
    }

    Bucket(int profundidadeglobal){
        profundidadeLocal=profundidadeglobal;
        capacidadeMaxima=3;
    }

    ~Bucket(){
        delete &compras;
    }

    bool isEmpty() {
        return true;
    }

    // profundidade local = profundidade global: tem que duplicar o diretório
    bool isFull(){
        if(compras.size()==capacidadeMaxima){
            return true;
        }
        return false;
    }

    void adicionar_registro(int pedido,double valor,string ano){
        if(isFull()){
            //split
        }
        compras.push_back(Compras(pedido,valor,ano));
        
        
    }

};