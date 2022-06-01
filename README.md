## Game Preview

<details>
  <summary>Preview</summary>
  
![image](https://user-images.githubusercontent.com/40632486/171402683-225855b7-d595-4100-9ed5-b2c842ce5df5.png)
![image](https://user-images.githubusercontent.com/40632486/171412337-6bde64aa-c176-48ea-88bf-5400e7ef7877.png)
![image](https://user-images.githubusercontent.com/40632486/171412400-14c3a87a-eb18-4ffd-a6b7-f9359f93c73a.png)
![image](https://user-images.githubusercontent.com/40632486/171412517-9f62b4a8-0ff7-4386-8efe-1244f2846277.png)
![image](https://user-images.githubusercontent.com/40632486/171412555-a73d2527-a7bf-4b4b-8531-dfeeeb3e8d45.png)
</details>

## Settings

![image](https://user-images.githubusercontent.com/40632486/171413071-f987ce67-a49c-468c-b522-c3fb2c860f19.png)

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

If it doesn't compile, you might have those missing in your `.pro` file:
```pro
QT += gui core widgets multimedia
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
