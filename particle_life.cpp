#include "particle_life.h"
#include <vector>
#include <thread>
#include <algorithm>


particle_life::particle_life(particle_life_state state)
{
    this->state = state;
}

bool compare_x_pos(particle p1, particle p2)
{
    return p1.position[0] > p2.position[0];
}


void particle_life::update()
{
    sort(state.particles.begin(), state.particles.end(), compare_x_pos);

    int num_particles = state.particles.size();


    // multi threading
    int num_threads = std::min(16,num_particles);
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        double i_double = static_cast<double>(i);
        double num_particles_double = static_cast<double>(num_particles);
        double num_threads_double = static_cast<double>(num_threads);
        threads.push_back(std::thread(&particle_life::apply_all_forces, this, static_cast<int>(num_particles_double/num_threads_double*i_double), static_cast<int>(num_particles_double/num_threads_double*(i_double+1.0))));
    }

    for (auto &th : threads) {
        th.join();
    }
}

void particle_life::apply_all_forces(int start_num, int end_num)
{
    int num_particles = state.particles.size();
    for (int i=start_num;i< end_num;i++)
    {
        int j = (i+1)%num_particles;

        while (get_dist(state.particles[i], state.particles[j])[0] < state.force_range and i != j)
        {
            apply_force(state.particles[j], state.particles[i]);

            j = (j+1)%num_particles;
        }
        state.particles[i].update(state.dt, state.friction, state.size_x, state.size_y);
    }
}

double particle_life::get_force(double dist, int type_p1, int type_p2)
{
    if (dist < 1.0)
    {
        double force = state.force_tb[type_p1][type_p2];
        if (force >= 0.0)
        {
            if (dist < state.stable_dist)
            {
                return dist/state.stable_dist - 1.0;
            }
            else
            {
                return force * (1-abs(2*dist-1-state.stable_dist)/(1-state.stable_dist));
            }
        }
        else
        {
            if (dist < state.stable_dist)
            {
                return dist*(1.0+force)/state.stable_dist-1.0;
            }
            else
            {
                return dist*force/(state.stable_dist-1.0) + force-force*state.stable_dist/(state.stable_dist-1.0);
            }
        }
    }
    else {
        return 0.0;
    }
}

void particle_life::apply_force(particle& p1, particle& p2)
{
    Eigen::Vector2d diff = get_dist(p2, p1);


    if (diff[1] < state.force_range)
    {
        double dist = sqrt(diff.dot(diff));
        if (dist < state.force_range and dist != 0.0)
        {
            Eigen::Vector2d normal = diff / dist;
            double force_p1 = get_force(dist/state.force_range, p1.type, p2.type);
            p1.force += normal * force_p1;
            double force_p2 = get_force(dist/state.force_range, p2.type, p1.type);
            p2.force += -normal * force_p2;
        }
    }
}

Eigen::Vector2d particle_life::get_dist(const particle p1, const particle p2)
{
    double x_diff;
    if (abs(p2.position[0] - p1.position[0]) < abs(abs(p2.position[0] - p1.position[0]) - state.size_x)) {
        x_diff = -(p2.position[0] - p1.position[0]);
    }
    else {
        if (p1.position[0] > p2.position[0]) {
            x_diff = p1.position[0] - p2.position[0]-state.size_x;
        }
        else {
            x_diff = -(p2.position[0] - p1.position[0]-state.size_x);
        }
    }
    double y_diff;
    if (abs(p2.position[1] - p1.position[1]) < abs(abs(p2.position[1] - p1.position[1])-state.size_y)) {
        y_diff = -(p2.position[1] - p1.position[1]);
    }
    else {
        if (p1.position[1] > p2.position[1]) {
            y_diff = p1.position[1] - p2.position[1]-state.size_y;
        }
        else {
            y_diff = -(p2.position[1] - p1.position[1]-state.size_y);
        }
    }
    return {x_diff , y_diff};
}

