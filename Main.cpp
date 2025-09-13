#include <iostream>
#include <fstream>

using namespace std;

struct Productos{
    string SKU[11];
    string Descripcion[21];
    float CostoFijo; 
};
struct Reparaciones{
    string Cliente[16];
    int Tproducto;
    string SKU[11];
    float CostoDirecto;
    float Presupuestado;
};

string tipos[3] = {"Electrónico", "Mecánico", "Mecatrónico"};

void mostrar(Productos vec[], int dim) {
    cout << "SKU\t\tCosto Fijo\t\tDescripción" << endl;
    for (int i = 0; i < dim; i++) {
        cout << vec[i].SKU << "\t\t"
             << vec[i].CostoFijo << "\t\t"
             << vec[i].Descripcion << endl;
    }
}


int main(){
    int dim = 10;
    Productos productos[dim];
    Reparaciones reparaciones; 
    	ifstream archilec;
	archilec.open("productos.bin", ios::binary);
	archilec.read(reinterpret_cast<char*>(&productos[0]), sizeof(Productos));
	archilec.close();
    mostrar(productos, dim);
	return 0;
}