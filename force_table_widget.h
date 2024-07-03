#ifndef force_table_widget_H
#define force_table_widget_H

#include "particle_life_state.h"
#include <QWidget>
#include <QWheelEvent>
#include <QPainter>
#include <vector>

class force_table_widget : public QWidget {
    Q_OBJECT

public:
    explicit force_table_widget(QWidget *parent = nullptr, particle_life_state *state = nullptr);
    particle_life_state *state;
    void setParticles(const std::vector<QColor>& colors);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    int size;
    std::vector<QRect> particle_rects;
    std::vector<std::vector<QRect>> value_rects;
    std::vector<QColor> particle_colors;

    void update_rects();
    QColor interpolateColor(double value);
};

#endif // force_table_widget_H
