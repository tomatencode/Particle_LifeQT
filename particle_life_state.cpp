#include "particle_life_state.h"
#include <random>

particle_life_state::particle_life_state()
    : force_range(0), stable_dist(0), friction(0), dt(0), size_x(0), size_y(0), num_particle_types(0)
{}


void particle_life_state::reset_particles(int num) {

    if (num == -1) {
        num = particles.size();
    }

    // innitilise the random number generators
    std::uniform_real_distribution<double> rand_x(0,size_x);
    std::uniform_real_distribution<double> rand_y(0,size_y);
    std::uniform_int_distribution<> rand_type(0, num_particle_types-1);
    std::random_device rd;

    // spawn particles
    particles.clear();
    for (int i = 0; i < num; i++) {
        particles.push_back(particle({rand_x(rd),rand_y(rd)},rand_type(rd)));
    }
}

void particle_life_state::reset_force_tb()
{
    // innitilise the random number generators
    std::uniform_real_distribution<double> rand(-1,1);
    std::random_device rd;

    // spawn particles
    force_tb.resize(num_particle_types, std::vector<double>(num_particle_types, 0.0));
    for (int i = 0; i < num_particle_types; i++) {
        for (int j = 0; j < num_particle_types; j++) {
            force_tb[i][j] = rand(rd);
        }
    }
}

