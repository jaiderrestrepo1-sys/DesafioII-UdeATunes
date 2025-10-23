#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;

const int MAX_SONGS = 50;
const int MAX_ADS = 50;
const int MAX_USERS = 50;
const int MAX_FAVORITES = 10000;
const int M_PREVIOUS = 6;      
const int K_AUTOPLAY = 5;       

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
private:
    string nombre;
    string id;
    int duracion;
    string ruta128, ruta320;
    Artista artista;
    Album album;
    Credito creditos[10];
    int totalCreditos;
    int reproducciones;

public:
    Cancion() { totalCreditos = 0; reproducciones = 0; }
    Cancion(string _nombre, string _id, int _duracion,
            string _ruta128, string _ruta320, Artista _artista, Album _album)
        : nombre(_nombre), id(_id), duracion(_duracion),
        ruta128(_ruta128), ruta320(_ruta320),
        artista(_artista), album(_album),
        totalCreditos(0), reproducciones(0) {}

    string getNombre() const { return nombre; }
    string getId() const { return id; }
    void agregarCredito(const Credito &c) {
        if (totalCreditos < 10) creditos[totalCreditos++] = c;
    }
    void mostrar(int calidad) {
        reproducciones++;
        cout << "\n--------------------------------------------------" << endl;
        cout << "Cantante: " << artista.nombre << endl;
        cout << "Album: " << album.nombre << endl;
        cout << "Ruta a la portada: " << album.portada << endl;
        cout << "Titulo: " << nombre << endl;
        cout << "Ruta al archivo de audio: " << ((calidad == 128) ? ruta128 : ruta320) << endl;
        cout << "Duracion: " << duracion << " segundos" << endl;
        cout << "\n--- Creditos ---" << endl;
        for (int i = 0; i < totalCreditos; ++i) creditos[i].mostrar();
        cout << "--------------------------------------------------" << endl;
    }

    bool equalsId(const string &otherId) const { return id == otherId; }
};

// CLASE ALBUM:
class Publicidad {
private:
    string texto;
    string categoria; 
public:
    Publicidad() {}
    Publicidad(string _texto, string _categoria) : texto(_texto), categoria(_categoria) {}
    void mostrar() {
        cout << "\n--- PUBLICIDAD ---" << endl;
        cout << texto << " (categoria " << categoria << ")" << endl;
        cout << "--------------------" << endl;
    }
    string getCategoria() const { return categoria; }
    int getPeso() const {
        if (categoria == "AAA") return 3;
        if (categoria == "B") return 2;
        return 1; 
    }
};

// CLASE USUARIO:
class Usuario {
protected:
    string nickname, tipo, ciudad, pais, contrasena, fechaRegistro;
public:
    virtual ~Usuario() {}
    Usuario() {}
    Usuario(string _nick, string _tipo, string _ciudad, string _pais, string _contra)
        : nickname(_nick), tipo(_tipo), ciudad(_ciudad), pais(_pais), contrasena(_contra) {
        time_t t = time(0);
        fechaRegistro = ctime(&t);
        if (!fechaRegistro.empty() && fechaRegistro.back() == '\n') fechaRegistro.pop_back();
    }

    string getNick() const { return nickname; }
    string getContra() const { return contrasena; }
    string getTipo() const { return tipo; }
 void mostrarDatos() const {
        cout << "\nUsuario: " << nickname
             << "\nPais: " << pais
             << "\nCiudad: " << ciudad
             << "\nTipo: " << tipo
             << "\nRegistro: " << fechaRegistro << endl;
    }

    string getLineaArchivo() const {
        return nickname + "," + tipo + "," + ciudad + "," + pais + "," + contrasena + "," + fechaRegistro;
    }
};

// CLASE LISTA FAVORITOS:
class ListaFavoritos {
private:
    Cancion* canciones[MAX_FAVORITES];
    int total;
public:
    ListaFavoritos() : total(0) {}

    bool existePorPtr(Cancion* c) const {
        for (int i = 0; i < total; ++i) if (canciones[i] == c) return true;
        return false;
    }

    bool existePorId(const string &id) const {
        for (int i = 0; i < total; ++i) if (canciones[i]->getId() == id) return true;
        return false;
    }
    bool agregar(Cancion* c) {
        if (c == nullptr) return false;
        if (total >= MAX_FAVORITES) {
            cout << " Lista de favoritos llena." << endl;
            return false;
        }
        if (existePorPtr(c)) {
            cout << " La cancion ya esta en favoritos." << endl;
            return false;
        }
        canciones[total++] = c;
        cout << " Agregada a favoritos: " << c->getNombre() << endl;
        return true;
    }
    bool quitarPorNombre(const string &nombre) {
        for (int i = 0; i < total; ++i) {
            if (canciones[i]->getNombre() == nombre) {
                for (int j = i; j < total - 1; ++j) canciones[j] = canciones[j + 1];
                --total;
                cout << " Quitada de favoritos: " << nombre << endl;
                return true;
            }
        }
        cout << " No se encontro la cancion en favoritos." << endl;
        return false;
    }

    
    void añadirLista(ListaFavoritos &otro) {
        for (int i = 0; i < otro.total && total < MAX_FAVORITES; ++i) {
            if (!existePorPtr(otro.canciones[i])) canciones[total++] = otro.canciones[i];
        }
        cout << " Se agregaron canciones de la lista seguida (si las hay)." << endl;
    }

    void mostrar() const {
        if (total == 0) {
            cout << "No hay canciones en favoritos." << endl;
            return;
        }
        cout << "\n Lista de Favoritos" << endl;
        for (int i = 0; i < total; ++i) cout << i + 1 << ". " << canciones[i]->getNombre() << " (id: " << canciones[i]->getId() << ")" << endl;
    }
};
// CLASE USUARIO PREMIUM:
class UsuarioPremium : public Usuario {
private:
    ListaFavoritos favoritos;
public:
    UsuarioPremium(string _nick, string _ciudad, string _pais, string _contra)
        : Usuario(_nick, "Premium", _ciudad, _pais, _contra) {}
    ListaFavoritos& getFavoritos() { return favoritos; }
};
//CLASE REPRODUCTOR:
class Reproductor {
private:
    Cancion* canciones[MAX_SONGS];
    Publicidad* anuncios[MAX_ADS];
    int totalC, totalP;
public:
    Reproductor() : totalC(0), totalP(0) {}

    bool agregarCancion(Cancion* c) {
        if (totalC >= MAX_SONGS) return false;
        canciones[totalC++] = c;
        return true;
    }
    bool agregarPublicidad(Publicidad* p) {
        if (totalP >= MAX_ADS) return false;
        anuncios[totalP++] = p;
        return true;
    }

    void mostrarCanciones() const {
        cout << "\n Canciones disponibles:" << endl;
        for (int i = 0; i < totalC; ++i) cout << i + 1 << ". " << canciones[i]->getNombre() << " (id: " << canciones[i]->getId() << ")" << endl;
    }



