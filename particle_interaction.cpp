#include "particle_interaction.h"

particle_interaction::particle_interaction(QObject *parent) : QObject(parent)
{
    interacting = false;
    mode = 0;
    radius = 80.0;
    strenght = 0.5;
}

void particle_interaction::apply_interaction(particle_life_state *state) {}

bool particle_interaction::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        interacting = true;
        return true; // Return true if the event is handled
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        interacting = false;
        return true; // Return true if the event is handled
    }
    return QObject::eventFilter(obj, event); // Call base class method for default processing
}
