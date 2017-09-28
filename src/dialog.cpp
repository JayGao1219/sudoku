#include "dialog.h"

#include <QDebug>
#include <ctime>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
   resize(300,300);
   currentPoint=-1;
   signalMapper=new QSignalMapper(this);
   aa=new QSignalMapper(this);
   mm=new QSignalMapper(this);
   for(int i=1;i<10;++i){
       number[i]=new QPushButton(this);
       QString str;
       str=QString(":/number/%1.ico").arg(i);
       QIcon ico(str);
       number[i]->setIcon(ico);
       number[i]->setFlat(true);
       number[i]->setGeometry(i*30-8,260,18,18);
       connect(number[i],SIGNAL(clicked()),aa,SLOT(map()));
       aa->setMapping(number[i],i);
       connect(aa,SIGNAL(mapped(int)),this,SLOT(numberCLick(int)));
   }
   level=1;
   getGrids();
   fillGrids();
   setLevel(level);
   //get the buttons
   replay=new QPushButton(this);
   QIcon rp(":/image/replay.png");
   replay->setIcon(rp);
   replay->setFlat(true);
   replay->setGeometry(10,10,18,18);
   connect(replay,SIGNAL(clicked()),this,SLOT(replayIt()));

   last=new QPushButton(this);
   QIcon l(":/image/last.png");
   last->setIcon(l);
   last->setFlat(true);
   last->setGeometry(35,10,18,18);
   connect(last,SIGNAL(clicked()),this,SLOT(undo()));

   next=new QPushButton(this);
   QIcon n(":image/next.png");
   next->setIcon(n);
   next->setFlat(true);
   next->setGeometry(60,10,18,18);
   connect(next,SIGNAL(clicked()),this,SLOT(redo()));

   pause=new QPushButton(this);
   QIcon p(":image/pause.png") ;
   pause->setIcon(p);
   pause->setFlat(true);
   pause->setGeometry(85,10,18,18);
   connect(pause,SIGNAL(clicked()),this,SLOT(pauseIt()));

   mark=new QPushButton(this);
   QIcon m(":image/mark.ico");
   mark->setIcon(m);
   mark->setFlat(true);
   mark->setGeometry(108,10,18,18);
   connect(mark,SIGNAL(clicked()),this,SLOT(markGrid()));

   del=new QPushButton(this);
   QIcon dd(":image/delete.png");
   del->setIcon(dd);
   del->setFlat(true);
   del->setGeometry(130,10,18,18);
   connect(del,SIGNAL(clicked()),this,SLOT(gridDelete()));

   comBox=new QComboBox(this);
   comBox->setGeometry(150,12,40,18);
   for(int i=1;i<11;++i){
       comBox->addItem(QString("%1").arg(i));
   }

   choose=new QPushButton(this);
   QIcon cc(":image/choose.png");
   choose->setIcon(cc);
   choose->setFlat(true);
   choose->setGeometry(190,10,18,18);
   connect(choose,SIGNAL(clicked()),this,SLOT(chooseLevel()));

   music=new QPushButton(this);
   QIcon mm(":image/music.png");
   music->setIcon(mm);
   music->setFlat(true);
   music->setGeometry(250,10,18,18);
   connect(music,SIGNAL(clicked()),this,SLOT(playMusic()));

   help=new QPushButton(this);
   QIcon h(":/image/help.png");
   help->setIcon(h);
   help->setFlat(true);
   help->setGeometry(275,10,18,18);
   connect(help,SIGNAL(clicked()),this,SLOT(helpMe()));

   startTimer(1000);
   time=0;
   state=0;
   pauseClicked=0;
   blankClicked=false;
   nonBlankClicked=false;
   std::memset(ifBlankClicked,0,sizeof(ifBlankClicked));
   std::memset(customFill,0,sizeof(customFill));
   //0, game is not begin
   //1, game is on
   //2, game is pause
   //3, game is restart
   playTimes=0;
   player=new QMediaPlayer();
}

void Dialog::undo(){
    if(currentPoint<0)
        return;
    for(int i=0;i<81;++i){
        cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
    }
    switch (history[currentPoint][0]) {
    case 1:
        for(int i=0;i<history[currentPoint].size()/2;++i){
            cells[history[currentPoint][2*i+1]]->setText(QString("%1").arg(history[currentPoint][2*i+2]));
            customFill[history[currentPoint][2*i+2]]=history[currentPoint][2*i+2];
        }
        currentPoint--;
        break;
    case 2:
        for(int i=0;i<history[currentPoint].size()/2;++i){
            cells[history[currentPoint][2*i+1]]->setText(QString(""));
            customFill[history[currentPoint][2*i+2]]=0;
        }
        currentPoint--;
        break;
    default:
        break;
    }
}

void Dialog::redo(){
    if(currentPoint<history.size()){
        return;
    }
    for(int i=0;i<81;++i){
        cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
    }
    switch (history[++currentPoint][0]) {
    case 2:

        for(int i=0;i<history[currentPoint].size()/2;++i){
            cells[history[currentPoint][2*i+1]]->setText(QString("%1").arg(history[currentPoint][2*i+2]));
            customFill[history[currentPoint][2*i+2]]=history[currentPoint][2*i+2];
        }
        break;
    case 1:

        for(int i=0;i<history[currentPoint].size()/2;++i){
            cells[history[currentPoint][2*i+1]]->setText(QString(""));
            customFill[history[currentPoint][2*i+2]]=0;
        }
        break;
    }
}

void Dialog::numberCLick(int clicked){
    if(blankClicked==false)
        return;
//    blankClicked=false;
    if(history.size()>currentPoint+1)
        history.erase(history.begin()+1+currentPoint);
    std::vector<int>a;
    a.push_back(2);
    for(int i=0;i<81;++i){
        if(ifBlankClicked[i]){
            QString str=cells[i]->text();
            if(str.indexOf(QString::number(clicked))==-1)
                str=str+QString("%1").arg(clicked);
            cells[i]->setText(str);
            a.push_back(i);
            a.push_back(clicked);
            customFill[i]=clicked;
        }
        cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
    }
    ++currentPoint;
    history.push_back(a);
}

void Dialog::fillGrids(){
    for(int i=0;i<9;++i){
        for(int j=0;j<9;++j){
            cells[i*9+j]=new MyLabel(this);
            cells[i*9+j]->setGeometry(50+j*22,45+i*22,22,22);
            connect(cells[i*9+j],SIGNAL(clicked()),signalMapper,SLOT(map()));
            signalMapper->setMapping(cells[i*9+j],i*9+j);
            cells[i*9+j]->setAlignment(Qt::AlignCenter);
        }
    }
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(gridsClick(int)));
}

void Dialog::gridsClick(int clicked){
    if(state!=1)
        return;
    int a=grids[clicked];
    if(shower[clicked]==true){
        nonBlankClicked=true;
        std::memset(ifBlankClicked,0,sizeof(ifBlankClicked));
        for(int i=0;i<81;++i){
            if(shower[i]==true){
                if(grids[i]==a)
                    cells[i]->setStyleSheet("QLabel{background-color:gray;}");
                else
                    cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
            }
            else
                cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
        }
        return;
    }
    ifBlankClicked[clicked]=true;
    blankClicked=true;
    if(nonBlankClicked==true){
        nonBlankClicked=false;
        for(int i=0;i<81;++i){
            cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
        }
    }
    int y=clicked/9;
    int x=clicked-9*y;
    for(int i=0;i<9;++i){
        cells[i+y*9]->setStyleSheet("QLabel{background-color:gray;}");
        cells[i*9+x]->setStyleSheet("QLabel{background-color:gray;}");
   }
}

void Dialog::gridDelete(){
    if(!blankClicked)
        return;
    blankClicked=false;
    if(history.size()>currentPoint+1)
        history.erase(history.begin()+1+currentPoint);
    std::vector<int>a;
    a.push_back(1);
    for(int i=0;i<81;++i){
        if(ifBlankClicked[i]){
            ifBlankClicked[i]=false;
            cells[i]->setText(QString(""));
            a.push_back(i);
            a.push_back(customFill[i]);
            customFill[i]=0;
        }
        cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
    }
    history.push_back(a);
    currentPoint++;

}

void Dialog::fillNumber(){
    for(int i=0;i<9;++i)
        for(int j=0;j<9;++j)
            if(shower[i*9+j])
                cells[i*9+j]->setText(QString("%1").arg(grids[i*9+j]));
}

void Dialog::getGrids(){
    sudo.map();
    int *a=sudo.get();
    for(int i=0;i<81;++i){
        grids[i]=*(a+i);
    }
}

void Dialog::setLevel(int l){
    memset(shower,true,sizeof(shower));
    bool *b=sudo.getLevel(l);
    for(int i=0;i<81;++i){
        shower[i]=*(b+i);
    }
    getGrids();
    replayIt();
}

void Dialog::chooseLevel(){
    int a=(comBox->currentText()).toInt();
    setLevel(a);
}

void Dialog::playMusic(){
    ++playTimes;
    player->setMedia(QUrl::fromLocalFile("/Users/mac/Desktop/qtProject/src/music/1.mp3"));
    player->setVolume(100);
    player->play();
    if(playTimes%2==0){
        player->stop();
    }
}

void Dialog::pauseIt(){
    if(pauseClicked==0){
        ++pauseClicked;
        state=1;
        fillNumber();
        return;
    }
    ++pauseClicked;
    if(pauseClicked%2==0)
        state=2;
    else
        state=1;
}

void Dialog::replayIt(){
    state=3;
    for(int i=0;i<81;++i){
        cells[i]->setStyleSheet("QLabel{background-color:transparent;}");
        if(!shower[i])
            cells[i]->setText(QString(""));
    }
    time=0;
    state=0;
    pauseClicked=0;
    blankClicked=false;
    nonBlankClicked=false;
    std::memset(ifBlankClicked,0,sizeof(ifBlankClicked));
    history.clear();
    currentPoint=-1;

}

void Dialog::timerEvent(QTimerEvent *event){
    if(state==1)
        ++time;
    bool flag=true;
    for(int i=0;i<81;++i){
        if(shower[i]==false&&customFill[i]!=grids[i]){
            flag=false;
            break;
        }
    }
    if(flag){
        setLevel(++level);
        return;
    }
    flag=true;
    update();
}

void Dialog::markGrid(){
    if(blankClicked==false)
        return;
    for(int i=0;i<81;++i){
        if(ifBlankClicked[i]){
            cells[i]->setStyleSheet("QLabel{background-color:blue;}");
        }
        else
            cells[i]->setStyleSheet("QLabel{backgrouond-color:transparent;}");
    }
}

void Dialog::helpMe(){
    for(int i=0;i<81;++i){
        if(shower[i]==0){
            if(customFill[i]!=grids[i]){
                cells[i]->setText(QString("%1").arg(grids[i]));
                cells[i]->setStyleSheet("QLabel{background-color:blue;}");
                customFill[i]=grids[i];
                return;
            }
        }
    }
}

void Dialog::paintEvent(QPaintEvent *){
    //draw the clock
    QPainter painter(this);
    QFont font(tr("微软雅黑"),15,QFont::Normal,false);
    painter.setFont(font);
    QRect r(210,10,36,18);
    painter.drawText(r,Qt::AlignCenter,QString::number(time));
    //draw the board
    QPen pen;
    pen.setBrush(Qt::gray);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    for(int i=0;i<10;++i){
        for(int j=0;j<2;++j){
            if(i%3==0)
                pen.setWidth(5);
            else
                pen.setWidth(3);

            if(j==0){//draw the vertical
                painter.setPen(pen);
                painter.drawLine(i*22+50,45,i*22+50,243);
            }
            else{
                painter.setPen(pen);
                painter.drawLine(50,i*22+45,248,i*22+45);
            }
        }
    }
}

Dialog::~Dialog()
{

}
