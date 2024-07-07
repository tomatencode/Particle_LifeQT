#ifndef PARTICLE_LIFE_STATE_H
#define PARTICLE_LIFE_STATE_H

#include "particle.h"
#include <vector>
#include <QColor>

class particle_life_state
{
public:
    std::vector<particle> particles;
    std::vector<particle*> particles_x;
    std::vector<particle*> particles_y;
    std::vector<std::vector<double>> force_tb;
    double force_range;
    double stable_dist;
    double friction;
    double dt;
    int size_x;
    int size_y;
    int num_particle_types;
    int num_particles;

    particle_life_state();

    void reset_particles();
    void add_particle(const particle& new_particle);
    void remove_particle(size_t index);
    void reset_force_tb();
};

#endif // PARTICLE_LIFE_STATE_H
