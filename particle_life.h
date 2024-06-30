#ifndef PARTICLE_LIFE_H
#define PARTICLE_LIFE_H

#include "particle_life_state.h"
#include <QTime>
#include <Eigen/Dense>


class particle_life
{
public:
    particle_life_state state;
    particle_life(particle_life_state state);

    void update();

private:
    void apply_all_forces(int start_num, int end_num);
    double get_force(double dist, int type_p1, int type_p2);
    Eigen::Vector2d get_dist(const particle p1, const particle p2);
    void apply_force(particle& p1, particle& p2);
};


#endif // PARTICLE_LIFE_H
