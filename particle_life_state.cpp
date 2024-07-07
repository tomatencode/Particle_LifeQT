#include "particle_life_state.h"
#include <random>

particle_life_state::particle_life_state()
    : force_range(0), stable_dist(0), friction(0), dt(0), size_x(0), size_y(0), num_particle_types(0), num_particles(0)
{}


void particle_life_state::reset_particles() {

    // innitilise the random number generators
    std::uniform_real_distribution<double> rand_x(0,size_x);
    std::uniform_real_distribution<double> rand_y(0,size_y);
    std::uniform_int_distribution<> rand_type(0, num_particle_types-1);
    std::random_device rd;
    std::mt19937 gen(rd());

    // Reserve space to prevent reallocation
    particles.reserve(num_particles*2);
    particles_x.reserve(num_particles*2);
    particles_y.reserve(num_particles*2);

    particles.clear();
    particles_x.clear();
    particles_y.clear();

    // spawn particles
    for (int i = 0; i < num_particles; i++) {
        particles.push_back(particle({rand_x(gen),rand_y(gen)},rand_type(gen)));
        particles_x.push_back(&particles.back());
        particles_y.push_back(&particles.back());
    }
}

void particle_life_state::add_particle(const particle& new_particle) {
    // Check if there is enough reserved space
    if (particles.size() < particles.capacity()) {
        // Add the new particle
        particles.push_back(new_particle);
        particles_x.push_back(&particles.back());
        particles_y.push_back(&particles.back());
        num_particles++;
    }
}

void particle_life_state::remove_particle(size_t index) {
    // Remove the particle from the particles vector
    particle* removed_particle = &particles[index];

    // Erase the particle from particles_x and particles_y
    auto remove_from_vector = [&](std::vector<particle*>& vec) {
        vec.erase(std::remove(vec.begin(), vec.end(), removed_particle), vec.end());
    };

    remove_from_vector(particles_x);
    remove_from_vector(particles_y);

    // Remove the particle from the particles vector
    particles.erase(particles.begin() + index);

    num_particles--;
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

