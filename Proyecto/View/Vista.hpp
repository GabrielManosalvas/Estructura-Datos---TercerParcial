#ifndef VISTA_HPP
#define VISTA_HPP

#include <SFML/Graphics.hpp>
#include "../Model/Modelo.hpp"

class Vista
{
private:
    sf::RenderWindow* ventana;
    sf::Font fuente;
    bool fuenteCargada;

    // Elementos visuales del tablero
    sf::RectangleShape fichas[3][3];
    sf::Text* numerosTexto[3][3];

    // Botones
    sf::RectangleShape botonSugerencia;
    sf::RectangleShape botonReiniciar;
    sf::RectangleShape botonesDificultad[3];

    // Textos - usando punteros para evitar problemas de inicialización
    sf::Text* textoTitulo;
    sf::Text* textoSubtitulo;
    sf::Text* textoTurno;
    sf::Text* textoInfoJ1;
    sf::Text* textoInfoJ2;
    sf::Text* textoInstrucciones;
    sf::Text* textoPuntaje;
    sf::Text* textoNivel;
    sf::Text* textoBotonSugerencia;
    sf::Text* textoBotonReiniciar;
    sf::Text* textosDificultad[3];

    // Estados
    bool mostrandoMenuDificultad;
    bool juegoIniciado;

    // Estado del juego para renderizar
    int jugadorActual;
    int movimientosJugador1;
    int movimientosJugador2;
    int puntajeJugador1;
    int puntajeJugador2;
    bool juegoTerminado;

    // Animación de movimiento
    sf::Clock relojAnimacion;
    bool animandoMovimiento;
    Posicion posicionAnimacionInicio;
    Posicion posicionAnimacionFin;
    int fichaAnimada;
    float duracionAnimacion;

    // Sugerencia visual
    sf::RectangleShape resaltadoSugerencia;
    bool mostrandoSugerencia;
    Posicion fichaSugerida;
    sf::Clock relojSugerencia;

    // Hover
    Posicion fichaResaltada;

    // Métodos privados
    void configurarElementosUI();
    void crearFichas();
    void crearBotones();
    void crearTextos();
    sf::Vector2f interpolarPosicion(sf::Vector2f inicio, sf::Vector2f fin, float progreso);
    sf::String intToString(int valor);

public:
    Vista();
    ~Vista();

    bool estaAbierta();
    void cerrar();
    std::optional<sf::Event> pollEvent();
    void manejarEventos(sf::Event& evento);
    void actualizar(const Modelo& modelo);
    void renderizar();

    // Animaciones
    void iniciarAnimacionMovimiento(int fichaX, int fichaY, int vaciaX, int vaciaY);
    void mostrarSugerencia(int x, int y);

    // Getters para eventos
    bool clicEnFicha(int x, int y, sf::Vector2i posicionMouse);
    bool clicEnBotonSugerencia(sf::Vector2i posicionMouse);
    bool clicEnBotonReiniciar(sf::Vector2i posicionMouse);
    int clicEnBotonDificultad(sf::Vector2i posicionMouse);

    bool obtenerMostrandoMenuDificultad() const;
    bool obtenerJuegoIniciado() const;
    void establecerJuegoIniciado(bool estado);
};

#endif
