#ifndef CONTROLADOR_HPP
#define CONTROLADOR_HPP

#include "../Model/Modelo.hpp"
#include "../View/Vista.hpp"
#include <SFML/Graphics.hpp>

class Controlador
{
private:
    Modelo* modelo;
    Vista* vista;

    void manejarEventosMouse(sf::Event& evento);
    void manejarClicFicha(sf::Vector2i posicionMouse);
    void manejarClicBotonSugerencia();
    void manejarClicBotonReiniciar();
    void manejarClicBotonDificultad(sf::Vector2i posicionMouse);
    void mostrarSugerencia();

public:
    Controlador();
    ~Controlador();

    void ejecutar();
};

#endif
