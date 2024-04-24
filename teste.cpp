#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
using namespace std;

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