#ifndef PARTICLE_LIFE_H
#define PARTICLE_LIFE_H

#include "particle_life_state.h"
#include <QTime>
#include <Eigen/Dense>


namespace particle_life {

    void update(particle_life_state &state);
    void apply_all_forces(particle_life_state &state, int start_num, int end_num);
    double get_force(particle_life_state &state, double dist, int type_p1, int type_p2);
    Eigen::Vector2d get_dist(particle_life_state &state, const particle p1, const particle p2);
    void apply_force(particle_life_state &state, particle& p1, particle& p2);
    void update_all_partclies(particle_life_state &state);
    void auto_dt(particle_life_state &state);
}


#endif // PARTICLE_LIFE_H
