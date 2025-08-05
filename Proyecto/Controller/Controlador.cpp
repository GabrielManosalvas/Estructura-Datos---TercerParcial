#include "Controlador.hpp"

Controlador::Controlador()
{
    modelo = new Modelo();
    vista = new Vista();
}

Controlador::~Controlador()
{
    delete modelo;
    delete vista;
}

void Controlador::ejecutar()
{
    while (vista->estaAbierta()) {
        // Procesar eventos
        while (auto evento = vista->pollEvent()) {
            vista->manejarEventos(*evento);
            manejarEventosMouse(*evento);
        }

        // Actualizar y renderizar
        vista->actualizar(*modelo);
        vista->renderizar();

        // Breve pausa para evitar uso excesivo de CPU
        sf::sleep(sf::milliseconds(16)); // ~60 FPS
    }
}

void Controlador::manejarEventosMouse(sf::Event& evento)
{
    if (const auto* mouseEvent = evento.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i posicionMouse(mouseEvent->position.x, mouseEvent->position.y);

            if (vista->obtenerMostrandoMenuDificultad()) {
                manejarClicBotonDificultad(posicionMouse);
            } else if (vista->obtenerJuegoIniciado()) {
                // Verificar clics en botones primero
                if (vista->clicEnBotonSugerencia(posicionMouse)) {
                    manejarClicBotonSugerencia();
                } else if (vista->clicEnBotonReiniciar(posicionMouse)) {
                    manejarClicBotonReiniciar();
                } else {
                    // Si no es un botón, verificar clics en fichas
                    manejarClicFicha(posicionMouse);
                }
            }
        }
    }
}

void Controlador::manejarClicFicha(sf::Vector2i posicionMouse)
{
    if (modelo->obtenerJuegoTerminado()) {
        return;
    }

    // Buscar en qué ficha se hizo clic
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (vista->clicEnFicha(i, j, posicionMouse)) {
                // Intentar mover la ficha
                if (modelo->moverFicha(i, j)) {
                    // Iniciar animación
                    Posicion posVacia = modelo->obtenerPosicionVacia();
                    vista->iniciarAnimacionMovimiento(i, j, posVacia.x, posVacia.y);

                    // Cambiar turno si el juego no ha terminado
                    if (!modelo->obtenerJuegoTerminado()) {
                        modelo->cambiarJugador();
                    }
                }
                return;
            }
        }
    }
}

void Controlador::manejarClicBotonSugerencia()
{
    if (!modelo->obtenerJuegoTerminado()) {
        modelo->usarSugerencia();
        mostrarSugerencia();
    }
}

void Controlador::manejarClicBotonReiniciar()
{
    int dificultadActual = modelo->obtenerNivelDificultad();
    modelo->inicializarJuego(dificultadActual);
}

void Controlador::manejarClicBotonDificultad(sf::Vector2i posicionMouse)
{
    int dificultad = vista->clicEnBotonDificultad(posicionMouse);
    if (dificultad > 0) {
        modelo->inicializarJuego(dificultad);
        vista->establecerJuegoIniciado(true);
    }
}

void Controlador::mostrarSugerencia()
{
    Movimiento sugerencia = modelo->obtenerSugerencia();

    if (sugerencia.deX >= 0 && sugerencia.deY >= 0) {
        // Resaltar la ficha sugerida por un momento
        // Esto se podría implementar cambiando temporalmente el color de la ficha
        // Por simplicidad, aquí solo mostramos que se puede mover esa ficha

        // En una implementación más completa, se podría:
        // 1. Cambiar el color de la ficha sugerida
        // 2. Mostrar una flecha indicando la dirección
        // 3. Usar un timer para volver al color original
    }
}
