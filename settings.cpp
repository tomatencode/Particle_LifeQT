#include "settings.h"
#include <QWidget>
#include <QPalette>
#include <string>
#include <QIcon>

void Settings::update_lables() {
    // sets the force table of the simulation to the force table of the widet
    this->state->force_tb = this->force_tb->values;

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

    // displays Time Running
    int time_running = start_time.secsTo(QTime::currentTime());
    if (time_running < 60) {
        time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running) + " sec"));
    }
    else if (time_running < 3600){
        time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running/60) + " min " + std::to_string(time_running%60) + " sec"));
    }
    else {
        time_running_label->setText(QString::fromStdString("Time Running: " + std::to_string(time_running/3600) + " h " + std::to_string((time_running%3600)/60) + " min"));
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
    }
    else {
        play_pause_Button->setIcon(QIcon(":/icons/pause_icon.png"));
    }
}

void Settings::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        tabWidget ->resize(300,150);
    }
    else if (index == 1)
        tabWidget ->resize(300,500);
    else {
        tabWidget ->resize(300,500);
    }
}


void Settings::on_screenshot_Button_clicked()
{
    visualization->take_screenshot(5.0);
}

void Settings::on_Random_tb_Button_clicked()
{
    state->reset_force_tb();
    force_tb->values = state->force_tb;
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
    state->force_range = 10.0 + static_cast<double>(value)/1000.0 * 140.0;
}

void Settings::on_stable_dist_slider_valueChanged(int value)
{
    state->stable_dist = 0.1 + static_cast<double>(value)/1000.0 * 0.7;
}

Settings::Settings(QWidget *parent, particle_visualization *visualization, particle_life_state *state)
    : QWidget(parent)
{

    this->visualization = visualization;
    this->state = state;
    setupUi(this);

    start_time = QTime::currentTime();
    last_tps = {1.0};
    t_last = QTime::currentTime();


    // set default dlider values
    dt_slider->setSliderPosition(((state->dt-0.02)/0.18)*1000.0+1.0);
    friction_slider->setSliderPosition(((state->friction-0.95)/0.05)*1000.0+1.0);
    force_range_slider->setSliderPosition(((state->force_range-10.0)/140.0)*1000.0+1.0);
    stable_dist_slider->setSliderPosition(((state->stable_dist-0.1)/0.7)*1000.0+1.0);

    // set default force tb
    force_tb->setParticles({visualization->colormap.begin(), visualization->colormap.begin()+state->num_particle_types});
    force_tb->values = state->force_tb;
}


