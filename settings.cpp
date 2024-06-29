#include "settings.h"
#include <QWidget>
#include <QPalette>
#include <string>
#include <QIcon>

void Settings::update_lables() {
    // sets the force table of the simulation to the force table of the widet
    this->visualization->particle_life_ptr->force_tb = this->force_tb->values;

    // gets the avrage of the last 100 tps values and diplays them
    last_tps.insert(last_tps.begin(), visualization->tps);
    if (static_cast<int>(last_tps.size()) > 100) {
        last_tps.pop_back();
    }
    tps_label->setText(QString::fromStdString("tps: " + std::to_string(static_cast<int>(std::reduce(last_tps.begin(), last_tps.end()) / static_cast<int>(last_tps.size())))));

    // displays the Delta Time
    dt_label->setText(QString::fromStdString("dt: " + std::to_string(visualization->particle_life_ptr->dt).substr(0, 5)));

    // displays the friction
    friction_label->setText(QString::fromStdString("Friction: " + std::to_string(visualization->particle_life_ptr->friction).substr(0, 5)));

    // displays num particles
    int num = visualization->particle_life_ptr->particles.size();
    num_particles_label->setText(QString::fromStdString("Particles: " + std::to_string(num)));

    // displays num Types
    num_types_label->setText(QString::fromStdString("Types: " + std::to_string(static_cast<int>(visualization->colormap.size()))));

    // displays Force Range
    force_range_label->setText(QString::fromStdString("Force Range: " + std::to_string(static_cast<int>(visualization->particle_life_ptr->force_range))));

    // displays Stable Dist
    sable_dist_label->setText(QString::fromStdString("Stable Distance: " + std::to_string(visualization->particle_life_ptr->stable_dist).substr(0, 4)));

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
}

void Settings::on_Reset_Button_clicked() {
    start_time = QTime::currentTime();
    visualization->reset_particles(visualization->particle_life_ptr->particles.size(), static_cast<int>(visualization->particle_life_ptr->force_tb.size()));
}

void Settings::on_play_pause_Button_toggled(bool checked)
{
    if (checked){
        play_pause_Button->setIcon(QIcon(":/icons/play_icon.png"));
        visualization->paused = true;
    }
    else {
        play_pause_Button->setIcon(QIcon(":/icons/pause_icon.png"));
        visualization->paused = false;
    }
}


void Settings::on_screenshot_Button_clicked()
{
    visualization->take_screenshot(3.0);
}

void Settings::on_dt_slider_valueChanged(int value)
{
    visualization->particle_life_ptr->dt = 0.02 + static_cast<double>(value)/1000.0 * 0.18;
}

void Settings::on_friction_slider_valueChanged(int value)
{
    visualization->particle_life_ptr->friction = 0.95 + static_cast<double>(value)/1000.0 * 0.05;
}


void Settings::on_force_range_slider_valueChanged(int value)
{
    visualization->particle_life_ptr->force_range = 10.0 + static_cast<double>(value)/1000.0 * 140.0;
}

void Settings::on_stable_dist_slider_valueChanged(int value)
{
    visualization->particle_life_ptr->stable_dist = 0.1 + static_cast<double>(value)/1000.0 * 0.7;
}

Settings::Settings(QWidget *parent, particle_visualization *visualization)
    : QWidget(parent)
{
    this->visualization = visualization;
    setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Settings::update_lables);
    timer->start(10);

    start_time = QTime::currentTime();
    last_tps = {};

    // set default dlider values
    dt_slider->setSliderPosition(((visualization->particle_life_ptr->dt-0.02)/0.18)*1000.0+1.0);
    friction_slider->setSliderPosition(((visualization->particle_life_ptr->friction-0.95)/0.05)*1000.0+1.0);
    force_range_slider->setSliderPosition(((visualization->particle_life_ptr->force_range-10.0)/140.0)*1000.0+1.0);
    stable_dist_slider->setSliderPosition(((visualization->particle_life_ptr->stable_dist-0.1)/0.7)*1000.0+1.0);

    // set default force tb
    force_tb->setParticles(visualization->colormap);
    force_tb->values = visualization->particle_life_ptr->force_tb;
}
