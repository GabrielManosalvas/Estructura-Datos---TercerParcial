#ifndef VISTA_HPP
#define VISTA_HPP

#include <SFML/Graphics.hpp>
#include "../Model/Modelo.hpp"


class Vista
{
private:
    sf::RenderWindow* ventana;
    sf::RectangleShape fichas[3][3];
    sf::CircleShape numerosVisuales[3][3];
    sf::RectangleShape botonSugerencia;
    sf::RectangleShape botonReiniciar;
    sf::RectangleShape botonesDificultad[3];

    bool mostrandoMenuDificultad;
    bool juegoIniciado;

    // Estado del juego para renderizar
    int jugadorActual;
    int movimientosJugador1;
    int movimientosJugador2;
    int puntajeJugador1;
    int puntajeJugador2;
    bool juegoTerminado;

    // Animación
    sf::Clock relojAnimacion;
    bool animandoMovimiento;
    Posicion posicionAnimacionInicio;
    Posicion posicionAnimacionFin;
    int fichaAnimada;
    float duracionAnimacion;

    void configurarElementosUI();
    void crearFichas();
    void crearBotones();
    sf::Vector2f interpolarPosicion(sf::Vector2f inicio, sf::Vector2f fin, float progreso);
    sf::Color obtenerColorNumero(int numero);

public:
    Vista();
    ~Vista();

    bool estaAbierta();
    void cerrar();
    std::optional<sf::Event> pollEvent();
    void manejarEventos(sf::Event& evento);
    void actualizar(const Modelo& modelo);
    void renderizar();
    void iniciarAnimacionMovimiento(int fichaX, int fichaY, int vaciaX, int vaciaY);

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
