#ifndef PARTICLE_VISUALIZATION_H
#define PARTICLE_VISUALIZATION_H

#include "particle_life.h"
#include <QWidget>
#include <QPainter>
#include <QTimer>

class particle_visualization : public QWidget
{
    Q_OBJECT

public:
    bool paused;
    int size_x;
    int size_y;
    double size_particles;
    double tps;
    std::vector<QColor> colormap;
    particle_life *particle_life_ptr;

    void take_screenshot(double Quality);
    std::vector<std::vector<double>> get_random_force_tb(int size);

    particle_visualization(QWidget *parent = nullptr);
    ~particle_visualization();

private slots:
    void update_visualization();

public slots:
    void reset_particles(int const num, int const num_type);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTimer *timer;
    QTime t_last;
    std::vector<particle> get_random_particles(int const num, int const num_types);
};

#endif // PARTICLE_VISUALIZATION_H
