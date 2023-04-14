#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNum);

    int levelIndex; // 内部成员属性，记录所选的关卡

    // 重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    // 设计一个二维数组，维护每个关卡的具体数据
    int gameArray[4][4];

    MyCoin * coinBtn[4][4];

    // 是否胜利
    bool isWin;

signals:
    // 写一个自定义信号
    void chooseSceneBack();

public slots:
};

#endif // PLAYSCENE_H
