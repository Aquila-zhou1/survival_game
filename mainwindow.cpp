#include "mainwindow.h"
#include "map.h"  // 引入 Map 类
#include "player.h"
#include "enemy.h"    // 引入 Enemy 类
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), player1(nullptr)  {
    // 创建控件
    QPushButton *startButton = new QPushButton("Start Game", this);
    QLabel *statusLabel = new QLabel("Game Status: Ready", this);
    mapDisplay = new QTextEdit(this);  // 创建 QTextEdit 控件用于显示地图
    mapDisplay->setReadOnly(true);
    // 创建玩家和武器
    //Weapon *meleeWeapon = new Weapon(WeaponType::Melee, 1, 5, 10);
    WeaponType player1_weapon = WeaponType::Melee;  // 设定玩家的武器类型为近战

    // 使用新的构造函数初始化玩家
    player1 = new Player("Warrior", 100, 20, 1, player1_weapon, 1, 0, 0);  // 传递武器类型、伤害区域、持续时间、冷却时间等

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(startButton);
    layout->addWidget(statusLabel);
    layout->addWidget(mapDisplay);

    // 创建一个中央部件，设置布局
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 连接信号与槽
    connect(startButton, &QPushButton::clicked, this, &MainWindow::initializeGame);
    // // 设置定时器，100毫秒调用一次 gameLoop 方法
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);
    timer->start(5000);  // 每 100 毫秒调用一次

}

MainWindow::~MainWindow() {
    delete player1;
    delete gameMap;
    for (auto enemy : enemies) {
        delete enemy;
    }
    // 手动清理资源，如果有必要
    // 不需要显式删除布局或控件，因为它们的父对象会自动清理
}

// 处理键盘按下事件
void MainWindow::keyPressEvent(QKeyEvent *event) {
    // 先清除当前玩家的位置
    for (int i = 0; i < gameMap->getRowCount(); ++i) {
        for (int j = 0; j < gameMap->getColCount(); ++j) {
            if (gameMap->getCell(i, j).cellType == CellType::Player) {
                gameMap->getCell(i, j).cellType = CellType::Normal;  // 清除玩家当前位置
            }
        }
    }

    // 根据按键更新玩家的位置
    if (event->key() == Qt::Key_W) {
        player1->move(0, -1, gameMap);  // 传递 gameMap
    }
    else if (event->key() == Qt::Key_S) {
        player1->move(0, 1, gameMap);   // 传递 gameMap
    }
    else if (event->key() == Qt::Key_A) {
        player1->move(-1, 0, gameMap);  // 传递 gameMap
    }
    else if (event->key() == Qt::Key_D) {
        player1->move(1, 0, gameMap);   // 传递 gameMap
    }

    // 更新玩家在 gameMap 中的位置
    int playerX = player1->getX();
    int playerY = player1->getY();
    gameMap->getCell(playerY, playerX).cellType = CellType::Player;  // 更新玩家的新位置

    // 每次按键后，更新地图
    updateMap();
}



// 你的初始化游戏函数
void MainWindow::initializeGame() {
    // 初始化地图，假设地图大小为 10x10
    gameMap = new Map(10, 10);

    // 随机生成至少 5 个障碍物
    gameMap->generateObstacles(5);

    // 选择玩家角色
    player1 = new Player("Warrior", 100, 20, 1, WeaponType::Melee, 1, 0, 0);  // 近战武器

    gameMap->getCell(0, 0).cellType = CellType::Player;  // 将玩家放在左上角（0, 0）

    // 将地图转换为字符串并显示在 QTextEdit 中
    updateMap();
    // 创建敌人
    enemies.push_back(new MeleeEnemy(8, 8));  // 创建一个近战敌人
    enemies.push_back(new RangedEnemy(1, 1)); // 创建一个远程敌人
}

void MainWindow::updateMap() {
    QString mapString = "";
    for (int i = 0; i < gameMap->getRowCount(); ++i) {
        for (int j = 0; j < gameMap->getColCount(); ++j) {
            bool enemyFound = false;
            for (auto enemy : enemies) {
                if (enemy->getX() == j && enemy->getY() == i) {
                    if (enemy->getType() == EnemyType::Melee) {
                        mapString += " M ";  // 近战敌人
                    } else if (enemy->getType() == EnemyType::Ranged) {
                        mapString += " R ";  // 远程敌人
                    }
                    mapString += QString("(%1) ");  // 显示敌人生命值
                    mapString += QString::number(enemy->getHealth());
                    enemyFound = true;
                    break;
                }
            }
            if (!enemyFound) {
                if (gameMap->getCell(i, j).cellType == CellType::Normal) {
                    mapString += " . ";  // 普通格子
                } else if (gameMap->getCell(i, j).cellType == CellType::Obstacle) {
                    mapString += " # ";  // 障碍物
                } else if (gameMap->getCell(i, j).cellType == CellType::Player) {
                    mapString += " P ";  // 玩家
                    mapString += QString("(%1) ");  // 显示玩家生命值
                    mapString += QString::number(player1->getHealth());  // 显示玩家生命值
                }
            }
        }
        mapString += "\n";  // 换行
    }

    // 设置 QTextEdit 显示地图
    mapDisplay->setText(mapString);
}

void MainWindow::gameLoop() {
    // 更新敌人的位置
    for (auto enemy : enemies) {
        enemy->move(gameMap, player1);  // 每个敌人都向玩家方向移动
        enemy->attack(player1);  // 每个敌人都攻击玩家
    }

    // 更新地图
    updateMap();
}
