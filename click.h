#ifndef CLICK_H
#define CLICK_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = nullptr) : QLabel(parent) {
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked(); // Sinyal gönder
        }
        QLabel::mousePressEvent(event); // Temel sınıfın işlevini çağır
    }
};

#endif // CLICK_H
