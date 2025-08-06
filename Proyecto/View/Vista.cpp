#include "Vista.hpp"
#include <SFML/System.hpp>

Vista::Vista() : mostrandoMenuDificultad(true), juegoIniciado(false),
                 jugadorActual(1), movimientosJugador1(0), movimientosJugador2(0),
                 puntajeJugador1(0), puntajeJugador2(0), juegoTerminado(false),
                 animandoMovimiento(false), duracionAnimacion(0.3f),
                 mostrandoSugerencia(false), fichaResaltada(-1, -1),
                 fichaSugerida(-1, -1), fuenteCargada(false)
{
    ventana = new sf::RenderWindow(sf::VideoMode({900u, 700u}), "8-Puzzle - Rompecabezas Recursivo");
    ventana->setFramerateLimit(60);

    // Intentar cargar fuente del sistema
    fuenteCargada = fuente.openFromFile("C:/Windows/Fonts/arial.ttf") ||
                    fuente.openFromFile("/System/Library/Fonts/Helvetica.ttc") ||
                    fuente.openFromFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");

    // Inicializar punteros de texto
    textoTitulo = nullptr;
    textoSubtitulo = nullptr;
    textoTurno = nullptr;
    textoInfoJ1 = nullptr;
    textoInfoJ2 = nullptr;
    textoInstrucciones = nullptr;
    textoPuntaje = nullptr;
    textoNivel = nullptr;
    textoBotonSugerencia = nullptr;
    textoBotonReiniciar = nullptr;

    for (int i = 0; i < 3; i++) {
        textosDificultad[i] = nullptr;
        for (int j = 0; j < 3; j++) {
            numerosTexto[i][j] = nullptr;
        }
    }

    configurarElementosUI();
}

Vista::~Vista()
{
    // Liberar textos
    delete textoTitulo;
    delete textoSubtitulo;
    delete textoTurno;
    delete textoInfoJ1;
    delete textoInfoJ2;
    delete textoInstrucciones;
    delete textoPuntaje;
    delete textoNivel;
    delete textoBotonSugerencia;
    delete textoBotonReiniciar;

    for (int i = 0; i < 3; i++) {
        delete textosDificultad[i];
        for (int j = 0; j < 3; j++) {
            delete numerosTexto[i][j];
        }
    }

    delete ventana;
}

void Vista::configurarElementosUI()
{
    crearFichas();
    crearBotones();
    crearTextos();
}

void Vista::crearFichas()
{
    float tamanoFicha = 100.0f;
    float espaciado = 5.0f;
    float inicioX = 400.0f;
    float inicioY = 250.0f;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Configurar rectángulo de la ficha
            fichas[i][j].setSize({tamanoFicha, tamanoFicha});
            fichas[i][j].setPosition({inicioX + j * (tamanoFicha + espaciado),
                                     inicioY + i * (tamanoFicha + espaciado)});
            fichas[i][j].setFillColor(sf::Color(220, 220, 255));
            fichas[i][j].setOutlineThickness(3);
            fichas[i][j].setOutlineColor(sf::Color(50, 50, 150));

            // Crear texto para números si la fuente se cargó
            if (fuenteCargada) {
                numerosTexto[i][j] = new sf::Text(fuente, "", 48);
                numerosTexto[i][j]->setFillColor(sf::Color::Black);
                numerosTexto[i][j]->setStyle(sf::Text::Bold);

                float textoX = inicioX + j * (tamanoFicha + espaciado) + tamanoFicha/2 - 15;
                float textoY = inicioY + i * (tamanoFicha + espaciado) + tamanoFicha/2 - 30;
                numerosTexto[i][j]->setPosition({textoX, textoY});
            }
        }
    }

    // Configurar el resaltado de sugerencia
    resaltadoSugerencia.setSize({tamanoFicha + 10, tamanoFicha + 10});
    resaltadoSugerencia.setFillColor(sf::Color::Transparent);
    resaltadoSugerencia.setOutlineThickness(5);
    resaltadoSugerencia.setOutlineColor(sf::Color(255, 215, 0));
}

void Vista::crearBotones()
{
    // Botón de sugerencia
    botonSugerencia.setSize({140.0f, 45.0f});
    botonSugerencia.setPosition({50.0f, 500.0f});
    botonSugerencia.setFillColor(sf::Color(100, 200, 100));
    botonSugerencia.setOutlineThickness(2);
    botonSugerencia.setOutlineColor(sf::Color::Black);

    // Botón de reiniciar
    botonReiniciar.setSize({140.0f, 45.0f});
    botonReiniciar.setPosition({210.0f, 500.0f});
    botonReiniciar.setFillColor(sf::Color(255, 180, 0));
    botonReiniciar.setOutlineThickness(2);
    botonReiniciar.setOutlineColor(sf::Color::Black);

    // Botones de dificultad
    for (int i = 0; i < 3; i++) {
        botonesDificultad[i].setSize({150.0f, 80.0f});
        botonesDificultad[i].setPosition({225.0f + i * 170.0f, 350.0f});

        if (i == 0) botonesDificultad[i].setFillColor(sf::Color(100, 200, 100));
        else if (i == 1) botonesDificultad[i].setFillColor(sf::Color(100, 150, 255));
        else botonesDificultad[i].setFillColor(sf::Color(255, 100, 100));

        botonesDificultad[i].setOutlineThickness(3);
        botonesDificultad[i].setOutlineColor(sf::Color::Black);
    }
}

void Vista::crearTextos()
{
    if (!fuenteCargada) return;

    // Título del juego
    textoTitulo = new sf::Text(fuente, "8-PUZZLE", 48);
    textoTitulo->setFillColor(sf::Color(50, 50, 150));
    textoTitulo->setStyle(sf::Text::Bold);
    textoTitulo->setPosition({350.0f, 30.0f});

    // Subtítulo
    textoSubtitulo = new sf::Text(fuente, "Rompecabezas Recursivo", 24);
    textoSubtitulo->setFillColor(sf::Color(100, 100, 100));
    textoSubtitulo->setPosition({320.0f, 85.0f});

    // Texto de turno
    textoTurno = new sf::Text(fuente, "", 22);
    textoTurno->setFillColor(sf::Color::Black);
    textoTurno->setPosition({50.0f, 200.0f});

    // Textos de información
    textoInfoJ1 = new sf::Text(fuente, "", 18);
    textoInfoJ1->setFillColor(sf::Color(200, 0, 0));
    textoInfoJ1->setPosition({50.0f, 250.0f});

    textoInfoJ2 = new sf::Text(fuente, "", 18);
    textoInfoJ2->setFillColor(sf::Color(0, 0, 200));
    textoInfoJ2->setPosition({50.0f, 280.0f});

    // Instrucciones
    textoInstrucciones = new sf::Text(fuente, "Mueve las fichas para ordenar del 1 al 8", 16);
    textoInstrucciones->setFillColor(sf::Color(80, 80, 80));
    textoInstrucciones->setPosition({380.0f, 600.0f});

    // Puntaje
    textoPuntaje = new sf::Text(fuente, "", 18);
    textoPuntaje->setFillColor(sf::Color::Black);
    textoPuntaje->setPosition({50.0f, 350.0f});

    // Nivel
    textoNivel = new sf::Text(fuente, "", 20);
    textoNivel->setFillColor(sf::Color(100, 100, 100));
    textoNivel->setPosition({400.0f, 200.0f});

    // Textos de botones
    textoBotonSugerencia = new sf::Text(fuente, "Sugerencia", 18);
    textoBotonSugerencia->setFillColor(sf::Color::White);
    textoBotonSugerencia->setPosition({70.0f, 510.0f});

    textoBotonReiniciar = new sf::Text(fuente, "Reiniciar", 18);
    textoBotonReiniciar->setFillColor(sf::Color::White);
    textoBotonReiniciar->setPosition({235.0f, 510.0f});

    // Textos de dificultad
    sf::String nombresDificultad[3] = {"FACIL", "MEDIO", "DIFICIL"};
    for (int i = 0; i < 3; i++) {
        textosDificultad[i] = new sf::Text(fuente, nombresDificultad[i], 24);
        textosDificultad[i]->setFillColor(sf::Color::White);
        textosDificultad[i]->setStyle(sf::Text::Bold);
        textosDificultad[i]->setPosition({250.0f + i * 170.0f, 375.0f});
    }
}

sf::String Vista::intToString(int valor)
{
    if (valor == 0) return "0";

    sf::String resultado = "";
    bool negativo = valor < 0;
    if (negativo) valor = -valor;

    while (valor > 0) {
        char digito = '0' + (valor % 10);
        resultado = digito + resultado;
        valor /= 10;
    }

    if (negativo) resultado = "-" + resultado;
    return resultado;
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
    return ventana->pollEvent();
}

void Vista::manejarEventos(sf::Event& evento)
{
    if (evento.is<sf::Event::Closed>()) {
        cerrar();
    }
}

void Vista::actualizar(const Modelo& modelo)
{
    if (!juegoIniciado) return;

    // Guardar estado del modelo
    jugadorActual = modelo.obtenerJugadorActual();
    movimientosJugador1 = modelo.obtenerMovimientosJugador1();
    movimientosJugador2 = modelo.obtenerMovimientosJugador2();
    puntajeJugador1 = modelo.obtenerPuntajeJugador1();
    puntajeJugador2 = modelo.obtenerPuntajeJugador2();
    juegoTerminado = modelo.obtenerJuegoTerminado();

    // Actualizar textos si la fuente está cargada
    if (fuenteCargada) {
        textoTurno->setString("Turno: Jugador " + intToString(jugadorActual));
        textoInfoJ1->setString("Jugador 1 - Movimientos: " + intToString(movimientosJugador1));
        textoInfoJ2->setString("Jugador 2 - Movimientos: " + intToString(movimientosJugador2));

        sf::String nivelTexto = "Nivel: ";
        int nivel = modelo.obtenerNivelDificultad();
        if (nivel == 1) nivelTexto += "Facil";
        else if (nivel == 2) nivelTexto += "Medio";
        else nivelTexto += "Dificil";
        textoNivel->setString(nivelTexto);

        textoPuntaje->setString("Puntaje estimado:\nJ1: " +
                               intToString(1000 - 5 * movimientosJugador1) +
                               "\nJ2: " +
                               intToString(1000 - 5 * movimientosJugador2));
    }

    // Actualizar tablero
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int valor = modelo.obtenerValor(i, j);
            if (valor == 0) {
                fichas[i][j].setFillColor(sf::Color(100, 100, 100));
                if (fuenteCargada && numerosTexto[i][j]) {
                    numerosTexto[i][j]->setString("");
                }
            } else {
                fichas[i][j].setFillColor(sf::Color(220, 220, 255));
                if (fuenteCargada && numerosTexto[i][j]) {
                    numerosTexto[i][j]->setString(intToString(valor));
                }
            }
        }
    }

    // Actualizar animaciones
    if (animandoMovimiento) {
        float tiempoTranscurrido = relojAnimacion.getElapsedTime().asSeconds();
        if (tiempoTranscurrido >= duracionAnimacion) {
            animandoMovimiento = false;
        }
    }

    if (mostrandoSugerencia) {
        float tiempo = relojSugerencia.getElapsedTime().asSeconds();
        if (tiempo >= 2.0f) {
            mostrandoSugerencia = false;
        }
    }
}

void Vista::renderizar()
{
    ventana->clear(sf::Color(245, 245, 245));

    if (mostrandoMenuDificultad) {
        // Renderizar menú de dificultad
        if (fuenteCargada) {
            ventana->draw(*textoTitulo);
            ventana->draw(*textoSubtitulo);

            sf::Text instruccionMenu(fuente, "Selecciona el nivel de dificultad:", 24);
            instruccionMenu.setFillColor(sf::Color::Black);
            instruccionMenu.setPosition({270.0f, 280.0f});
            ventana->draw(instruccionMenu);
        }

        // Botones de dificultad
        for (int i = 0; i < 3; i++) {
            ventana->draw(botonesDificultad[i]);
            if (fuenteCargada && textosDificultad[i]) {
                ventana->draw(*textosDificultad[i]);
            }

            // Descripción de cada nivel
            if (fuenteCargada) {
                sf::Text descripcion(fuente, "", 14);
                descripcion.setFillColor(sf::Color(100, 100, 100));

                if (i == 0) descripcion.setString("5 movimientos");
                else if (i == 1) descripcion.setString("15 movimientos");
                else descripcion.setString("30 movimientos");

                descripcion.setPosition({245.0f + i * 170.0f, 410.0f});
                ventana->draw(descripcion);
            }
        }

    } else if (juegoIniciado) {
        // Renderizar juego
        if (fuenteCargada) {
            ventana->draw(*textoTitulo);
            ventana->draw(*textoNivel);
            ventana->draw(*textoTurno);
            ventana->draw(*textoInfoJ1);
            ventana->draw(*textoInfoJ2);
            ventana->draw(*textoPuntaje);
            ventana->draw(*textoInstrucciones);
        }

        // Panel de información
        sf::RectangleShape panelInfo;
        panelInfo.setSize({300.0f, 200.0f});
        panelInfo.setPosition({30.0f, 180.0f});
        panelInfo.setFillColor(sf::Color(240, 240, 250));
        panelInfo.setOutlineThickness(2);
        panelInfo.setOutlineColor(sf::Color(150, 150, 200));
        ventana->draw(panelInfo);

        // Indicador visual del jugador actual
        sf::CircleShape indicadorJugador(15.0f);
        indicadorJugador.setPosition({50.0f, 230.0f});
        if (jugadorActual == 1) {
            indicadorJugador.setFillColor(sf::Color::Red);
        } else {
            indicadorJugador.setFillColor(sf::Color::Blue);
        }
        indicadorJugador.setOutlineThickness(2);
        indicadorJugador.setOutlineColor(sf::Color::Black);
        ventana->draw(indicadorJugador);

        // Dibujar tablero
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ventana->draw(fichas[i][j]);
                if (fuenteCargada && numerosTexto[i][j]) {
                    ventana->draw(*numerosTexto[i][j]);
                }
            }
        }

        // Mostrar resaltado de sugerencia
        if (mostrandoSugerencia && fichaSugerida.x >= 0) {
            float tamanoFicha = 100.0f;
            float espaciado = 5.0f;
            float inicioX = 400.0f;
            float inicioY = 250.0f;

            resaltadoSugerencia.setPosition({
                inicioX + fichaSugerida.y * (tamanoFicha + espaciado) - 5,
                inicioY + fichaSugerida.x * (tamanoFicha + espaciado) - 5
            });

            float tiempo = relojSugerencia.getElapsedTime().asSeconds();
            // Efecto parpadeo simple sin usar sin()
            int ciclos = (int)(tiempo * 4); // 4 parpadeos por segundo
            int alpha = (ciclos % 2 == 0) ? 255 : 100; // Alterna entre brillante y tenue
            resaltadoSugerencia.setOutlineColor(sf::Color(255, 215, 0, alpha));
            ventana->draw(resaltadoSugerencia);
        }

        // Dibujar botones
        ventana->draw(botonSugerencia);
        ventana->draw(botonReiniciar);
        if (fuenteCargada) {
            ventana->draw(*textoBotonSugerencia);
            ventana->draw(*textoBotonReiniciar);
        }

        // Panel de victoria
        if (juegoTerminado) {
            sf::RectangleShape panelVictoria;
            panelVictoria.setSize({400.0f, 200.0f});
            panelVictoria.setPosition({250.0f, 150.0f});
            panelVictoria.setFillColor(sf::Color(255, 255, 200, 240));
            panelVictoria.setOutlineThickness(5);
            panelVictoria.setOutlineColor(sf::Color(255, 215, 0));
            ventana->draw(panelVictoria);

            if (fuenteCargada) {
                sf::Text textoVictoria(fuente, "", 32);
                textoVictoria.setStyle(sf::Text::Bold);

                sf::String ganador;
                if (puntajeJugador1 > puntajeJugador2) {
                    ganador = "Jugador 1 Gana!";
                    textoVictoria.setFillColor(sf::Color(200, 0, 0));
                } else if (puntajeJugador2 > puntajeJugador1) {
                    ganador = "Jugador 2 Gana!";
                    textoVictoria.setFillColor(sf::Color(0, 0, 200));
                } else {
                    ganador = "Empate!";
                    textoVictoria.setFillColor(sf::Color(100, 100, 100));
                }

                textoVictoria.setString(ganador);
                textoVictoria.setPosition({330.0f, 180.0f});
                ventana->draw(textoVictoria);

                sf::Text textosPuntajesFinal(fuente, "", 20);
                textosPuntajesFinal.setFillColor(sf::Color::Black);
                textosPuntajesFinal.setString(
                    "Jugador 1: " + intToString(puntajeJugador1) + " puntos\n" +
                    "Jugador 2: " + intToString(puntajeJugador2) + " puntos\n\n" +
                    "Movimientos J1: " + intToString(movimientosJugador1) + "\n" +
                    "Movimientos J2: " + intToString(movimientosJugador2)
                );
                textosPuntajesFinal.setPosition({300.0f, 230.0f});
                ventana->draw(textosPuntajesFinal);
            }
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

void Vista::mostrarSugerencia(int x, int y)
{
    mostrandoSugerencia = true;
    fichaSugerida = Posicion(x, y);
    relojSugerencia.restart();
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
            return i + 1;
        }
    }
    return 0;
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
