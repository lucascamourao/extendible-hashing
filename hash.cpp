#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <cmath>
#include <algorithm> 

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
        this->arquivo_txt = nomearquivo;
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
    int tamanhoDir; // 2**PG = Capacidade global
    readTxt Txt;
    vector<Bucket*> buckets;
   

    // Directory() : Txt("in.txt"), Csv("Compras.csv")
    Directory() : Txt("in.txt"){ // Inicializando o Txt no construtor
        
        profundidadeGlobal = Txt.PG(); 
        
        tamanhoDir = pow(2, profundidadeGlobal);
        buckets.resize(tamanhoDir);

        for (int i = 0; i < tamanhoDir; i++) { // criando os buckets vazios
            string nomearquivo="bucket" +to_string(i)+ ".txt";
            buckets[i] = new Bucket(nomearquivo, profundidadeGlobal);
        }

    }

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
            profundidadeGlobal++;
            buckets.resize(buckets.size()*2);

            // aponta para os mesmos buckets inicialmente
            for (int i = 0; i < buckets.size(); i++) {
                buckets[buckets.size() + i] = buckets[i]; 
            }
            return true;
        } else {
            return false; // não duplicou o diretório
        }

    }

    void splitBucket(int index){
        Bucket* bucket = this->buckets[index]; 
        vector<Compras> antigo = bucket->compras;
        bucket->compras.clear(); 
        
        int PL = bucket->profundidadeLocal;
        int newPL = bucket->profundidadeLocal += 1;
        bucket->profundidadeLocal++; 
        
        string nomearquivo= "bucket" + to_string(buckets.size()) + ".txt";

        Bucket* newBucket = new Bucket(nomearquivo, newPL); 
        this->buckets.push_back(newBucket);

        for(const auto& compra : antigo){
            int ano = stoi(compra.ano);
            int newIndex = funcaoHash(ano, newPL);

            if (newIndex == index) {
                bucket->adicionar_registro(compra.pedido,compra.valor,compra.ano);
            } else {
                newBucket->adicionar_registro(compra.pedido,compra.valor,compra.ano); 
            }
        }

        bucket->salvar();
        newBucket->salvar();
    }

    // FINALIZADO
    pair<int, int> adicionar(const Compras& compra){
        int index = funcaoHash(stoi(compra.ano), this->profundidadeGlobal);
        Bucket* bucket = buckets[index];
        bucket->carregar();
        
        while (doubleDir(*bucket)){
            doubleDir(*bucket); // enquanto precisar, duplica o diretório
        }
        
        while (bucket->isFull()) { // enquanto o bucket estiver cheio, dar split
            splitBucket(index);
        }

        bucket->adicionar_registro(compra.pedido, compra.valor, compra.ano);
        bucket->salvar();

        return {this->profundidadeGlobal, bucket->profundidadeLocal}; 
    }

    // FINALIZADO 
    int busca(int Year){
        int index = funcaoHash(Year, this->profundidadeGlobal);
        Bucket* bucket = buckets[index];
         
        int numTuples = count_if(bucket->compras.begin(), bucket->compras.end(), [Year](const Compras& compra) {
            return stoi(compra.ano) == Year;
        });
        
        return numTuples;
    }

    // FINALIZADO
    pair<int, int> remover_registro(int Year){
        int index = funcaoHash(Year, this->profundidadeGlobal);
        Bucket* bucket = buckets[index];

        bucket->carregar();

        int tuplas_iniciais=bucket->compras.size();
        
        bucket->compras.erase( remove_if(bucket->compras.begin(), bucket->compras.end(), [Year](const Compras& compra) {
            return stoi(compra.ano) == Year;
        })); 

        int tuplas_final=bucket->compras.size();
        int tuplas_removidas = abs(tuplas_iniciais - tuplas_final);
        bucket->salvar();

        return {tuplas_removidas, bucket->profundidadeLocal};
    }

};

struct Bucket {
    string arquivo;
    vector<Compras> compras;
    int profundidadeLocal;      // Profundidade local do bucket
    int capacidadeMaxima;  // Capacidade máxima do bucket

    // Construtor    

    Bucket(const string& nomearquivo, int profundidade){
        ofstream arquivo(nomearquivo);
        arquivo.close();
        profundidadeLocal = profundidade;
        capacidadeMaxima=3;
    }

    ~Bucket(){
        delete &compras;
    }

    void carregar(){
        readCsv ler(arquivo);
        compras=ler.lerArquivo();
    }

    void salvar(){
        ofstream file(arquivo);
        for(const auto& compra:compras){
            file<<compra.pedido << "," << compra.valor << "," << compra.ano << endl;
        }
    }

    // profundidade local = profundidade global: tem que duplicar o diretório
    bool isFull(){
        if(compras.size() == capacidadeMaxima){
            return true;
        }
        return false;
    }

    bool adicionar_registro(int pedido, double valor, string ano){
        if(isFull()){
            return false;
        }
        compras.push_back(Compras(pedido,valor,ano));
        return true;
    }
    
};