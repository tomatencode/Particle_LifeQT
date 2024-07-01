#ifndef CALCULATORFORM_H
#define CALCULATORFORM_H


#include "particle_life_state.h"
#include "ui_settings.h"
#include "particle_visualization.h"
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
    explicit Settings(QWidget *parent = nullptr, particle_visualization *visualization = nullptr, particle_life_state *state = nullptr);
    QPalette pal;
private:
    particle_visualization *visualization;
    particle_life_state *state;
    QTimer *timer;
    std::vector<double> last_tps;
    QTime start_time;
    QTime t_last;
private slots:
    void on_Reset_Button_clicked();
    void on_dt_slider_valueChanged(int value);
    void on_friction_slider_valueChanged(int value);
    void on_force_range_slider_valueChanged(int value);

    void on_stable_dist_slider_valueChanged(int value);
    void on_play_pause_Button_toggled(bool checked);
    void on_screenshot_Button_clicked();
    void on_Random_tb_Button_clicked();
    void on_tabWidget_currentChanged(int index);
signals:
    void togglePause(bool paused);
};

#endif // SETTINGS_H
