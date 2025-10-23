#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;

// CLASE CREDITO:
class Credito {
public:
    string nombre, apellido, codigo, rol;
    Credito() {}
    Credito(string n, string a, string c, string r)
        : nombre(n), apellido(a), codigo(c), rol(r) {}
    void mostrar() {
        cout << "  - " << rol << ": " << nombre << " " << apellido
             << " (" << codigo << ")" << endl;
    }
};

// CLASE ARTISTA:
class Artista {
public:
    string nombre, pais;
    int edad, seguidores, posicion;
    Artista() {}
    Artista(string n, int e, string p, int s, int pos)
        : nombre(n), edad(e), pais(p), seguidores(s), posicion(pos) {}
};

// CLASE ALBUM:
class Album {
public:
    string nombre, codigo, sello, portada, fecha;
    string generos[4];
    int puntuacion;
    Album() {}
    Album(string n, string c, string s, string p, string f, int punt) {
        nombre = n; codigo = c; sello = s; portada = p; fecha = f; puntuacion = punt;
    }
};

// CLASE CANCION:
class Cancion {
