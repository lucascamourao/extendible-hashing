#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "teste.cpp"

using namespace std; 

int main() {
    readCsv teste("compras.csv");
    vector<Compras> c = teste.lerArquivo();

    for(int i=0; i < c.size(); ++i){
        cout<<c[i].pedido<<endl;
        cout<<c[i].ano<<endl;
        cout<<c[i].valor<<endl;
    }

    

    return 0;
}