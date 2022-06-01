If it doesn't compile, you might have those missing in your `.pro` file:
```pro
QT += gui core widgets multimedia
```

## Debian 11

### Setup

apt packages:
```
sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtmultimedia5-dev -y
```

## Fedora 36

### Setup

dnf packages:
```
sudo dnf install qt5-qtbase-devel qt5-qtmultimedia-devel -y
```

Compiling:
```sh
cd directory/
qmake-qt5 -project # make sure to add QT field in the generated .pro file 
qmake-qt5
make
```

### Errors

If this error shows:
```stdout
qt.qpa.wayland: Wayland does not support QWindow::requestActivate()
```

Do this to fix it:
```bash
export QT_QPA_PLATFORM=xcb
```

---

If `qDebug` doesn't print, edit `/usr/share/qt5/qtlogging.ini` and change
`*.debug=` value to `true`.
```ini
[Rules]
*.debug=true
```

## GamePlay

You have one objective : capturing the flag that makes music in the level.
To direct you, there is an arrow on the side of the frame when the objective is
not showing on your screen. There is also a music that's playing in the
background and which volume will increase when you approach the objective.

You can move with the side arrows, and `A` & `D`.
Jump with the space bar or the up arrow of `W`, and sprint with `CTRL`.

## `stage.txt` file

This is the "Mario Maker" of the game. Tweaking this file will permit you to
make unique stages with unique gravity or jump height. You can make a level 
where you are on the Moon or one where you are on Mars.

First line : **Time limit**, **Gravity**, **Jump height**
```
0,25,2.2
```
|    Name     |                   Values                   |
| :---------: | :----------------------------------------: |
| Time limit  | no limit if value is `0`. Value in seconds |
|   Gravity   |     [1,100] *[one decimal precision]*      |
| Jump height |      [1,10] *[one decimal precision]*      |

Second to M lines : Stored scores (starting with an `#`) : **Player name**, **Time score**
```
#Youn,42.69
```
|    Name     |              Values               |
| :---------: | :-------------------------------: |
| Player name |              string               |
| Time score  | seconds *[two decimal precision]* |

M+1 to N lines : Stage structure
```
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
                        X               
                 XX     X               
XXX  XXX         XX     X       XXXXX   
         XXX            X       X       
 P       X     X        XX   XXXX O    X
XXXXXXX  X     XX               XXXXXXXX
         X           X      X   X       
        XX    X                 X       
  XXXXXXXX   XX        XXXX    XXXXXX   
                    X                   
X          X    X   XX       X         X
XXXXXXXXXXXXXXXXX   XXXXXXXXXXXXXXXXXXXX
```
| Bloc indicator |               Stage object                |
| :------------: | :---------------------------------------: |
|      `X`       |                   Bloc                    |
|      `P`       | Player spawn point *(empty space on top)* |
|      `O`       |     Objective *(empty space on top)*      |

## Disclaimer

The music used in the game is from this youtube video 
: https://youtu.be/iVzXq2-qkz0