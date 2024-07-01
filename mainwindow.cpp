#include "mainwindow.h"
#include "particle_life.h"
#include "qboxlayout.h"
#include "settings.h"
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

    visualization = new particle_visualization(this, state);
    setCentralWidget(visualization);

    settings = new Settings(this, visualization, state);
    settings->move(50,50);  // Set the size and position of the settings widget

    connect(settings, &Settings::togglePause, this, &MainWindow::handleTogglePause);

    add_close_button();

    paused = false;

    // setting up a timer to call update each tick
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update);
    timer->start(0);

    show();
}

void MainWindow::update()
{
    if (not paused)
    {
        visualization->update(); // Request repaint after update
        particle_life::update(state); // update particle life if not paused
        settings->update_lables();
    }
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
    close_button->setStyleSheet("background: transparent;");

    // Set the button size
    int buttonSize = 30; // Adjust size as needed
    close_button->setFixedSize(buttonSize, buttonSize);

    // Create a layout for the top row
    QWidget *topWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);

    // Add a spacer to push the button to the right
    topLayout->addStretch();
    topLayout->addWidget(close_button);

    // Remove margins to ensure the button is at the very top-right corner
    topLayout->setContentsMargins(0, 0, 0, 0);

    // Create a main layout for the entire window
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Add the top row layout and other widgets
    mainLayout->addWidget(topWidget);
    // Add other widgets to mainLayout as needed
    setCentralWidget(centralWidget);

    // Connect the button's clicked signal to the close slot
    connect(close_button, &QPushButton::clicked, this, &QMainWindow::close);
}
