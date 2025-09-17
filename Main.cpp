#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct Productos{
    char SKU[11];
    char Descripcion[21];
    float CostoFijo; 
};
struct Reparaciones{
    char Cliente[16];
    int Tproducto;
    char SKU[11];
    float CostoDirecto;
    float Presupuestado;
};

string tipos[3] = {"Electrónico", "Mecánico", "Mecatrónico"};

void mostrarProductos(Productos vec[], int dim) {
    cout << left;
    cout << setw(15) << "SKU"
         << setw(20) << "Descripcion"
         << setw(15) << "Costo Fijo" << endl;
    for (int i = 0; i < dim; i++){
        cout << setw(15) << vec[i].SKU 
             << setw(20) << vec[i].Descripcion 
             << setw(15) << vec[i].CostoFijo << endl;
    }
    
}

void mostrarReparaciones(Reparaciones vec[], int dim) {
    cout << left;
    cout << setw(20) << "Cliente" 
         << setw(15) << "Tipo Producto" 
         << setw(12) << "SKU" 
         << setw(15) << "Costo Directo" 
         << setw(15) << "Presupuestado" << endl;
    for (int i = 0; i < dim; i++) {
        cout << setw(20) << vec[i].Cliente 
             << setw(15) << vec[i].Tproducto 
             << setw(12) << vec[i].SKU 
             << setw(15) << fixed << setprecision(2) << vec[i].CostoDirecto
             << setw(15) << fixed << setprecision(2) << vec[i].Presupuestado << endl;
    }
}
int main() {
    int dimProductos = 10;
    Productos productos[dimProductos];

    // ==================== LEER PRODUCTOS ====================
    ifstream archiProd("productos.bin", ios::binary);
    for (int i = 0; i < dimProductos; i++) {
        archiProd.read(productos[i].SKU, 10);           
        productos[i].SKU[10] = '\0';                    
        archiProd.read(productos[i].Descripcion, 20);   
        productos[i].Descripcion[20] = '\0';            
        archiProd.read(reinterpret_cast<char*>(&productos[i].CostoFijo), 4); 
    }
    archiProd.close();
    // ==================== LEER REPARACIONES ====================
    ifstream archiRep("reparaciones.bin", ios::binary);
    archiRep.seekg(0, ios::end);
    Reparaciones reparaciones[archiRep.tellg() / sizeof(Reparaciones)];
    archiRep.seekg(0, ios::beg);
    int dimReparaciones = sizeof(reparaciones)/sizeof(Reparaciones);
    
    for (int i = 0; i < dimReparaciones; i++) {
        archiRep.read(reparaciones[i].Cliente, 15).read(reinterpret_cast<char*>(&reparaciones[i].Tproducto), 4);
        archiRep.read(reparaciones[i].SKU, 10).read(reinterpret_cast<char*>(&reparaciones[i].CostoDirecto), 4);
        archiRep.read(reinterpret_cast<char*>(&reparaciones[i].Presupuestado), 4);
        reparaciones[i].Cliente[15] = reparaciones[i].SKU[10] = '\0';
    }
    archiRep.close();
    mostrarProductos(productos, dimProductos);
    cout << "-----------------------------------------------" <<endl;
    mostrarReparaciones(reparaciones, dimReparaciones);
    return 0;
}