#ifndef CALCULATORFORM_H
#define CALCULATORFORM_H


#include "particle_life_state.h"
#include "ui_settings.h"
#include "particle_visualization.h"
#include "particle_interaction.h"
#include <QLabel>
#include <QPalette>
#include <QTimer>
#include <QTime>
#include <vector>


class Settings : public QWidget, private Ui::Settings_Widget
{
    Q_OBJECT

public:
    void update_lables();
    explicit Settings(QWidget *parent = nullptr, particle_interaction* interaction = nullptr, particle_visualization *visualization = nullptr, particle_life_state *state = nullptr);
    QPalette pal;
private:
    particle_visualization *visualization;
    particle_life_state *state;
    particle_interaction  *interaction;
    QTimer *timer;
    std::vector<double> last_tps;
    QTime start_time;
    QTime t_last;
    QTime time_paused;
    int secs_paused;
private slots:
    void on_dt_slider_valueChanged(int value);
    void on_friction_slider_valueChanged(int value);
    void on_force_range_slider_valueChanged(int value);

    void on_stable_dist_slider_valueChanged(int value);
    void on_play_pause_Button_toggled(bool checked);
    void on_screenshot_Button_clicked();
    void on_Random_tb_Button_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_interaction_Button_clicked();
    void on_strenght_slider_valueChanged(int value);

    void on_types_spinBox_valueChanged(int arg1);

signals:
    void togglePause(bool paused);
};

#endif // SETTINGS_H
