#ifndef PARTICLE_VISUALIZATION_H
#define PARTICLE_VISUALIZATION_H

#include "particle_life_state.h"
#include "particle_interaction.h"
#include <QWidget>
#include <QPainter>
#include <QTimer>

class particle_visualization : public QWidget
{
    Q_OBJECT

public:
    double size_particles;
    std::vector<QColor> colormap;
    particle_life_state *state;
    particle_interaction *interaction;

    void take_screenshot(double Quality);

    particle_visualization(QWidget *parent = nullptr, particle_life_state *state = nullptr, particle_interaction *interaction = nullptr);
    ~particle_visualization();

private:
    void draw_particles_on_painter(QPainter &painter, double Quality);
    void draw_interation_circle_on_painter(QPainter &painter);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PARTICLE_VISUALIZATION_H
