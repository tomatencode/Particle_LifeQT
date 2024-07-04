#include "force_table_widget.h"

force_table_widget::force_table_widget(QWidget *parent, particle_life_state *state) : QWidget(parent),
    state(state), size(0)
{}

void force_table_widget::setParticles(const std::vector<QColor>& colors) {
    particle_colors = colors;
    size = static_cast<int>(colors.size());
    if (size > static_cast<int>(state->force_tb.size())) {
        state->force_tb.resize(size, std::vector<double>());
        for (std::vector<double> &row : state->force_tb) {
            row.resize(size,0.0);
        }
    }
    particle_colors = colors;
    update_rects();
    update();
}

void force_table_widget::update_rects()
{
    value_rects.resize(size, std::vector<QRect>(size, QRect(0,0,0,0)));
    for (std::vector<QRect> &row : value_rects) {
        row.resize(size, QRect(0,0,0,0));
    }
    particle_rects.resize(size*2,QRect(0,0,0,0));

    int cell_size = static_cast<double>(this->width(),this->height())/(static_cast<double>(particle_colors.size())+1.0);
    int circleRadius = cell_size*0.75;

    // Update row circles
    for (int i = 0; i < size; ++i) {
        particle_rects[i] = QRect(0, circleRadius*1.125 + 5 + i * cell_size, circleRadius, circleRadius);
    }

    // Update column circles
    for (int i = 0; i < size; ++i) {
        particle_rects[i+size] = QRect(circleRadius*1.125 + 5 + i * cell_size, 0, circleRadius, circleRadius);
    }

    // Update cells
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            value_rects[row][col] = QRect(circleRadius + col * cell_size + 5, circleRadius + row * cell_size + 5, cell_size-2, cell_size-2);
        }
    }
    update();
}

QColor force_table_widget::interpolateColor(double value) {
    // Ensure value is within the range [-1, 1]
    value = qMax(-1.0f, qMin(1.0f, value));

    // Interpolate between red black and green
    if (value < 0.0) {
        return QColor(-value*205.0+50.0, 50, 50,-value*100.0+100.0);
    }
    else {
        return QColor(50, value*205.0+50.0, 50,value*100.0+100.0);
    }
}

void force_table_widget::wheelEvent(QWheelEvent* event)
{
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (value_rects[row][col].contains(QPoint(event->position().x(), event->position().y()))){
                if(event->angleDelta().y() > 0){ // up Wheel
                    state->force_tb[row][col] = qMax(-1.0f, qMin(1.0f, state->force_tb[row][col] - 0.05));
                }
                else if(event->angleDelta().y() < 0){ //down Wheel
                    state->force_tb[row][col] = qMax(-1.0f, qMin(1.0f, state->force_tb[row][col] + 0.05));
                }
            }
        }
    }
}

void force_table_widget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw circles
    for (int i = 0; i < size*2; ++i) {
        painter.setBrush(QColor(particle_colors[i%size].red()*0.9, particle_colors[i%size].green()*0.9, particle_colors[i%size].blue()*0.9, 200));
        painter.drawEllipse(particle_rects[i]);
    }

    // Draw cells
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            painter.fillRect(value_rects[row][col], interpolateColor(state->force_tb[row][col]));
        }
    }
}
