#ifndef PARTICLE_INTERACTION_H
#define PARTICLE_INTERACTION_H

#include "particle_life_state.h"
#include <QObject>
#include <QMouseEvent>
#include <QDebug>

class particle_interaction : public QObject {
    Q_OBJECT
public:
    bool interacting;
    int mode;
    double radius;
    double strenght;

    particle_interaction(QObject *parent = nullptr);


    void apply_interaction(particle_life_state *sate);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // PARTICLE_INTERACTION_H
