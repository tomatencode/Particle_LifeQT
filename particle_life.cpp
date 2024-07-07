#include "particle_life.h"
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>  // For std::ref


void particle_life::update_all_partclies(particle_life_state &state)
{
    for (particle &p: state.particles)
    {
        p.update(state.dt, state.friction, state.size_x, state.size_y);
    }
}

bool compare_x_pos(particle *p1, particle *p2)
{
    return p1->position[0] > p2->position[0];
}
bool compare_y_pos(particle *p1, particle *p2)
{
    return p1->position[1] > p2->position[1];
}


void particle_life::update(particle_life_state &state)
{
    std::sort(state.particles_x.begin(), state.particles_x.end(), compare_x_pos);
    std::sort(state.particles_y.begin(), state.particles_y.end(), compare_y_pos);

    for (int i = 0; i < state.num_particles; i++) {
        state.particles_y[i]->pos_y_list = i;
    }
    for (int i = 0; i < state.num_particles; i++) {
        state.particles_x[i]->pos_x_list = i;
    }

    // multi threading
    int num_threads = std::min(16,state.num_particles);
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; i++) {
        double i_double = static_cast<double>(i);
        double num_particles_double = static_cast<double>(state.num_particles);
        double num_threads_double = static_cast<double>(num_threads);
        threads.push_back(std::thread(particle_life::apply_all_forces, std::ref(state), static_cast<int>(num_particles_double/num_threads_double*i_double), static_cast<int>(num_particles_double/num_threads_double*(i_double+1.0))));
    }

    for (auto &th : threads) {
        th.join();
    }
}

void particle_life::apply_all_forces(particle_life_state &state, int start_num, int end_num)
{
    std::vector<particle*> neighbors_x_right;
    std::vector<particle*> neighbors_x_left;
    particle* p;
    for (int i=start_num;i< end_num;i++)
    {
        p = state.particles_x[i];
        for (int j = (i+neighbors_x_right.size()+1)%state.num_particles; get_dist(state, *p, *state.particles_x[j])[0] < state.force_range and i != j; j = (j+1)%state.num_particles)
        {
            neighbors_x_right.push_back(state.particles_x[j]);
        }

        int j = i-1;
        if (j < 0) {
            j = state.num_particles+j;
        }

        while (-get_dist(state, *p, *state.particles_x[j])[0] < state.force_range and i != j)
        {
            neighbors_x_left.push_back(state.particles_x[j]);
            j--;
            if (j < 0) {
                j = state.num_particles+j;
            }
        }

        for (int j = p->pos_y_list+1%state.num_particles; get_dist(state, *state.particles_y[p->pos_y_list], *state.particles_y[j])[1] < state.force_range and i != j; j = (j+1)%state.num_particles)
        {
            for (particle* neighbor_x : neighbors_x_right) {
                if (neighbor_x == state.particles_y[j]) {
                    apply_force(state, *p, *neighbor_x);
                }
            }
            for (particle* neighbor_x : neighbors_x_left) {
                if (neighbor_x == state.particles_y[j]) {
                    apply_force(state, *p, *neighbor_x);
                }
            }
        }
        neighbors_x_right.erase(neighbors_x_right.begin());
        neighbors_x_left.clear();
    }
}

double particle_life::get_force(particle_life_state &state, double dist, int type_p1, int type_p2)
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

void particle_life::apply_force(particle_life_state &state, particle& p1, particle& p2)
{
    Eigen::Vector2d diff = get_dist(state, p2, p1);

    if (diff[1] < state.force_range)
    {
        double dist = sqrt(diff.dot(diff));
        if (dist < state.force_range and dist != 0.0)
        {
            Eigen::Vector2d normal = diff / dist;
            double force_p1 = get_force(state, dist/state.force_range, p1.type, p2.type);
            p1.force += normal * force_p1;
            double force_p2 = get_force(state, dist/state.force_range, p2.type, p1.type);
            p2.force += -normal * force_p2;
        }
    }
}

Eigen::Vector2d particle_life::get_dist(particle_life_state &state, const particle p1, const particle p2)
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

void particle_life::auto_dt(particle_life_state &state) {
    double v_max = 0;
    for (particle &p : state.particles) {
        double len = p.velocety.norm();
        if (len > v_max) {
            v_max = len;
        }
    }
    if (v_max > 0) {
        state.dt = state.force_range / v_max;
    } else {
        state.dt = 0.08;  // Handle the case when v_max is zero
    }
}
