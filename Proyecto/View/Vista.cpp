#include "Vista.hpp"
#include <SFML/System.hpp>


#include "Vista.hpp"
#include <SFML/System.hpp>

Vista::Vista() : mostrandoMenuDificultad(true), juegoIniciado(false),
                 jugadorActual(1), movimientosJugador1(0), movimientosJugador2(0),
                 puntajeJugador1(0), puntajeJugador2(0), juegoTerminado(false),
                 animandoMovimiento(false), duracionAnimacion(0.3f)
{
    ventana = new sf::RenderWindow(sf::VideoMode({800u, 600u}), "8-Puzzle");
    ventana->setFramerateLimit(60);

    configurarElementosUI();
}

Vista::~Vista()
{
    delete ventana;
}

void Vista::configurarElementosUI()
{
    crearFichas();
    crearBotones();
}

void Vista::crearFichas()
{
    float tamanoFicha = 80.0f;
    float espaciado = 5.0f;
    float inicioX = 350.0f;
    float inicioY = 200.0f;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Configurar rectángulo de la ficha
            fichas[i][j].setSize({tamanoFicha, tamanoFicha});
            fichas[i][j].setPosition({inicioX + j * (tamanoFicha + espaciado),
                                     inicioY + i * (tamanoFicha + espaciado)});
            fichas[i][j].setFillColor(sf::Color::Cyan);
            fichas[i][j].setOutlineThickness(3);
            fichas[i][j].setOutlineColor(sf::Color::Black);

            // Usar círculos de colores para representar números
            numerosVisuales[i][j].setRadius(25.0f);
            numerosVisuales[i][j].setPosition({inicioX + j * (tamanoFicha + espaciado) + 15,
                                              inicioY + i * (tamanoFicha + espaciado) + 15});
            numerosVisuales[i][j].setOutlineThickness(2);
            numerosVisuales[i][j].setOutlineColor(sf::Color::Black);
        }
    }
}

void Vista::crearBotones()
{
    // Botón de sugerencia
    botonSugerencia.setSize({120.0f, 35.0f});
    botonSugerencia.setPosition({50.0f, 450.0f});
    botonSugerencia.setFillColor(sf::Color::Green);
    botonSugerencia.setOutlineThickness(2);
    botonSugerencia.setOutlineColor(sf::Color::Black);

    // Botón de reiniciar
    botonReiniciar.setSize({120.0f, 35.0f});
    botonReiniciar.setPosition({200.0f, 450.0f});
    botonReiniciar.setFillColor(sf::Color::Yellow);
    botonReiniciar.setOutlineThickness(2);
    botonReiniciar.setOutlineColor(sf::Color::Black);

    // Botones de dificultad - usar diferentes colores para identificarlos
    for (int i = 0; i < 3; i++) {
        botonesDificultad[i].setSize({100.0f, 60.0f});
        botonesDificultad[i].setPosition({300.0f + i * 120.0f, 300.0f});

        // Colores diferentes para cada dificultad
        if (i == 0) botonesDificultad[i].setFillColor(sf::Color::Green);      // Fácil
        else if (i == 1) botonesDificultad[i].setFillColor(sf::Color::Blue); // Medio
        else botonesDificultad[i].setFillColor(sf::Color::Red);              // Difícil

        botonesDificultad[i].setOutlineThickness(3);
        botonesDificultad[i].setOutlineColor(sf::Color::Black);
    }
}

sf::Color Vista::obtenerColorNumero(int numero)
{
    // Asignar colores únicos a cada número
    switch (numero) {
        case 1: return sf::Color::Red;
        case 2: return sf::Color::Blue;
        case 3: return sf::Color::Green;
        case 4: return sf::Color::Yellow;
        case 5: return sf::Color::Magenta;
        case 6: return sf::Color::Cyan;
        case 7: return sf::Color(255, 165, 0); // Naranja
        case 8: return sf::Color(128, 0, 128); // Púrpura
        default: return sf::Color::Transparent;
    }
}

bool Vista::estaAbierta()
{
    return ventana->isOpen();
}

void Vista::cerrar()
{
    ventana->close();
}

std::optional<sf::Event> Vista::pollEvent()
{
    if (auto evento = ventana->pollEvent()) {
        return evento;
    }
    return std::nullopt;
}

void Vista::manejarEventos(sf::Event& evento)
{
    if (evento.is<sf::Event::Closed>()) {
        cerrar();
    }
}

void Vista::actualizar(const Modelo& modelo)
{
    if (!juegoIniciado) {
        return;
    }

    // Guardar estado del modelo para usar en renderizar()
    jugadorActual = modelo.obtenerJugadorActual();
    movimientosJugador1 = modelo.obtenerMovimientosJugador1();
    movimientosJugador2 = modelo.obtenerMovimientosJugador2();
    puntajeJugador1 = modelo.obtenerPuntajeJugador1();
    puntajeJugador2 = modelo.obtenerPuntajeJugador2();
    juegoTerminado = modelo.obtenerJuegoTerminado();

    // Actualizar estado del tablero
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int valor = modelo.obtenerValor(i, j);
            if (valor == 0) {
                // Espacio vacío - casilla gris oscuro sin círculo
                fichas[i][j].setFillColor(sf::Color(80, 80, 80));
                numerosVisuales[i][j].setFillColor(sf::Color::Transparent);
                numerosVisuales[i][j].setOutlineColor(sf::Color::Transparent);
            } else {
                // Ficha con número - casilla cian con círculo de color
                fichas[i][j].setFillColor(sf::Color::Cyan);
                numerosVisuales[i][j].setFillColor(obtenerColorNumero(valor));
                numerosVisuales[i][j].setOutlineColor(sf::Color::Black);
            }
        }
    }

    // Actualizar animación
    if (animandoMovimiento) {
        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        if (tiempoTranscurrido >= duracionAnimacion) {
            animandoMovimiento = false;
        }
    }
}

void Vista::renderizar()
{
    ventana->clear(sf::Color::White);

    if (mostrandoMenuDificultad) {
        // Renderizar menú de dificultad
        // Título visual
        sf::RectangleShape titulo;
        titulo.setSize({400.0f, 80.0f});
        titulo.setPosition({200.0f, 100.0f});
        titulo.setFillColor(sf::Color(200, 200, 255));
        titulo.setOutlineThickness(3);
        titulo.setOutlineColor(sf::Color::Black);
        ventana->draw(titulo);

        // Instrucciones
        sf::RectangleShape instruccion;
        instruccion.setSize({400.0f, 40.0f});
        instruccion.setPosition({200.0f, 200.0f});
        instruccion.setFillColor(sf::Color(255, 255, 200));
        instruccion.setOutlineThickness(2);
        instruccion.setOutlineColor(sf::Color::Black);
        ventana->draw(instruccion);

        // Botones de dificultad
        for (int i = 0; i < 3; i++) {
            ventana->draw(botonesDificultad[i]);

            // Pequeños indicadores numéricos
            sf::CircleShape numero(10.0f);
            numero.setPosition({340.0f + i * 120.0f, 320.0f});
            numero.setFillColor(sf::Color::White);
            numero.setOutlineThickness(2);
            numero.setOutlineColor(sf::Color::Black);
            ventana->draw(numero);
        }

    } else if (juegoIniciado) {
        // Renderizar juego
        // Dibujar tablero
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ventana->draw(fichas[i][j]);
                // Solo dibujar círculo si no es transparente
                if (numerosVisuales[i][j].getFillColor() != sf::Color::Transparent) {
                    ventana->draw(numerosVisuales[i][j]);
                }
            }
        }

        // Panel de información mejorado
        sf::RectangleShape infoPanel;
        infoPanel.setSize({250.0f, 150.0f});
        infoPanel.setPosition({30.0f, 30.0f});
        infoPanel.setFillColor(sf::Color(240, 240, 240));
        infoPanel.setOutlineThickness(2);
        infoPanel.setOutlineColor(sf::Color::Black);
        ventana->draw(infoPanel);

        // Indicador de jugador actual más visible
        sf::CircleShape indicadorJugador(20.0f);
        indicadorJugador.setPosition({50.0f, 50.0f});
        if (jugadorActual == 1) {
            indicadorJugador.setFillColor(sf::Color::Red);
        } else {
            indicadorJugador.setFillColor(sf::Color::Blue);
        }
        indicadorJugador.setOutlineThickness(3);
        indicadorJugador.setOutlineColor(sf::Color::Black);
        ventana->draw(indicadorJugador);

        // Etiquetas para jugadores
        sf::RectangleShape etiquetaJ1({30.0f, 15.0f});
        etiquetaJ1.setPosition({100.0f, 60.0f});
        etiquetaJ1.setFillColor(sf::Color::Red);
        ventana->draw(etiquetaJ1);

        sf::RectangleShape etiquetaJ2({30.0f, 15.0f});
        etiquetaJ2.setPosition({140.0f, 60.0f});
        etiquetaJ2.setFillColor(sf::Color::Blue);
        ventana->draw(etiquetaJ2);

        // Contadores de movimientos mejorados
        float maxAlto = 80.0f;
        float factorEscala = 3.0f;

        float altoBarraJ1 = (movimientosJugador1 * factorEscala > maxAlto) ? maxAlto : movimientosJugador1 * factorEscala;
        float altoBarraJ2 = (movimientosJugador2 * factorEscala > maxAlto) ? maxAlto : movimientosJugador2 * factorEscala;

        sf::RectangleShape barraJ1({25.0f, altoBarraJ1});
        barraJ1.setPosition({105.0f, 160.0f - altoBarraJ1});
        barraJ1.setFillColor(sf::Color::Red);
        barraJ1.setOutlineThickness(1);
        barraJ1.setOutlineColor(sf::Color::Black);
        ventana->draw(barraJ1);

        sf::RectangleShape barraJ2({25.0f, altoBarraJ2});
        barraJ2.setPosition({140.0f, 160.0f - altoBarraJ2});
        barraJ2.setFillColor(sf::Color::Blue);
        barraJ2.setOutlineThickness(1);
        barraJ2.setOutlineColor(sf::Color::Black);
        ventana->draw(barraJ2);

        // Dibujar botones
        ventana->draw(botonSugerencia);
        ventana->draw(botonReiniciar);

        // Indicador de victoria mejorado
        if (juegoTerminado) {
            sf::RectangleShape panelVictoria;
            panelVictoria.setSize({300.0f, 100.0f});
            panelVictoria.setPosition({250.0f, 50.0f});
            panelVictoria.setFillColor(sf::Color::Yellow);
            panelVictoria.setOutlineThickness(5);
            panelVictoria.setOutlineColor(sf::Color::Red);
            ventana->draw(panelVictoria);

            // Mostrar ganador con color más grande
            sf::CircleShape ganador(35.0f);
            ganador.setPosition({380.0f, 75.0f});
            ganador.setOutlineThickness(4);
            ganador.setOutlineColor(sf::Color::Black);

            if (puntajeJugador1 > puntajeJugador2) {
                ganador.setFillColor(sf::Color::Red);  // Jugador 1
            } else if (puntajeJugador2 > puntajeJugador1) {
                ganador.setFillColor(sf::Color::Blue); // Jugador 2
            } else {
                ganador.setFillColor(sf::Color::Green); // Empate
            }
            ventana->draw(ganador);
        }
    }

    ventana->display();
}

void Vista::iniciarAnimacionMovimiento(int fichaX, int fichaY, int vaciaX, int vaciaY)
{
    animandoMovimiento = true;
    relojAnimacion.restart();
    posicionAnimacionInicio = Posicion(fichaX, fichaY);
    posicionAnimacionFin = Posicion(vaciaX, vaciaY);
    fichaAnimada = fichaX * 3 + fichaY;
}

sf::Vector2f Vista::interpolarPosicion(sf::Vector2f inicio, sf::Vector2f fin, float progreso)
{
    return sf::Vector2f(
        inicio.x + (fin.x - inicio.x) * progreso,
        inicio.y + (fin.y - inicio.y) * progreso
    );
}

bool Vista::clicEnFicha(int x, int y, sf::Vector2i posicionMouse)
{
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return false;
    }

    sf::FloatRect limiteFicha = fichas[x][y].getGlobalBounds();
    return limiteFicha.contains(sf::Vector2f(posicionMouse));
}

bool Vista::clicEnBotonSugerencia(sf::Vector2i posicionMouse)
{
    sf::FloatRect limiteBoton = botonSugerencia.getGlobalBounds();
    return limiteBoton.contains(sf::Vector2f(posicionMouse));
}

bool Vista::clicEnBotonReiniciar(sf::Vector2i posicionMouse)
{
    sf::FloatRect limiteBoton = botonReiniciar.getGlobalBounds();
    return limiteBoton.contains(sf::Vector2f(posicionMouse));
}

int Vista::clicEnBotonDificultad(sf::Vector2i posicionMouse)
{
    for (int i = 0; i < 3; i++) {
        sf::FloatRect limiteBoton = botonesDificultad[i].getGlobalBounds();
        if (limiteBoton.contains(sf::Vector2f(posicionMouse))) {
            return i + 1; // Retorna 1, 2, o 3
        }
    }
    return 0; // No se hizo clic en ningún botón
}

bool Vista::obtenerMostrandoMenuDificultad() const
{
    return mostrandoMenuDificultad;
}

bool Vista::obtenerJuegoIniciado() const
{
    return juegoIniciado;
}

void Vista::establecerJuegoIniciado(bool estado)
{
    juegoIniciado = estado;
    if (estado) {
        mostrandoMenuDificultad = false;
    }
}
