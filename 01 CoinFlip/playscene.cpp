#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}

PlayScene::PlayScene(int levelNum)
{
    QString str = QString("进入了第 %1 关").arg(levelNum);
    qDebug() << str;
    this->levelIndex = levelNum;

    // 初始化游戏场景
    // 配置选择关卡场景
    this->setFixedSize(320, 588);

    // 设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    // 设置标题
    this->setWindowTitle("翻金币场景");

    // 手写创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    // 创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    // 创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");
    // 点击退出 实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    // 添加音效资源
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);

    // 返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(), this->height()-backBtn->height());

    // 点击返回
    connect(backBtn, &MyPushButton::clicked, [=](){
        qDebug() << "翻金币场景中：点击了返回按钮";
        // 告诉选择关卡场景：我返回了，选择关卡场景监听chooseLevelScene的返回按钮
        // 延时返回
        // 播放返回按钮音效
        backSound->play();
        QTimer::singleShot(500, this, [=](){
            emit this->chooseSceneBack();
        });
    });

    // 显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level：%1").arg(this->levelIndex);
    // 将字体设置到标签控件中
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30, this->height()-50, 120, 50);

    // 初始化每个关卡的二维数组
    dataConfig config;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    // 胜利图片展示
    QLabel * winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5, -tmpPix.height());

    // 显示背景金币图案
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            // 绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            QLabel * label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i*50, 200 + j*50);

            // 创建金币
            QString str;
            if(this->gameArray[i][j] == 1) {
                // 显示金币
                str = ":/res/Coin0001.png";
            }
            else {
                str = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50, 204 + j*50);

            // 给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j]; // 1正面 0反面

            // 将金币放入到金币的二维数组中 以便后期的维护
            coinBtn[i][j] = coin;

            // 点击金币，进行翻转
            connect(coin, &MyCoin::clicked, [=](){
                // 播放翻金币音效
                flipSound->play();
                // 点击按钮，将所有按钮先禁用
                for(int i=0;i<4;i++){
                    for(int j=0;j<4;j++){
                        this->coinBtn[i][j]->isWin = true;
                    }
                }
                coin->changeFlag();
                this->gameArray[i][j] == this->gameArray[i][j] == 0 ? 1 : 0;

                // 翻转周围金币
                QTimer::singleShot(300, this, [=](){
                    if(coin->posX+1 <= 3) {  // 周围的右侧金币翻转的条件
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posX-1 >= 0) {  // 周围的左侧金币翻转的条件
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY] == 0 ? 1 : 0;
                    }
                    if(coin->posY-1 >= 0) {  // 周围的上侧金币翻转的条件
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1] == 0 ? 1 : 0;
                    }
                    if(coin->posY+1 <= 3) {  // 周围的下侧金币翻转的条件
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1] == 0 ? 1 : 0;
                    }

                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }

                    // 判断是否胜利
                    this->isWin = true;
                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            if(coinBtn[i][j]->flag==false) {    // 只要有一个失败，那就失败
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin==true) {
                        // 添加胜利音效
                        winSound->play();
                        qDebug() << "游戏胜利！";
                        // 将所有胜利的游戏标志改为true。如果再次点击，直接return，不做响应
                        for(int i=0;i<4;i++){
                            for(int j=0;j<4;j++){
                                this->coinBtn[i][j]->isWin = true;
                            }
                        }

                        // 将胜利的图片砸下来
                        QPropertyAnimation * animation = new QPropertyAnimation(winLabel, "geometry");
                        // 设置动画时间间隔
                        animation->setDuration(1000);
                        // 起始位置
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        // 结束位置
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y()+114, winLabel->width(), winLabel->height()));
                        // 设置弹跳曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        // 执行动画
                        animation->start();
                    }
                });
            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *)
{
    // 创建画家，指定绘图设备
    QPainter painter(this);
    // 创建QPixmap对象
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    // 画背景上图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5, pix.height()*0.5);
    painter.drawPixmap(10, 30, pix.width(), pix.height(), pix);
}
