#include "settings.h"
#include <QWidget>
#include <QPalette>
#include <string>
#include <QIcon>

void Settings::update_lables() {

    // gets the avrage of the last 100 tps values and diplays them

    last_tps.insert(last_tps.begin(), 1000/t_last.msecsTo(QTime::currentTime()));

    t_last = QTime::currentTime();
    if (static_cast<int>(last_tps.size()) > 100) {
        last_tps.pop_back();
    }
    tps_label->setText(QString::fromStdString("tps: " + std::to_string(static_cast<int>(std::reduce(last_tps.begin(), last_tps.end()) / static_cast<int>(last_tps.size())))));

    // displays the Delta Time
    dt_label->setText(QString::fromStdString("dt: " + std::to_string(state->dt).substr(0, 5)));

    // displays the friction
    friction_label->setText(QString::fromStdString("Friction: " + std::to_string(state->friction).substr(0, 5)));

    // displays num particles
    int num = state->particles.size();
    num_particles_label->setText(QString::fromStdString("Particles: " + std::to_string(num)));

    // displays num Types
    num_types_label->setText(QString::fromStdString("Types: " + std::to_string(state->num_particle_types)));

    // displays Force Range
    force_range_label->setText(QString::fromStdString("Force Range: " + std::to_string(static_cast<int>(state->force_range))));

    // displays Stable Dist
    sable_dist_label->setText(QString::fromStdString("Stable Distance: " + std::to_string(state->stable_dist).substr(0, 4)));

    // displays Interaction Radius
    radius_label->setText(QString::fromStdString("Radius: " + std::to_string(static_cast<int>(interaction->radius))));

    // displays Interaction Strenght
    strenght_label->setText(QString::fromStdString("Strenght: " + std::to_string(interaction->strenght).substr(0, 4)));

    // displays Time Running
    if (not play_pause_Button->isChecked()) {
        int time_running = start_time.secsTo(QTime::currentTime()) - secs_paused;
        if (time_running < 60) {
            time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running) + " sec"));
        }
        else if (time_running < 3600){
            time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running/60) + " min " + std::to_string(time_running%60) + " sec"));
        }
        else {
            time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running/3600) + " h " + std::to_string((time_running%3600)/60) + " min"));
        }
    }
    update();
}

void Settings::on_Reset_Button_clicked() {
    start_time = QTime::currentTime();
    state->reset_particles();
}

void Settings::on_play_pause_Button_toggled(bool checked)
{
    emit togglePause(checked);
    if (checked){
        play_pause_Button->setIcon(QIcon(":/icons/play_icon.png"));
        time_paused = QTime::currentTime();
    }
    else {
        play_pause_Button->setIcon(QIcon(":/icons/pause_icon.png"));
        secs_paused += time_paused.secsTo(QTime::currentTime());
    }
}

void Settings::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        tabWidget->resize(300,150);
    }
    else if (index == 1)
        tabWidget->resize(300,500);
    else {
        tabWidget->resize(300,180);
    }
}

void Settings::on_screenshot_Button_clicked()
{
    visualization->take_screenshot(5.0);
}

void Settings::on_Random_tb_Button_clicked()
{
    state->reset_force_tb();
}

void Settings::on_interaction_Button_clicked()
{
    interaction->mode =(interaction->mode+1)%3;
    switch (interaction->mode) {
    case 0:
        interaction_Button->setText(QString("None"));
        break;
    case 1:
        interaction_Button->setText(QString("Pull"));
        break;
    case 2:
        interaction_Button->setText(QString("Push"));
        break;
    default:
        break;
    }
}

void Settings::on_strenght_slider_valueChanged(int value)
{
    interaction->strenght = value/1000.0;
}


void Settings::on_radius_slider_valueChanged(int value)
{
    interaction->radius = 20 + value/100.0 * 33.0;
}

void Settings::on_dt_slider_valueChanged(int value)
{
    state->dt = 0.02 + static_cast<double>(value)/1000.0 * 0.18;
}

void Settings::on_friction_slider_valueChanged(int value)
{
    state->friction = 0.95 + static_cast<double>(value)/1000.0 * 0.05;
}


void Settings::on_force_range_slider_valueChanged(int value)
{
    state->force_range = 10.0 + static_cast<double>(value)/100.0 * 14.0;
}

void Settings::on_stable_dist_slider_valueChanged(int value)
{
    state->stable_dist = 0.1 + static_cast<double>(value)/1000.0 * 0.7;
}

Settings::Settings(QWidget *parent, particle_interaction *interaction, particle_visualization *visualization, particle_life_state *state)
    : QWidget(parent)
{
    this->interaction = interaction;
    this->visualization = visualization;
    this->state = state;
    setupUi(this);

    start_time = QTime::currentTime();
    last_tps = {1.0};
    t_last = QTime::currentTime();
    secs_paused = 0;

    // set default dlider values
    dt_slider->setSliderPosition(((state->dt-0.02)/0.18)*1000.0+1.0);
    friction_slider->setSliderPosition(((state->friction-0.95)/0.05)*1000.0+1.0);
    force_range_slider->setSliderPosition(((state->force_range-10.0)/140.0)*1000.0+1.0);
    stable_dist_slider->setSliderPosition(((state->stable_dist-0.1)/0.7)*1000.0+1.0);
    radius_slider->setSliderPosition(((interaction->radius-20.0)/330.0)*1000.0+1.0);
    strenght_slider->setSliderPosition(interaction->strenght*1000.0+1.0);

    // set default force tb
    force_tb->state = state;
    force_tb->setParticles({visualization->colormap.begin(), visualization->colormap.begin()+state->num_particle_types});
}

