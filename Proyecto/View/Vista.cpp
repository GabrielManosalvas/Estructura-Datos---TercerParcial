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
    float espaciado = 10.0f;
    float inicioX = 300.0f;
    float inicioY = 150.0f;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Configurar rectángulo de la ficha
            fichas[i][j].setSize({tamanoFicha, tamanoFicha});
            fichas[i][j].setPosition({inicioX + j * (tamanoFicha + espaciado),
                                     inicioY + i * (tamanoFicha + espaciado)});
            fichas[i][j].setFillColor(sf::Color::Cyan);
            fichas[i][j].setOutlineThickness(2);
            fichas[i][j].setOutlineColor(sf::Color::Black);

            // Usar círculos de colores para representar números
            numerosVisuales[i][j].setRadius(20.0f);
            numerosVisuales[i][j].setPosition({inicioX + j * (tamanoFicha + espaciado) + 20,
                                              inicioY + i * (tamanoFicha + espaciado) + 20});
            numerosVisuales[i][j].setOutlineThickness(2);
            numerosVisuales[i][j].setOutlineColor(sf::Color::Black);
        }
    }
}

void Vista::crearBotones()
{
    // Botón de sugerencia
    botonSugerencia.setSize({150.0f, 40.0f});
    botonSugerencia.setPosition({50.0f, 400.0f});
    botonSugerencia.setFillColor(sf::Color::Green);
    botonSugerencia.setOutlineThickness(2);
    botonSugerencia.setOutlineColor(sf::Color::Black);

    // Botón de reiniciar
    botonReiniciar.setSize({150.0f, 40.0f});
    botonReiniciar.setPosition({220.0f, 400.0f});
    botonReiniciar.setFillColor(sf::Color::Yellow);
    botonReiniciar.setOutlineThickness(2);
    botonReiniciar.setOutlineColor(sf::Color::Black);

    // Botones de dificultad - usar diferentes colores para identificarlos
    for (int i = 0; i < 3; i++) {
        botonesDificultad[i].setSize({120.0f, 50.0f});
        botonesDificultad[i].setPosition({250.0f + i * 140.0f, 250.0f});

        // Colores diferentes para cada dificultad
        if (i == 0) botonesDificultad[i].setFillColor(sf::Color::Green);      // Fácil
        else if (i == 1) botonesDificultad[i].setFillColor(sf::Color::Blue); // Medio
        else botonesDificultad[i].setFillColor(sf::Color::Red);              // Difícil

        botonesDificultad[i].setOutlineThickness(3);
        botonesDificultad[i].setOutlineColor(sf::Color::White);
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
                fichas[i][j].setFillColor(sf::Color(50, 50, 50)); // Gris oscuro para espacio vacío
                numerosVisuales[i][j].setFillColor(sf::Color::Transparent);
            } else {
                fichas[i][j].setFillColor(sf::Color::Cyan);
                numerosVisuales[i][j].setFillColor(obtenerColorNumero(valor));
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
        // Mostrar instrucciones visuales
        sf::RectangleShape instruccion;
        instruccion.setSize({400.0f, 60.0f});
        instruccion.setPosition({200.0f, 150.0f});
        instruccion.setFillColor(sf::Color::White);
        instruccion.setOutlineThickness(3);
        instruccion.setOutlineColor(sf::Color::Black);
        ventana->draw(instruccion);

        for (int i = 0; i < 3; i++) {
            ventana->draw(botonesDificultad[i]);
        }

        // Leyenda visual simple
        sf::RectangleShape leyenda;
        leyenda.setSize({400.0f, 100.0f});
        leyenda.setPosition({200.0f, 350.0f});
        leyenda.setFillColor(sf::Color::White);
        leyenda.setOutlineThickness(2);
        leyenda.setOutlineColor(sf::Color::Black);
        ventana->draw(leyenda);

        // Pequeños indicadores de dificultad
        for (int i = 0; i < 3; i++) {
            sf::CircleShape indicador(10.0f);
            indicador.setPosition({220.0f + i * 60.0f, 370.0f});
            if (i == 0) indicador.setFillColor(sf::Color::Green);      // Fácil
            else if (i == 1) indicador.setFillColor(sf::Color::Blue); // Medio
            else indicador.setFillColor(sf::Color::Red);              // Difícil
            ventana->draw(indicador);
        }
    } else if (juegoIniciado) {
        // Renderizar juego
        // Dibujar tablero
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ventana->draw(fichas[i][j]);
                ventana->draw(numerosVisuales[i][j]);
            }
        }

        // Mostrar información del juego con elementos visuales
        sf::RectangleShape infoPanel;
        infoPanel.setSize({200.0f, 150.0f});
        infoPanel.setPosition({30.0f, 30.0f});
        infoPanel.setFillColor(sf::Color(240, 240, 240));
        infoPanel.setOutlineThickness(2);
        infoPanel.setOutlineColor(sf::Color::Black);
        ventana->draw(infoPanel);

        // Indicador de jugador actual
        sf::CircleShape indicadorJugador(15.0f);
        indicadorJugador.setPosition({50.0f, 50.0f});
        if (jugadorActual == 1) {
            indicadorJugador.setFillColor(sf::Color::Red);
        } else {
            indicadorJugador.setFillColor(sf::Color::Blue);
        }
        ventana->draw(indicadorJugador);

        // Mostrar contadores de movimientos con barras
        float altoBarraJ1 = movimientosJugador1 * 2.0f;
        float altoBarraJ2 = movimientosJugador2 * 2.0f;

        sf::RectangleShape barraJ1({20.0f, altoBarraJ1});
        barraJ1.setPosition({80.0f, 150.0f - altoBarraJ1});
        barraJ1.setFillColor(sf::Color::Red);
        ventana->draw(barraJ1);

        sf::RectangleShape barraJ2({20.0f, altoBarraJ2});
        barraJ2.setPosition({110.0f, 150.0f - altoBarraJ2});
        barraJ2.setFillColor(sf::Color::Blue);
        ventana->draw(barraJ2);

        // Dibujar botones
        ventana->draw(botonSugerencia);
        ventana->draw(botonReiniciar);

        // Indicador de victoria
        if (juegoTerminado) {
            sf::RectangleShape panelVictoria;
            panelVictoria.setSize({300.0f, 100.0f});
            panelVictoria.setPosition({250.0f, 50.0f});
            panelVictoria.setFillColor(sf::Color::Yellow);
            panelVictoria.setOutlineThickness(3);
            panelVictoria.setOutlineColor(sf::Color::Red);
            ventana->draw(panelVictoria);

            // Mostrar ganador con color
            sf::CircleShape ganador(30.0f);
            ganador.setPosition({380.0f, 80.0f});

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
