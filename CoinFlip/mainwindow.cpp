#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QSound>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 配置主场景

    // 设置固定大小
    setFixedSize(320, 588);

    // 设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 设置标题
    setWindowTitle("翻金币主场景");

    // 退出按钮实现
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });

    // 准备开始按钮的音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav", this);
    startSound->setLoops(10);// -1，无限循环播放
    startSound->play();

    // 开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5 - startBtn->width()*0.5, this->height()*0.7);

    // 实例化选择关卡的场景
    chooseScene = new ChooseLevelScene;

    // 监听选择关卡的返回按钮的信号
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, this, [=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide(); // 将选择关卡场景隐藏掉
        this->show(); // 重新显示主场景
    });

    connect(startBtn, &MyPushButton::clicked, [=](){
        qDebug() << "点击了开始按钮";
        // 播放开始音效资源
        startSound->play();

        // 做一个弹起的特效
        startBtn->zoom1();
        startBtn->zoom2();

        // 延时进入选择关卡场景
        QTimer::singleShot(500, this, [=](){
            // 设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());
            // 自身隐藏
            this->hide();
            // 显示选择关卡场景
            chooseScene->show();


        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    // 创建画家，指定绘图设备
    QPainter painter(this);
    // 创建QPixmap对象
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    // 画背景上图标
    pix.load(":/res/Title.png");
    // 缩放图片
    pix = pix.scaled(pix.width()*0.5, pix.height()*0.5);
    painter.drawPixmap(10, 30, pix);
}
