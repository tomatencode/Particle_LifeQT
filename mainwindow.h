#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "particle_life_state.h"
#include "settings.h"
#include "particle_visualization.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    particle_life_state state;
    particle_visualization *visualization;
    bool paused;

    MainWindow();
    Settings *settings;

private:
    QPushButton *close_button;
    void update();
    void add_close_button();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleTogglePause(bool paused);
};

#endif // MAINWINDOW_H
