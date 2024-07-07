#include "particle_interaction.h"
#include <random>
#include <math.h>
#include <QScrollEvent>
#include "particle_life.h"
#include "particle.h"
#include "qapplication.h"
#include "qpainter.h"

particle_interaction::particle_interaction(QObject *parent, particle_life_state *state) : QObject(parent)
{
    this->state = state;
    mode = 0;
    sub_mode = 0;
    radius = 80.0;
    strenght = 0.5;
}

void particle_interaction::apply_interaction()
{
    if (sub_mode != 0) {

        QScreen *screen = QApplication::screens().at(0);
        QPoint pos_mouse = QCursor::pos(screen);
        double m_x = pos_mouse.x();
        double m_y = pos_mouse.y();

        if (mode == 1) {

            for (particle &p : state->particles) {
                Eigen::Vector2d diff = particle_life::get_dist(*state, particle({m_x,m_y},0), p);
                double dist = sqrt(diff.dot(diff));

                if (dist < radius) {
                    Eigen::Vector2d normal = diff / dist;
                    if (sub_mode == 1) {
                        p.force += normal*(1-dist/radius)*100*strenght*state->dt;
                    }
                    else if (sub_mode == 2) {
                        p.force -= normal*(1-dist/radius)*100*strenght*state->dt;
                    }
                }
            }
        }
        else if (mode == 2) {
            if (sub_mode == 1) {

                std::uniform_real_distribution<double> spawn(0, 1);
                std::random_device rd;

                if (spawn(rd) < strenght) {

                    // innitilise the random number generators
                    std::uniform_real_distribution<double> rand_x(0, state->size_x);
                    std::uniform_real_distribution<double> rand_y(0, state->size_y);
                    std::uniform_int_distribution<> rand_type(0, state->num_particle_types-1);

                    double x;
                    double y;

                    do {
                        x = rand_x(rd);
                        y = rand_y(rd);
                    } while (sqrt(particle_life::get_dist(*state, particle({m_x,m_y},0), particle({x,y},0)).dot(particle_life::get_dist(*state, particle({m_x,m_y},0), particle({x,y},0))))>radius);

                    state->add_particle(particle({x, y},rand_type(rd)));
                }
            }
            else if (sub_mode == 2) {
                std::uniform_real_distribution<double> rm(0, 1);
                std::random_device rd;
                for (int i = 0; i < state->num_particles; i++) {
                    if (sqrt(particle_life::get_dist(*state, particle({m_x,m_y},0), state->particles[i]).dot(particle_life::get_dist(*state, particle({m_x,m_y},0), state->particles[i])))<radius) {
                        if (rm(rd) < pow(strenght/radius*100, 3)+0.05*strenght) {
                            state->remove_particle(i);
                        }
                    }
                }
            }
        }
    }
}


bool particle_interaction::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            sub_mode = 1;
        } else if (mouseEvent->button() == Qt::RightButton) {
            sub_mode = 2;
        }
        return true; // Return true if the event is handled
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        sub_mode = 0;
        return true; // Return true if the event is handled
    }
    else if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        // Process the wheel event here
        if (wheelEvent->angleDelta().y() > 0) {
            radius = fmin(radius + 10.0, fmin(350.0, fmin(state->size_x, state->size_y)));
        }
        else {
            radius = fmax(radius - 10.0, 20.0);
        }
    }
    return QObject::eventFilter(obj, event); // Call base class method for default processing
}

void particle_interaction::draw_interation_circle_on_painter(QPainter &painter)
{
    if (mode != 0) {
        QColor color;

        switch (sub_mode) {
        case 0:
            color = QColor(120,120,120,120);
            break;
        case 1:
            color = QColor(10,200,10,120);
            break;
        case 2:
            color = QColor(240,10,10,120);
            break;
        default:
            break;
        }

        painter.setBrush(Qt::NoBrush);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(color,2));

        QScreen *screen = QApplication::screens().at(0);
        QPoint pos_mouse = QCursor::pos(screen);

        painter.drawEllipse(QPointF(pos_mouse), static_cast<qreal>(radius), static_cast<qreal>(radius));

        painter.drawEllipse(QPointF(pos_mouse.x()+state->size_x,pos_mouse.y()), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x()-state->size_x,pos_mouse.y()), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x(),pos_mouse.y()+state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x(),pos_mouse.y()-state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x()+state->size_x,pos_mouse.y()+state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x()-state->size_x,pos_mouse.y()+state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x()+state->size_x,pos_mouse.y()-state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
        painter.drawEllipse(QPointF(pos_mouse.x()-state->size_x,pos_mouse.y()-state->size_y), static_cast<qreal>(radius), static_cast<qreal>(radius));
    }
}
