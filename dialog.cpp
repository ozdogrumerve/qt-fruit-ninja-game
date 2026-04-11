#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include "click.h"
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QDebug>
#include <QPropertyAnimation>
#include <windows.h> // Bu satırı ekle

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    ui->frame->setGeometry(0, 0, this->maximumWidth(), 100);

    QScreen *screen = QApplication::primaryScreen();
    // Ekran genişliğini al
    int screenWidth = screen->availableGeometry().width();
    screenHeight = screen->availableGeometry().height();

    //Widget genisligi al
    int widgetSize = ui->splitter->width();

    ui->splitter->move(screenWidth-widgetSize-10, 10);

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(time()));
    setAutoFillBackground(false);
    m_background.load(":/resimler/images/back.jpg");

    // for watermelon
    m_watermelonPositions = loadFruitPositions("konumlar.txt");

    // for pineapple
    m_pineapplePositions = loadFruitPositions("konumlar.txt");

    //for strawberry
    m_strawberryPositions = loadFruitPositions("konumlar.txt");

    //for bomb
    m_bombaPositions = loadFruitPositions("konumlar.txt");

    // timer for watermelon
    m_watermelonTimer = new QTimer(this);
    connect(m_watermelonTimer, &QTimer::timeout, this, &Dialog::spawnWatermelon);
    m_watermelonTimer->start(QRandomGenerator::global()->bounded(3001) + 1750);

    // timer for pineapple
    m_pineappleTimer = new QTimer(this);
    connect(m_pineappleTimer, &QTimer::timeout, this, &Dialog::spawnPineapple);
    m_pineappleTimer->start(QRandomGenerator::global()->bounded(3001) + 1750);

    //timer for strawberry
    m_strawberryTimer = new QTimer(this);
    connect(m_strawberryTimer, &QTimer::timeout, this, &Dialog::spawnStrawberry);
    m_strawberryTimer->start(QRandomGenerator::global()->bounded(3001) + 1750);

    //4 defa atılmak üzere bomba
    m_bombaTimer = new QTimer(this);
    connect(m_bombaTimer, &QTimer::timeout, this, &Dialog::spawnBomba);
    m_bombaTimer->start(7500); // 7.5 saniyede bir bomba (30s/4=7.5s)

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Arka plan resmini çiz
    if(!m_background.isNull()) {
        painter.drawPixmap(rect(), m_background);
    }

    QDialog::paintEvent(event); // Temel sınıfın paintEvent'ini çağır
}

void Dialog::time()
{
    int sure = ui->lb_time->text().toInt();

    int karpuz = ui->lb_cut_melon->text().toInt(),
        ananas = ui->lb_cut_pine->text().toInt(),
        strawberry = ui->lb_cut_berry->text().toInt();
    int missed_melon = ui->lb_missed_melon->text().toInt(),
        missed_pine = ui->lb_missed_pine->text().toInt(),
        missed_straw = ui->lb_missed_berry->text().toInt();

    int karpuzSkor =  ui->lb_cut_melon->text().toInt() * 5;
    int pineSkor = ui->lb_cut_pine->text().toInt() * 3;
    int berrySkor = ui->lb_cut_berry->text().toInt();
    int skor = karpuzSkor+pineSkor+berrySkor;
    int maxSkor = 0;

    QFile file("skorlar.txt");

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "HATA", file.errorString());
        return;
    } else {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            int mSkor = line.toInt();
            if (mSkor > maxSkor) maxSkor = mSkor;
        }
        file.close();
    }

    sure -= 1;
    if(sure != -1) ui->lb_time->setText(QString::number(sure));

    if (sure <= 0) {

        for (auto anim : findChildren<QPropertyAnimation*>()) anim->stop();
        m_tumMeyveler.clear();

        if (m_watermelonTimer) {
            if (m_watermelonTimer->isActive()) {
                m_watermelonTimer->stop();
            }
            delete m_watermelonTimer;
            m_watermelonTimer = nullptr;
        }

        if (m_pineappleTimer) {
            if (m_pineappleTimer->isActive()) {
                m_pineappleTimer->stop();
            }
            delete m_pineappleTimer;
            m_pineappleTimer = nullptr;
        }

        if (m_strawberryTimer) {
            if (m_strawberryTimer->isActive()) {
                m_strawberryTimer->stop();
            }
            delete m_strawberryTimer;
            m_strawberryTimer = nullptr;
        }

        if (m_bombaTimer) {
            m_bombaTimer->stop();
            delete m_bombaTimer;
            m_bombaTimer = nullptr;
        }

        if (skor > maxSkor) {
            QMessageBox::information(this, "Bilgi", "PUANLAR\nKARPUZ: 5P | ANANAS: 3P | ÇİLEK: 1P\n"
                                                    "\nKesilen Karpuz Sayısı: "+QString::number(karpuz)+
                                                    "\nKaçırılan Karpuz Sayısı: "+QString::number(missed_melon)+
                                                    "\nKazanılan Karpuz Puanı: "+QString::number(karpuzSkor)+ "\n"
                                                    "\nKesilen Ananas Sayısı: "+QString::number(ananas)+
                                                    "\nKaçırılan Ananas Sayısı: "+QString::number(missed_pine)+
                                                    "\nKazanılan Ananas Puanı: "+QString::number(pineSkor)+ "\n"
                                                    "\nKesilen Çilek Sayısı: "+QString::number(strawberry)+
                                                    "\nKaçırılan Çilek Sayısı: "+QString::number(missed_straw)+
                                                    "\nKazanılan Çilek Puanı: "+QString::number(berrySkor)+ "\n"
                                                    "\nÖnceki Maksimum Skor: "+QString::number(maxSkor)+ "\n"
                                                    "Tebrikler Yeni Maksimum Skor Sizde! Toplam Skorunuz: " + QString::number(skor));


        } else {
            QMessageBox::information(this, "Bilgi", "PUANLAR\nKARPUZ: 5P | ANANAS: 3P | ÇİLEK: 1P\n"
                                                    "\nKesilen Karpuz Sayısı: "+QString::number(karpuz)+
                                                    "\nKaçırılan Karpuz Sayısı: "+QString::number(missed_melon)+
                                                    "\nKazanılan Karpuz Puanı: "+QString::number(karpuzSkor)+ "\n"
                                                    "\nKesilen Ananas Sayısı: "+QString::number(ananas)+
                                                    "\nKaçırılan Ananas Sayısı: "+QString::number(missed_pine)+
                                                    "\nKazanılan Ananas Puanı: "+QString::number(pineSkor)+ "\n"
                                                    "\nKesilen Çilek Sayısı: "+QString::number(strawberry)+
                                                    "\nKaçırılan Çilek Sayısı: "+QString::number(missed_straw)+
                                                    "\nKazanılan Çilek Puanı: "+QString::number(berrySkor)+ "\n"
                                                    "\nÖnceki Maksimum Skor: "+QString::number(maxSkor)+ "\n"
                                                    "Üzgünüz, Maksimum Skoru Geçemediniz! Toplam Skorunuz: " + QString::number(skor));
        }

        //SKORLAR DOSYASINA SKORU YAZ
        updateMaxScore(skor);
        close();
    }
}

QList<QPoint> Dialog::loadFruitPositions(const QString& filePath) {
    QList<QPoint> positions;
    QFile file(filePath); // Parametreyi kullan

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed(); // trimmed ekledik
            if(line.isEmpty()) continue; // Boş satırları atla
            QStringList coords = line.split(' ', Qt::SkipEmptyParts);
            if (coords.size() >= 2) {
                bool okX, okY;
                int x = coords[0].toInt(&okX);
                int y = coords[1].toInt(&okY);
                if(okX && okY) positions.append(QPoint(x, y));
            }
        }
        file.close();
    } else {
        qDebug() << "Dosya açılamadı:" << file.errorString();
    }

    return positions;
}


void Dialog::spawnWatermelon(){
    int rastgeleIndex = QRandomGenerator::global()->bounded(m_watermelonPositions.size());
    QPoint p = m_watermelonPositions.at(rastgeleIndex);
    ClickableLabel* karpuz = new ClickableLabel(this); //resim icin
    m_tumMeyveler.append(karpuz);

    QPixmap resim(":/resimler/images/watermelon.png");
    if (resim.isNull()) {
        qDebug() << "Error! Label melon cannot be load!";
        return;
    }

    karpuz->setPixmap(resim);
    karpuz->setFixedSize(50, 50);
    karpuz->setScaledContents(true);
    karpuz->move(p);
    karpuz->setProperty("cut", false);
    karpuz->show();
    karpuz->raise();

    QPropertyAnimation* fallAnim = new QPropertyAnimation(karpuz, "pos", karpuz); // parent olarak karpuz
    fallAnim->setDuration(4750);
    fallAnim->setStartValue(karpuz->pos());
    fallAnim->setEndValue(QPoint(karpuz->x(), height()));
    fallAnim->start();

    connect(karpuz, &ClickableLabel::clicked, this, [this, karpuz, fallAnim]() {
        if (!karpuz->property("cut").toBool()) {
            fallAnim->stop();
            ui->lb_cut_melon->setText(QString::number(ui->lb_cut_melon->text().toInt()+1));
            karpuz->setPixmap(QPixmap(":/resimler/images/slice_watermelon.png"));
            karpuz->setProperty("cut", true);
            playCutSound();
            karpuz->setVisible(false);
        }
    });

    connect(fallAnim, &QPropertyAnimation::finished, [this, karpuz]() {
        if (!karpuz->property("cut").toBool() && karpuz->isEnabled()) {
            ui->lb_missed_melon->setText(QString::number(ui->lb_missed_melon->text().toInt() + 1));
        }
    });
}

void Dialog::spawnPineapple(){
    int rastgeleIndex = QRandomGenerator::global()->bounded(m_pineapplePositions.size());
    QPoint p = m_pineapplePositions.at(rastgeleIndex);
    ClickableLabel* pineapple = new ClickableLabel(this);
    m_tumMeyveler.append(pineapple);

    QPixmap resim(":/resimler/images/pineapple.png");
    if (resim.isNull()) {
        qDebug() << "Error! Label pine cannot be load!";
        return;
    }

    pineapple->setPixmap(resim);
    pineapple->setFixedSize(45, 75);
    pineapple->setScaledContents(true);
    pineapple->move(p);
    pineapple->setProperty("cut", false);
    pineapple->show();
    pineapple->raise();

    QPropertyAnimation* fallAnim = new QPropertyAnimation(pineapple, "pos", pineapple);
    fallAnim->setDuration(4750);
    fallAnim->setStartValue(pineapple->pos());
    fallAnim->setEndValue(QPoint(pineapple->x(), height()));
    fallAnim->start();

    connect(pineapple, &ClickableLabel::clicked, this, [this, pineapple, fallAnim]() {
        if (!pineapple->property("cut").toBool()) {
            fallAnim->stop();
            ui->lb_cut_pine->setText(QString::number(ui->lb_cut_pine->text().toInt()+1));
            pineapple->setPixmap(QPixmap(":/resimler/images/slice_pineapple.png"));
            pineapple->setProperty("cut", true);
            playCutSound();
            pineapple->setVisible(false);
        }
    });

    connect(fallAnim, &QPropertyAnimation::finished, [this, pineapple]() {
        if (!pineapple->property("cut").toBool() && pineapple->isEnabled()) {
            ui->lb_missed_pine->setText(QString::number(ui->lb_missed_pine->text().toInt() + 1));
        }
    });
}

void Dialog::spawnStrawberry(){
    int rastgeleIndex = QRandomGenerator::global()->bounded(m_strawberryPositions.size());
    QPoint p = m_strawberryPositions.at(rastgeleIndex);
    ClickableLabel* strawberry = new ClickableLabel(this);
    m_tumMeyveler.append(strawberry);

    QPixmap resim(":/resimler/images/strawberry.png");
    if (resim.isNull()) {
        qDebug() << "Error! Label straw cannot be load!";
        return;
    }

    strawberry->setPixmap(resim);
    strawberry->setFixedSize(45, 75);
    strawberry->setScaledContents(true);
    strawberry->move(p);
    strawberry->setProperty("cut", false);
    strawberry->show();
    strawberry->raise();

    QPropertyAnimation* fallAnim = new QPropertyAnimation(strawberry, "pos", strawberry);
    fallAnim->setDuration(4750);
    fallAnim->setStartValue(strawberry->pos());
    fallAnim->setEndValue(QPoint(strawberry->x(), height()));
    fallAnim->start();
    connect(fallAnim, &QPropertyAnimation::finished, []() {});

    connect(strawberry, &ClickableLabel::clicked, this, [this, strawberry, fallAnim]() {
        if (!strawberry->property("cut").toBool()) {
            fallAnim->stop();
            ui->lb_cut_berry->setText(QString::number(ui->lb_cut_berry->text().toInt()+1));
            strawberry->setPixmap(QPixmap(":/resimler/images/slice_strawberry.png"));
            strawberry->setProperty("cut", true);
            playCutSound();
            strawberry->setVisible(false);
        }
    });

    connect(fallAnim, &QPropertyAnimation::finished, [this, strawberry]() {
        if (!strawberry->property("cut").toBool() && strawberry->isEnabled()) {
            ui->lb_missed_berry->setText(QString::number(ui->lb_missed_berry->text().toInt() + 1));
        }
    });
}

void Dialog::spawnBomba() {
    int rastgeleIndex = QRandomGenerator::global()->bounded(m_bombaPositions.size());
    QPoint p = m_bombaPositions.at(rastgeleIndex);
    ClickableLabel* bomba = new ClickableLabel(this);
    m_tumMeyveler.append(bomba);

    QPixmap resim(":/resimler/images/bomb.png");
    if (resim.isNull()) {
        qDebug() << "Bomba resmi yüklenemedi!";
        return;
    }

    bomba->setPixmap(resim);
    bomba->setFixedSize(65, 65);
    bomba->setScaledContents(true);
    bomba->move(p);
    bomba->setProperty("patladi", false);
    bomba->show();

    // Animasyon için QPropertyAnimation
    QPropertyAnimation* anim = new QPropertyAnimation(bomba, "pos", bomba);
    anim->setDuration(5000); // 5 saniyede yukarı çıkacak
    anim->setStartValue(QPoint(p.x(), height()));
    anim->setEndValue(QPoint(p.x(), 100));
    anim->start();

    // Tıklanınca patlat
    connect(bomba, &ClickableLabel::clicked, this, [this, bomba]() {
        if (!bomba->property("patladi").toBool()) {

            for (ClickableLabel* meyve : m_tumMeyveler) {
                meyve->setEnabled(false); // Tıklanamaz yap
            }

            for (auto anim : findChildren<QPropertyAnimation*>()) anim->stop();
            m_tumMeyveler.clear();

            if (m_pineappleTimer) {
                if (m_pineappleTimer->isActive()) {
                    m_pineappleTimer->stop();
                }
                delete m_pineappleTimer;
                m_pineappleTimer = nullptr;
            }

            if (m_watermelonTimer) {
                if (m_watermelonTimer->isActive()) {
                    m_watermelonTimer->stop();
                }
                delete m_watermelonTimer;
                m_watermelonTimer = nullptr;
            }

            if (m_strawberryTimer) {
                if (m_strawberryTimer->isActive()) {
                    m_strawberryTimer->stop();
                }
                delete m_strawberryTimer;
                m_strawberryTimer = nullptr;
            }

            if (m_bombaTimer) {
                if(m_bombaTimer->isActive()){
                    m_bombaTimer->stop();
                }
                delete m_bombaTimer;
                m_bombaTimer = nullptr;
            }

            bomba->setPixmap(QPixmap(":/resimler/images/patlama.png"));
            playBombaSound(); // Patlama sesi
            bomba->setProperty("patladi", true);

            ui->lb_time->setText(QString::number(0));
        }
    });

    // Tıklanmazsa aşağı düşsün
    QTimer::singleShot(5000, this, [this, bomba]() {
        if (!bomba->property("patladi").toBool()) {
            QPropertyAnimation* fallAnim = new QPropertyAnimation(bomba, "pos");
            fallAnim->setDuration(5000);
            fallAnim->setStartValue(bomba->pos());
            fallAnim->setEndValue(QPoint(bomba->x(), height()));
            fallAnim->start();
            connect(fallAnim, &QPropertyAnimation::finished, [bomba]() { bomba->deleteLater(); });
        }
    });
}


// Ses çalma fonksiyonu
void Dialog::playCutSound() {
    QSoundEffect effect;

    // 1. Ses dosyasını yükle (qrc kullanarak)
    effect.setSource(QUrl("qrc:/sesler/sounds/cutSound.wav"));

    // 2. SES SEVİYESİNİ AYARLA (0.0-1.0 arası)
    effect.setVolume(0.8f);  // DİKKAT: setVolume()

    // 3. Hata kontrolü
    connect(&effect, &QSoundEffect::statusChanged, [&](){
        if(effect.status() == QSoundEffect::Error) {
            qDebug() << "Ses hatası:";
        }
    });

    // 4. Sesi çal
    effect.play();

    // 5. Ses bitene kadar bekle (opsiyonel)
    QEventLoop loop;
    QTimer::singleShot(2000, &loop, &QEventLoop::quit);
    loop.exec();
}

void Dialog::playBombaSound(){
    QSoundEffect effect;

    // 1. Ses dosyasını yükle (qrc kullanarak)
    effect.setSource(QUrl("qrc:/sesler/sounds/bomb.wav"));

    // 2. SES SEVİYESİNİ AYARLA (0.0-1.0 arası)
    effect.setVolume(0.8f);  // DİKKAT: setVolume()

    // 3. Hata kontrolü
    connect(&effect, &QSoundEffect::statusChanged, [&](){
        if(effect.status() == QSoundEffect::Error) {
            qDebug() << "Ses hatası:";
        }
    });

    // 4. Sesi çal
    effect.play();

    // 5. Ses bitene kadar bekle (opsiyonel)
    QEventLoop loop;
    QTimer::singleShot(1000, &loop, &QEventLoop::quit);
    loop.exec();

}

void Dialog::updateMaxScore(int newScore) {
    QString dosyaYolu = "skorlar.txt";
    QFile file(dosyaYolu);

    // 1. Dosyayı oku ve satırları listeye al
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList satirlar;

        // İlk boş satırı atla (opsiyonel)
        while (!in.atEnd()) {
            QString satir = in.readLine().trimmed();
            if (!satir.isEmpty()) {
                satirlar << satir;
            }
        }
        file.close();

        // Yeni skoru ekle
        satirlar << QString::number(newScore);

        // Dosyayı yeniden yaz
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "\n"; // İlk satır boş
            for (const QString &satir : satirlar) {
                out << satir << "\n";
            }
            file.close();
        }
    }
}
