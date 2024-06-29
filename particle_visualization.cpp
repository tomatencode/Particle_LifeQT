#include "particle_visualization.h"
#include "qscreen.h"
#include <vector>
#include <random>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>

particle_visualization::particle_visualization(QWidget *parent) : QWidget(parent) {

    t_last  = QTime::currentTime();
    paused = false;

    // particle life default parameters
    int num_particle_types = 6;
    std::vector<std::vector<double>>  force_tb = get_random_force_tb(num_particle_types);


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    size_y = screenGeometry.height();
    size_x = screenGeometry.width();

    std::vector<particle> particles = {};

    double dt = 0.08;
    double friction = 0.982;
    double force_range = 100.0;
    double stable_dist = 0.3;

    this->particle_life_ptr = new class particle_life(particles, force_tb, force_range, stable_dist, dt, friction, size_x, size_y);
    reset_particles(800, num_particle_types); // spawns 800 random particles

    // visualization parameters
    particle_visualization::colormap = {
        QColor(255,0,0),
        QColor(0,255,0),
        QColor(0,0,255),
        QColor(255,0,255),
        QColor(0,255,255),
        QColor(255,255,0)
    };

    // setting up a timer to call update each tick
    particle_visualization::tps = 0;
    particle_visualization::timer = new QTimer(this);
    particle_visualization::connect(timer, &QTimer::timeout, this, &particle_visualization::update_visualization);
    particle_visualization::timer->start(0);
}

particle_visualization::~particle_visualization()
{
    delete particle_life_ptr; // Clean up dynamically allocated memory
    delete timer; // Clean up timer
}

void particle_visualization::update_visualization()
{

    if (not paused)
    {
        particle_life_ptr->update(); // update particle life if not paused
    }

    update(); // Request repaint after update

    // measure tps (ticks per second)
    tps = 1000/t_last.msecsTo(QTime::currentTime());
    t_last = QTime::currentTime();
}

void particle_visualization::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw background
    painter.setBrush(QBrush(QColor(10,10,10)));
    painter.drawRect(this->rect());
    painter.setBrush(QBrush(QColor(25,25,25)));
    painter.drawRect(QRect(0,0,size_x,size_y));

    // draw particles
    for (particle &particle : particle_life_ptr->particles)
    {
        painter.setBrush(QBrush(colormap[particle.type]));
        double pos_x = particle.position[0];
        double pos_y = particle.position[1];
        painter.drawEllipse(QPointF(pos_x,pos_y), 2, 2);
    }
}

void particle_visualization::take_screenshot(double Quality)
{
    QPixmap pixmap(size_x * Quality, size_y* Quality);
    pixmap.fill(QColor(10,10,10)); // Fill the pixmap with white color

    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw particles
    for (particle &particle : particle_life_ptr->particles)
    {
        painter.setBrush(QBrush(colormap[particle.type]));
        double pos_x = particle.position[0] * Quality;
        double pos_y = particle.position[1] * Quality;
        painter.drawEllipse(QPointF(pos_x,pos_y), 2 * Quality, 2 * Quality);
    }

    // Save the pixmap to a file
    // Get the Pictures directory path
    QString picturesDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);


    // Define the new folder name
    QString newFolderName = "ParticleLife_Screenshots";
    QDir dir(picturesDir);

    // Create the new folder if it doesn't exist
    if (!dir.exists(newFolderName)) {
        if (!dir.mkdir(newFolderName)) {
            qDebug() << "Failed to create the directory" << newFolderName;
        }
    }

    // Set the new folder path
    dir.cd(newFolderName);

    // Generate a unique filename
    QString baseFileName = "ParticleLife_Screenshot";
    QString fileExtension = ".png";
    QString fileName;
    int counter = 1;
    do {
        fileName = QString("%1%2%3").arg(baseFileName).arg(counter).arg(fileExtension);
        counter++;
    } while (dir.exists(fileName));

    // Construct the full file path
    QString filePath = dir.filePath(fileName);

    if (not pixmap.save(filePath)) {
        qDebug() << "Failed to save the image to " << filePath;
    }
}

std::vector<particle> particle_visualization::get_random_particles(int const num, int const num_types)
{
    // innitilise the random number generators
    std::uniform_real_distribution<double> rand_x(0,size_x);
    std::uniform_real_distribution<double> rand_y(0,size_y);
    std::uniform_int_distribution<> rand_type(0, num_types-1);
    std::random_device rd;

    // spawn particles
    std::vector<particle> particles;
    for (int i = 0; i < num; i++) {
        particles.push_back(particle({rand_x(rd),rand_y(rd)},rand_type(rd)));
    }

    return particles;
}

std::vector<std::vector<double>> particle_visualization::get_random_force_tb(int size)
{
    // innitilise the random number generators
    std::uniform_real_distribution<double> rand(-1,1);
    std::random_device rd;

    // spawn particles
    std::vector<std::vector<double>> force_tb;
    force_tb.resize(size, std::vector<double>(size, 0.0));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
        force_tb[i][j] = rand(rd);
        }
    }

    return force_tb;
}


// slots
void particle_visualization::reset_particles(int const num, int const num_types)
{
    particle_life_ptr->particles = get_random_particles(num, num_types);
}
