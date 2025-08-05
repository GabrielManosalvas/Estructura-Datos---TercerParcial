#include "Modelo.hpp"
#include <SFML/System.hpp>

Modelo::Modelo() : tamano(3), movimientosJugador1(0), movimientosJugador2(0),
                   jugadorActual(1), juegoTerminado(false), puntajeJugador1(0),
                   puntajeJugador2(0), usandoSugerencia(false), nivelDificultad(1),
                   historialMovimientos(nullptr), ultimoMovimiento(nullptr)
{
    crearMatriz();
}

Modelo::~Modelo()
{
    liberarMatriz(tablero);

    // Limpiar historial
    while (historialMovimientos) {
        Nodo* temp = historialMovimientos;
        historialMovimientos = historialMovimientos->siguiente;
        liberarMatriz(temp->estado);
        delete temp;
    }
}

void Modelo::crearMatriz()
{
    tablero = new int*[tamano];
    for (int i = 0; i < tamano; i++) {
        tablero[i] = new int[tamano];
    }
}

void Modelo::liberarMatriz(int** matriz)
{
    if (matriz) {
        for (int i = 0; i < tamano; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
}

int** Modelo::copiarMatriz(int** matriz)
{
    int** copia = new int*[tamano];
    for (int i = 0; i < tamano; i++) {
        copia[i] = new int[tamano];
        for (int j = 0; j < tamano; j++) {
            copia[i][j] = matriz[i][j];
        }
    }
    return copia;
}

void Modelo::inicializarJuego(int dificultad)
{
    nivelDificultad = dificultad;
    movimientosJugador1 = 0;
    movimientosJugador2 = 0;
    jugadorActual = 1;
    juegoTerminado = false;
    puntajeJugador1 = 0;
    puntajeJugador2 = 0;
    usandoSugerencia = false;

    // Configurar tablero resuelto
    int contador = 1;
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (i == tamano - 1 && j == tamano - 1) {
                tablero[i][j] = 0;
                posicionVacia = Posicion(i, j);
            } else {
                tablero[i][j] = contador++;
            }
        }
    }

    // Mezclar según dificultad
    int movimientosMezcla = 5; // Fácil
    if (dificultad == 2) movimientosMezcla = 15; // Medio
    else if (dificultad == 3) movimientosMezcla = 30; // Difícil

    mezclarTablero(movimientosMezcla);
}

void Modelo::mezclarTablero(int movimientos)
{
    sf::Clock reloj;
    unsigned int semilla = static_cast<unsigned int>(reloj.getElapsedTime().asMicroseconds());

    for (int i = 0; i < movimientos; i++) {
        semilla = semilla * 1103515245 + 12345;
        int direccion = (semilla / 65536) % 4;

        int nuevaX = posicionVacia.x;
        int nuevaY = posicionVacia.y;

        switch (direccion) {
            case 0: nuevaX--; break; // Arriba
            case 1: nuevaX++; break; // Abajo
            case 2: nuevaY--; break; // Izquierda
            case 3: nuevaY++; break; // Derecha
        }

        if (nuevaX >= 0 && nuevaX < tamano && nuevaY >= 0 && nuevaY < tamano) {
            tablero[posicionVacia.x][posicionVacia.y] = tablero[nuevaX][nuevaY];
            tablero[nuevaX][nuevaY] = 0;
            posicionVacia = Posicion(nuevaX, nuevaY);
        }
    }
}

bool Modelo::moverFicha(int x, int y)
{
    if (!esMovimientoValido(x, y) || juegoTerminado) {
        return false;
    }

    // Intercambiar ficha con espacio vacío
    tablero[posicionVacia.x][posicionVacia.y] = tablero[x][y];
    tablero[x][y] = 0;
    posicionVacia = Posicion(x, y);

    // Incrementar contador de movimientos
    if (jugadorActual == 1) {
        movimientosJugador1++;
    } else {
        movimientosJugador2++;
    }

    // Verificar si está resuelto
    if (estaResuelto()) {
        juegoTerminado = true;
        calcularPuntajes();
    }

    return true;
}

bool Modelo::esMovimientoValido(int x, int y)
{
    if (x < 0 || x >= tamano || y < 0 || y >= tamano) {
        return false;
    }

    if (tablero[x][y] == 0) {
        return false;
    }

    // Verificar si está adyacente al espacio vacío
    int difX = x - posicionVacia.x;
    int difY = y - posicionVacia.y;

    if (difX < 0) difX = -difX;
    if (difY < 0) difY = -difY;

    return (difX == 1 && difY == 0) || (difX == 0 && difY == 1);
}

void Modelo::cambiarJugador()
{
    if (!juegoTerminado) {
        jugadorActual = (jugadorActual == 1) ? 2 : 1;
    }
}

bool Modelo::estaResuelto()
{
    int valorEsperado = 1;
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (i == tamano - 1 && j == tamano - 1) {
                return tablero[i][j] == 0;
            }
            if (tablero[i][j] != valorEsperado) {
                return false;
            }
            valorEsperado++;
        }
    }
    return true;
}

void Modelo::calcularPuntajes()
{
    puntajeJugador1 = 1000 - (5 * movimientosJugador1);
    puntajeJugador2 = 1000 - (5 * movimientosJugador2);

    if (puntajeJugador1 < 0) puntajeJugador1 = 0;
    if (puntajeJugador2 < 0) puntajeJugador2 = 0;

    // Bonus por no usar sugerencias
    if (!usandoSugerencia) {
        if (jugadorActual == 1) {
            puntajeJugador1 += 200;
        } else {
            puntajeJugador2 += 200;
        }
    }
}

void Modelo::usarSugerencia()
{
    usandoSugerencia = true;
}

Movimiento Modelo::obtenerSugerencia()
{
    // Implementación simple de sugerencia
    // Buscar movimiento válido hacia la solución

    // Probar cada movimiento posible
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (esMovimientoValido(i, j)) {
                return Movimiento(i, j, posicionVacia.x, posicionVacia.y);
            }
        }
    }

    return Movimiento(-1, -1, -1, -1);
}

bool Modelo::resolverBacktracking(int** estado, int profundidad, int maxProfundidad)
{
    if (profundidad > maxProfundidad) {
        return false;
    }

    // Verificar si está resuelto
    int valorEsperado = 1;
    bool resuelto = true;
    for (int i = 0; i < tamano && resuelto; i++) {
        for (int j = 0; j < tamano && resuelto; j++) {
            if (i == tamano - 1 && j == tamano - 1) {
                if (estado[i][j] != 0) resuelto = false;
            } else {
                if (estado[i][j] != valorEsperado) resuelto = false;
                valorEsperado++;
            }
        }
    }

    if (resuelto) return true;

    // Encontrar posición vacía
    Posicion vacia = encontrarPosicionVacia(estado);

    // Probar movimientos
    int movX[] = {-1, 1, 0, 0};
    int movY[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nuevaX = vacia.x + movX[i];
        int nuevaY = vacia.y + movY[i];

        if (nuevaX >= 0 && nuevaX < tamano && nuevaY >= 0 && nuevaY < tamano) {
            // Hacer movimiento
            estado[vacia.x][vacia.y] = estado[nuevaX][nuevaY];
            estado[nuevaX][nuevaY] = 0;

            if (resolverBacktracking(estado, profundidad + 1, maxProfundidad)) {
                return true;
            }

            // Deshacer movimiento
            estado[nuevaX][nuevaY] = estado[vacia.x][vacia.y];
            estado[vacia.x][vacia.y] = 0;
        }
    }

    return false;
}

bool Modelo::sonEstadosIguales(int** estado1, int** estado2)
{
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (estado1[i][j] != estado2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

Posicion Modelo::encontrarPosicionVacia(int** estado)
{
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (estado[i][j] == 0) {
                return Posicion(i, j);
            }
        }
    }
    return Posicion(-1, -1);
}

// Getters
int Modelo::obtenerValor(int x, int y) const
{
    if (x >= 0 && x < tamano && y >= 0 && y < tamano) {
        return tablero[x][y];
    }
    return -1;
}

Posicion Modelo::obtenerPosicionVacia() const
{
    return posicionVacia;
}

int Modelo::obtenerJugadorActual() const
{
    return jugadorActual;
}

int Modelo::obtenerMovimientosJugador1() const
{
    return movimientosJugador1;
}

int Modelo::obtenerMovimientosJugador2() const
{
    return movimientosJugador2;
}

int Modelo::obtenerPuntajeJugador1() const
{
    return puntajeJugador1;
}

int Modelo::obtenerPuntajeJugador2() const
{
    return puntajeJugador2;
}

bool Modelo::obtenerJuegoTerminado() const
{
    return juegoTerminado;
}

int Modelo::obtenerNivelDificultad() const
{
    return nivelDificultad;
}
