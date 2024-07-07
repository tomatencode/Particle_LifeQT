#include "particle.h"

particle::particle(Eigen::Vector2d position, int type) {
    this->position = position;
    this->velocety = {0.0,0.0};
    this->force = {0.0, 0.0};
    this->type = type;
    pos_x_list = 0;
    pos_y_list = 0;
}

void particle::update(double dt, double friction, int max_x, int max_y)
{
    position += velocety * dt;
    // [m]   =   [m/s]  *  [s]

    velocety = velocety * friction + force * dt;
    // [m/s]  = [m/s]   *   []   +   [m/s²]*[s]

    force = {0.0, 0.0};

    if (position[0]> max_x) {
        position[0] = position[0] - max_x;
    }
    else if (position[0] < 0) {
        position[0] = position[0] + max_x;
    }
    if (position[1]> max_y) {
        position[1] = position[1] - max_y;
    }
    else if (position[1] < 0) {
        position[1] = position[1] + max_y;
    }
}

