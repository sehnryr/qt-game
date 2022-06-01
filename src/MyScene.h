#ifndef CPP_QT_MY_SCENE_H
#define CPP_QT_MY_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "Player.h"
#include "Box.h"
#include "PlayerView.h"
#include "PauseMenu.h"
#include "StartMenu.h"
#include "RetryMenu.h"
#include "ClearObjectiveMenu.h"
#include "SaveScoreMenu.h"
#include "Stage.h"

class MyScene : public QGraphicsScene
{
    Q_OBJECT
private:
    QTimer *_timer;
    QPixmap _arrow;
    qreal *_gravity;
    qreal *_jumpHeight;
    QMediaPlayer *_musicPlayer;

    PauseMenu *_pauseMenu;
    StartMenu *_startMenu;
    RetryMenu *_retryMenu;
    ClearObjectiveMenu *_clearObjectiveMenu;
    SaveScoreMenu *_saveScoreMenu;
    Stage *_stage;

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    qreal time() const;
    int timeLimit() const { return this->stage()->timeLimit(); }
    QPixmap arrow() const { return _arrow; }
    qreal *gravity() const { return _gravity; }
    qreal *jumpHeight() const { return _jumpHeight; }
    QMediaPlayer *musicPlayer() const { return _musicPlayer; }
    QLineF playerObjectiveLine() const;

    PauseMenu *pauseMenu() const { return _pauseMenu; }
    StartMenu *startMenu() const { return _startMenu; }
    RetryMenu *retryMenu() const { return _retryMenu; }
    ClearObjectiveMenu *clearObjectiveMenu() const { return _clearObjectiveMenu; }
    SaveScoreMenu *saveScoreMenu() const { return _saveScoreMenu; }
    Stage *stage() const { return _stage; }

    Box *objective() const { return stage()->objective(); }
    Player *player() const { return stage()->player(); }
    QGraphicsRectItem *leftWall() const { return stage()->leftWall(); }
    QGraphicsRectItem *rightWall() const { return stage()->rightWall(); }

    void updateVolume();

public:
    MyScene(qreal *gravity, qreal *jumpHeight, QObject *parent = nullptr);
    virtual ~MyScene();

    void drawForeground(QPainter *painter, const QRectF &rect);

    qreal getGravity() const { return *gravity(); }
    qreal getJumpHeight() const { return *jumpHeight(); }

    QTimer *timer() const { return _timer; }
    PlayerView *playerView() const { return stage()->playerView(); }

public slots:
    void update();
    void restart(QString reason);
};

#endif // CPP_QT_MY_SCENE_H
