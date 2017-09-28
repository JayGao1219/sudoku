#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QTime>
#include <QPainter>
#include <QMouseEvent>
#include <QUrl>
#include <QtMultimedia>
#include <vector>
#include <QSignalMapper>
#include <QComboBox>

#include "generate.h"
#include "mylabel.h"

class Dialog : public QDialog
{
    Q_OBJECT
    //buttons
    QPushButton *number[10];//to have the number button
    QPushButton *replay;
    QPushButton *pause;
    QPushButton *help;
    QPushButton *next;
    QPushButton *last;
    QPushButton *mark;
    QPushButton *del;
    QPushButton *music;
    QPushButton *choose;

    //time
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *);
    int pauseClicked;

    //draw the background
    bool shower[81];
    int grids[81];
    sudoku sudo;
    int getColor(int,int);

    //change the state
    int time;
    int state;
    int playTimes;
    bool blankClicked;
    bool ifBlankClicked[81];
    bool nonBlankClicked;
    //play the music
    QMediaPlayer *player;

    //fill the keyboard
    MyLabel *cells[81];
    void getGrids();
    void fillGrids();
    void fillNumber();

    //set the level
    int level;
    void setLevel(int);
    bool isFinshed;

    QSignalMapper *signalMapper;
    QSignalMapper *aa;
    QSignalMapper *mm;

    //realize the undo and redo
    std::vector<std::vector<int>>history;
    int currentPoint;
    int customFill[81];
    //1 for delete
    //2 for fill
    //the second of the map means the 0th grids fill/delete 1th number

    //choose the level
    QComboBox *comBox;

private slots:
    void playMusic();
    void pauseIt();
    void replayIt();
    void gridsClick(int);
    void numberCLick(int);
    void gridDelete();
    void markGrid();
    void helpMe();
    void undo();
    void redo();
    void chooseLevel();

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

};

#endif // DIALOG_H
