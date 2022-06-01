#include <QDebug>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      sliderGravity(Qt::Horizontal),
      sliderJumpHeight(Qt::Horizontal)
{
    this->gravity = new qreal;
    this->jumpHeight = new qreal;
    *this->gravity = 25;
    *this->jumpHeight = 2.2;

    this->mainScene = new MyScene(this->gravity, this->jumpHeight);
    QGraphicsView *playerView = mainScene->playerView();

    this->setCentralWidget(playerView);
    this->setWindowTitle("thing maker");
    this->resize(playerView->width(), playerView->height());

    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *settingsHelp = new QAction(tr("&Settings"), this);
    connect(settingsHelp, SIGNAL(triggered()), this, SLOT(slot_settingsMenu()));
    helpMenu->addAction(settingsHelp);

    QAction *aboutHelp = new QAction(tr("&About"), this);
    connect(aboutHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(aboutHelp);

    QWidget *widgetGravity = new QWidget();
    QWidget *widgetJumpHeight = new QWidget();

    QHBoxLayout *layoutGravity = new QHBoxLayout();
    QHBoxLayout *layoutJumpHeight = new QHBoxLayout();

    QLabel *labelGravity = new QLabel("Gravity:");
    QLabel *labelJumpHeight = new QLabel("Jump Height:");

    this->sliderGravity.setMinimum(10);
    this->sliderGravity.setMaximum(1000);
    this->sliderGravity.setValue((int)this->getGravity() * 10);
    this->labelGravityValue.setText(QString::number(this->getGravity()));
    this->labelGravityValue.setFixedWidth(30);

    this->sliderJumpHeight.setMinimum(10);
    this->sliderJumpHeight.setMaximum(100);
    this->sliderJumpHeight.setValue((int)this->getJumpHeight() * 10);
    this->labelJumpHeightValue.setText(QString::number(this->getJumpHeight()));
    this->labelJumpHeightValue.setFixedWidth(30);

    layoutGravity->addWidget(labelGravity);
    layoutGravity->addWidget(&this->sliderGravity);
    layoutGravity->addWidget(&this->labelGravityValue);
    widgetGravity->setLayout(layoutGravity);

    layoutJumpHeight->addWidget(labelJumpHeight);
    layoutJumpHeight->addWidget(&this->sliderJumpHeight);
    layoutJumpHeight->addWidget(&this->labelJumpHeightValue);
    widgetJumpHeight->setLayout(layoutJumpHeight);

    QVBoxLayout *layoutSettings = new QVBoxLayout();

    layoutSettings->addWidget(widgetGravity);
    layoutSettings->addWidget(widgetJumpHeight);

    this->settingsMenu = new QDialog();
    this->settingsMenu->setWindowTitle("Settings");
    this->settingsMenu->setLayout(layoutSettings);

    connect(&this->sliderGravity, &QSlider::valueChanged,
            &this->labelGravityValue, [this](int value)
            { labelGravityValue.setText(QString::number((qreal)value / 10)); 
            setGravity((qreal)value / 10); });

    connect(&this->sliderJumpHeight, &QSlider::valueChanged,
            &this->labelJumpHeightValue, [this](int value)
            { labelJumpHeightValue.setText(QString::number((qreal)value / 10)); 
            setJumpHeight((qreal)value/10); });
}

void MainWindow::slot_settingsMenu()
{
    this->sliderGravity.setValue((int)this->getGravity() * 10);
    this->labelGravityValue.setText(QString::number(this->getGravity()));

    this->sliderJumpHeight.setValue((int)this->getJumpHeight() * 10);
    this->labelJumpHeightValue.setText(QString::number(this->getJumpHeight()));

    this->settingsMenu->exec();
}

void MainWindow::slot_aboutMenu()
{
    QMessageBox msgBox;
    msgBox.setText("Created by Youn Mélois.\nhttps://github.com/sehnryr/qt-game");
    msgBox.exec();
}