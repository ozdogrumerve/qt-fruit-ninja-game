#ifndef FROZEN_H
#define FROZEN_H

#include <QLabel>

class FrozenFruit : public QLabel {

public:
    FrozenFruit(QPixmap resim, QWidget* parent = nullptr) : QLabel(parent) {
        setPixmap(resim);
        setFixedSize(resim.size());
        setScaledContents(false);

    }
};

#endif // FROZEN_H
