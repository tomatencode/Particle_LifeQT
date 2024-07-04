#ifndef PARTICLE_INTERACTION_H
#define PARTICLE_INTERACTION_H

#include "particle_life_state.h"
#include <QObject>
#include <QMouseEvent>
#include <QDebug>

class particle_interaction : public QObject {
    Q_OBJECT
public:
    particle_life_state *state;
    int mode;
    int sub_mode;
    double radius;
    double strenght;

    particle_interaction(QObject *parent = nullptr, particle_life_state *state  = nullptr);
    void draw_interation_circle_on_painter(QPainter &painter);


    void apply_interaction();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // PARTICLE_INTERACTION_H
