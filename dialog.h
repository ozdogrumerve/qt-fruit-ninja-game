#ifndef DIALOG_H
#define DIALOG_H

#include "click.h"
#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMessageBox>
#include <QList>
#include <QPoint>
#include <QRandomGenerator>


QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    QList<QPoint> loadFruitPositions(const QString& filePath);

    //for melons
    QList<QPoint> m_watermelonPositions;
    QTimer* m_watermelonTimer;

    //for pineapples
    QList<QPoint> m_pineapplePositions;
    QTimer* m_pineappleTimer;

    //for strawberries
    QList<QPoint> m_strawberryPositions;
    QTimer* m_strawberryTimer;

private slots:
    void spawnWatermelon();
    void spawnPineapple();
    void spawnStrawberry();
    void time();
    void playCutSound();
    void playBombaSound();
    void updateMaxScore(int newScore);


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Dialog *ui;
    QTimer *timer;
    QPixmap m_background;

    QList<QPoint> m_bombaPositions;
    QTimer* m_bombaTimer;
    void spawnBomba();

    int screenHeight;
    QList<ClickableLabel*> m_tumMeyveler;
};

#endif // DIALOG_H
