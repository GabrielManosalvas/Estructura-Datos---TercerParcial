#ifndef MODELO_HPP
#define MODELO_HPP

struct Posicion {
    int x, y;
    Posicion(int x = 0, int y = 0) : x(x), y(y) {}
};

struct Movimiento {
    int deX, deY, aX, aY;
    Movimiento(int dx, int dy, int ax, int ay) : deX(dx), deY(dy), aX(ax), aY(ay) {}
};

class Modelo
{
private:
    int** tablero;
    int tamano;
    Posicion posicionVacia;
    int movimientosJugador1;
    int movimientosJugador2;
    int jugadorActual;
    bool juegoTerminado;
    int puntajeJugador1;
    int puntajeJugador2;
    bool usandoSugerencia;
    int nivelDificultad;

    struct Nodo {
        int** estado;
        Nodo* siguiente;
        Nodo* anterior;

        Nodo() : estado(nullptr), siguiente(nullptr), anterior(nullptr) {}
    };

    Nodo* historialMovimientos;
    Nodo* ultimoMovimiento;

    void crearMatriz();
    void liberarMatriz(int** matriz);
    int** copiarMatriz(int** matriz);
    bool esEstadoValido(int** estado);
    int calcularInversiones(int** matriz);
    void mezclarTablero(int movimientos);

public:
    Modelo();
    ~Modelo();

    void inicializarJuego(int dificultad);
    bool moverFicha(int x, int y);
    bool esMovimientoValido(int x, int y);
    void cambiarJugador();
    bool estaResuelto();
    void calcularPuntajes();
    void usarSugerencia();
    Movimiento obtenerSugerencia();

    // Getters
    int obtenerValor(int x, int y) const;
    Posicion obtenerPosicionVacia() const;
    int obtenerJugadorActual() const;
    int obtenerMovimientosJugador1() const;
    int obtenerMovimientosJugador2() const;
    int obtenerPuntajeJugador1() const;
    int obtenerPuntajeJugador2() const;
    bool obtenerJuegoTerminado() const;
    int obtenerNivelDificultad() const;

    // Backtracking para sugerencias
    bool resolverBacktracking(int** estado, int profundidad, int maxProfundidad);
    bool sonEstadosIguales(int** estado1, int** estado2);
    Posicion encontrarPosicionVacia(int** estado);
};

#endif
