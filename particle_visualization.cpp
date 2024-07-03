#include "particle_visualization.h"
#include "qscreen.h"
#include <vector>
#include "particle_life_state.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>

particle_visualization::particle_visualization(QWidget *parent, particle_life_state *state, particle_interaction *interaction)
    : QWidget(parent)
{
    this->state = state;
    this->interaction = interaction;
    // visualization parameters
    particle_visualization::colormap = {
        QColor(255,0,0),
        QColor(0,255,0),
        QColor(0,0,255),
        QColor(255,0,255),
        QColor(0,255,255),
        QColor(255,255,0)
    };
}

particle_visualization::~particle_visualization()
{}

void particle_visualization::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    draw_particles_on_painter(painter, 1.0);
    draw_interation_circle_on_painter(painter);
}

void particle_visualization::take_screenshot(double Quality)
{
    // creates pixmap of current frame
    QPixmap pixmap(state->size_x * Quality, state->size_y * Quality);

    QPainter painter(&pixmap);

    draw_particles_on_painter(painter, Quality);


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

    // save Image
    if (not pixmap.save(filePath)) {
        qDebug() << "Failed to save the image to " << filePath;
    }
}

void particle_visualization::draw_particles_on_painter(QPainter &painter, double Quality)
{
    // set painter propertys
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw background
    painter.setBrush(QBrush(QColor(10, 10, 10)));
    painter.drawRect(0, 0, state->size_x * Quality, state->size_y * Quality);
    painter.setBrush(QBrush(QColor(25, 25, 25)));
    painter.drawRect(0, 0, state->size_x * Quality, state->size_y * Quality);

    // draw particles
    for (particle &particle : state->particles)
    {
        painter.setBrush(QBrush(colormap[particle.type]));
        painter.drawEllipse(QPointF(particle.position[0] * Quality, particle.position[1] * Quality), 2 * Quality, 2 * Quality);
    }
}


void particle_visualization::draw_interation_circle_on_painter(QPainter &painter)
{
    bool show = false;
    QColor color;
    int radius = interaction->radius;

    switch (interaction->mode) {
    case 0:
        show = false;
        break;
    case 1:
        show = true;
        if (interaction->interacting) {
            color = QColor(10,200,10,120);
        }
        else {
            color = QColor(20,100,20,120);
        }
        break;
    case 2:
        show = true;
        if (interaction->interacting) {
            color = QColor(240,10,10,120);
        }
        else {
            color = QColor(150,20,20,120);
        }
        break;
    default:
        break;
    }

    if (show) {
        painter.setBrush(Qt::NoBrush);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(color,2));

        QScreen *screen = QApplication::screens().at(0);
        QPoint pos_mouse = QCursor::pos(screen);

        painter.drawEllipse(pos_mouse, radius, radius);
    }
}
