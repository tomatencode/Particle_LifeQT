#include "mainwindow.h"
#include "particle_life.h"
#include "particle_visualization.h"
#include "settings.h"
#include "particle_interaction.h"
#include <QPushButton>

MainWindow::MainWindow() {

    // particle life default parameters
    state.dt = 0.08;
    state.force_range = 80;
    state.friction = 0.98;
    state.stable_dist = 0.3;
    state.num_particle_types = 5;

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    state.size_x = screenGeometry.width();
    state.size_y = screenGeometry.height();

    state.reset_force_tb();
    state.reset_particles(800);

    interaction = new particle_interaction(this);
    this->installEventFilter(interaction);

    visualization = new particle_visualization(this, &state, interaction);

    settings = new Settings(this, interaction, visualization, &state);
    settings->move(50,50);  // Set the size and position of the settings widget

    connect(settings, &Settings::togglePause, this, &MainWindow::handleTogglePause);

    add_close_button();

    paused = false;

    // setting up a timer to call update each tick
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(0);

    setCentralWidget(visualization);
    show();
}

void MainWindow::update()
{
    if (not paused)
    {
        interaction->apply_interaction(&state);
        visualization->update(); // Request repaint after update
        particle_life::update(state); // update particle state
    }
    settings->update_lables();
}

void MainWindow::handleTogglePause(bool paused) {
    this->paused = paused;
}

void MainWindow::add_close_button()
{
    close_button = new QPushButton(this);

    // Set the icon (you can replace the path with your own icon path)
    QIcon closeIcon(":/icons/close_icon.png");
    close_button->setIcon(closeIcon);

    // Make the button transparent except for the icon
    close_button->setFlat(true);  // Removes the button border and background
    close_button->setStyleSheet(
        "QPushButton:pressed {"
            "background-color: rgba(255, 0, 0, 0);"
        "}"
        );

    // Set the button size
    int buttonSize = 30; // Adjust size as needed
    close_button->setIconSize(QSize(0.8*buttonSize, 0.8*buttonSize));
    close_button->setFixedSize(buttonSize, buttonSize);

    close_button->move(width() - close_button->width() - 5, 5);

    // Connect the button's clicked signal to the close slot
    connect(close_button, &QPushButton::clicked, this, &QMainWindow::close);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    close_button->move(width() - close_button->width() - 5, 5);
}
