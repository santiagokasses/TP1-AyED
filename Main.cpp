#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
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

// ==================== MOSTRAR ====================
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
             << setw(15) << tipos[vec[i].Tproducto]
             << setw(12) << vec[i].SKU
             << setw(15) << fixed << setprecision(2) << vec[i].CostoDirecto
             << setw(15) << fixed << setprecision(2) << vec[i].Presupuestado << endl;
    }
}

// ==================== PLANTILLA ORDENAMIENTO ====================
template <typename T> void ordenar(T vec[], int dim) {
    for (int i = 0; i < dim - 1; ++i) {
        for (int j = 0; j < dim - 1; j++) {
            // Comparar por Cliente
            if (strcmp(vec[j].Cliente, vec[j + 1].Cliente) > 0) {
                T tmp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = tmp;
            }
            // Si mismo Cliente, comparar por Tipo Producto
            else if (strcmp(vec[j].Cliente, vec[j + 1].Cliente) == 0) {
                if (vec[j].Tproducto > vec[j + 1].Tproducto) {
                    T tmp = vec[j];
                    vec[j] = vec[j + 1];
                    vec[j + 1] = tmp;
                }
                // Si mismo Cliente y mismo Tipo Producto, comparar por SKU
                else if (vec[j].Tproducto == vec[j + 1].Tproducto) {
                    if (strcmp(vec[j].SKU, vec[j + 1].SKU) > 0) {
                        T tmp = vec[j];
                        vec[j] = vec[j + 1];
                        vec[j + 1] = tmp;
                    }
                }
            }
        }
    }
}

// ==================== PLANTILLA BÚSQUEDA ====================
template <typename T> int buscarLinealOrdenado(T vec[], int dim, const char* cliente) {
    int i;
    for (i = 0; i < dim && strcmp(vec[i].Cliente, cliente) < 0; i++);
    return (i < dim && strcmp(vec[i].Cliente, cliente) == 0) ? i : -1;
}

// ==================== BÚSQUEDA DE CLIENTE ====================
void buscarCliente(Reparaciones reparaciones[], int dimR, Productos productos[], int dimP) {
    char nombre[16];
    cout << "\nIngrese nombre de cliente (EOF para salir):" << endl;
    while (true) {
        cout << "> ";
        cin.getline(nombre, 16);
        
        if (cin.eof()) break;
        if (strlen(nombre) == 0) continue;

        int pos = buscarLinealOrdenado(reparaciones, dimR, nombre);

        if (pos == -1) {
            cout << "No se encontraron reparaciones para el cliente: "
                 << nombre << "\n";
            continue;
        }

        cout << left;
        cout << setw(15) << "Cliente"
             << setw(19) << "Tipo Producto"
             << setw(12) << "SKU"
             << setw(20) << "Producto"
             << setw(15) << "Costo Fijo"
             << setw(15) << "Costo Directo"
             << setw(15) << "Presupuestado"
             << setw(15) << "Ganancia" << endl;

        float totalPresupuestado = 0.0;
        float gananciaTotal = 0.0;

        for (int i = pos; i < dimR && strcmp(reparaciones[i].Cliente, nombre) == 0; i++) {
            string descProd = "<<No encontrado>>";
            float costoFijo = 0.0;
            for (int j = 0; j < dimP; j++) {
                if (strcmp(productos[j].SKU, reparaciones[i].SKU) == 0) {
                    descProd = productos[j].Descripcion;
                    costoFijo = productos[j].CostoFijo;
                    break;
                }
            }

            float ganancia = reparaciones[i].Presupuestado - (costoFijo + reparaciones[i].CostoDirecto);
            totalPresupuestado += reparaciones[i].Presupuestado;
            gananciaTotal += ganancia;

            cout << setw(15) << reparaciones[i].Cliente
                 << setw(20) << tipos[reparaciones[i].Tproducto]
                 << setw(12) << reparaciones[i].SKU
                 << setw(20) << descProd
                 << setw(15) << fixed << setprecision(2) << costoFijo
                 << setw(15) << reparaciones[i].CostoDirecto
                 << setw(15) << reparaciones[i].Presupuestado
                 << setw(15) << ganancia << endl;
        }

        cout << "\nTotal PRESUPUESTADO a " << nombre << ": "
             << fixed << setprecision(2) << totalPresupuestado << endl;
        cout << "Ganancia TOTAL con " << nombre << ": "
             << fixed << setprecision(2) << gananciaTotal << "\n\n";
    }
}

int main() {
    const int dimProductos = 10;
    Productos productos[dimProductos];

    // ==================== LEER PRODUCTOS ====================
    ifstream archiProd("productos.bin", ios::binary);
    if (!archiProd) {
        cout << "Error: no se pudo abrir productos.bin\n";
        return 1;
    }

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
    if (!archiRep) {                             
        cout << "Error: no se pudo abrir reparaciones.bin\n";
        return 1;                                 
    }   
    
    archiRep.seekg(0, ios::end);
    int dimReparaciones = archiRep.tellg() / sizeof(Reparaciones);
    archiRep.seekg(0, ios::beg);
    Reparaciones reparaciones[dimReparaciones];

    for (int i = 0; i < dimReparaciones; i++) {
        archiRep.read(reparaciones[i].Cliente, 15).read(reinterpret_cast<char*>(&reparaciones[i].Tproducto), 4);
        archiRep.read(reparaciones[i].SKU, 10).read(reinterpret_cast<char*>(&reparaciones[i].CostoDirecto), 4);
        archiRep.read(reinterpret_cast<char*>(&reparaciones[i].Presupuestado), 4);
        reparaciones[i].Cliente[15] = '\0';
        reparaciones[i].SKU[10] = '\0';
    }
    archiRep.close();

    // ORDENAR
    ordenar(reparaciones, dimReparaciones);

    // MOSTRAR (PARA COMPROBAR QUE TODO ESTA BIEN)
    //mostrarProductos(productos, dimProductos);
    //cout << "-----------------------------------------------" <<endl;
    //mostrarReparaciones(reparaciones, dimReparaciones);

    // BÚSQUEDA POR CLIENTE
    buscarCliente(reparaciones, dimReparaciones, productos, dimProductos);

    return 0;
}
