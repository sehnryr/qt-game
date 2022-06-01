#ifndef CPP_QT_MAIN_WINDOW_H
#define CPP_QT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QAction>
#include <QSlider>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "MyScene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    MyScene *mainScene;
    QMenu *helpMenu;
    qreal *gravity;
    qreal *jumpHeight;
    QSlider sliderGravity;
    QSlider sliderJumpHeight;
    QLabel labelGravityValue;
    QLabel labelJumpHeightValue;
    QDialog *settingsMenu;

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() {}

    qreal getGravity() const { return *gravity; }
    void setGravity(qreal const value) { *gravity = value; }

    qreal getJumpHeight() const { return *jumpHeight; }
    void setJumpHeight(qreal const value) { *jumpHeight = value; }

public slots:
    void slot_settingsMenu();
    void slot_aboutMenu();
};

#endif // CPP_QT_MAIN_WINDOW_H
