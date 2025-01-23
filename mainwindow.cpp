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
#include <QMessageBox>
#include <qapplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), player1(nullptr)  {
    // 创建控件
    startButton = new QPushButton("Start Game", this);
    QLabel *statusLabel = new QLabel("Game Status: Ready", this);


    mapDisplay = new QTextEdit(this);  // 创建 QTextEdit 控件用于显示地图
    mapDisplay->setReadOnly(true);

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
    startButton->setEnabled(can_create_new);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::initializeGame);

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

// 初始化游戏函数
void MainWindow::initializeGame() {
    can_create_new = false;
    startButton->setEnabled(can_create_new);
    // 创建玩家和武器
    Weapon* meleeWeapon = new Weapon(WeaponType::Melee, 1, 5, 10);  // 创建一个近战武器
    player1 = new Player("Warrior", 100, 20, 1, meleeWeapon, 1, 0, 0);  // 传递武器对象、伤害区域、持续时间、冷却时间等

    // 创建玩家状态标签
    playerStatusLabel = new QLabel("Player: Warrior | Level: 1 | Experience: 0", this);


    // 初始化地图，假设地图大小为 10x10
    gameMap = new Map(10, 10);

    // 随机生成至少 5 个障碍物
    gameMap->generateObstacles(5);

    // 创建武器对象
    // Weapon* rangedWeapon = new Weapon(WeaponType::Ranged, 3, 5, 5);  // 远程武器
    // player1 = new Player("Archer", 100, 15, 1, rangedWeapon, 2, 3, 10);  // 创建玩家

    gameMap->getCell(0, 0).cellType = CellType::Player;  // 将玩家放在左上角（0, 0）

    // 将地图转换为字符串并显示在 QTextEdit 中
    // updateMap(); // 感觉这一步没必要
    // 创建敌人
    // 创建两个敌人
    Enemy* enemy1 = new MeleeEnemy(8, 8);  // 近战敌人
    Enemy* enemy2 = new RangedEnemy(1, 1); // 远程敌人

    // 设置敌人 注意：在这之前敌人一直是空指针
    gameMap->setEnemy(enemy1, enemy2);

    gameLoop();
}

// 更新玩家状态的显示
void MainWindow::updatePlayerStatus(int experience, int level) {
    // 显示玩家的经验和等级
    playerStatusLabel->setText(QString("Player: Warrior | Level: %1 | Experience: %2")
                                   .arg(level)
                                   .arg(experience));
}


void MainWindow::gameLoop() {
    // 在这里实现游戏的每帧更新逻辑，例如更新玩家位置、敌人行为等
    // 以下是一个简单的示例：
    // 更新敌人的位置
    // 逐个更新敌人的位置和攻击玩家
    // 将地图转换为字符串并显示在 QTextEdit 中
    updateMap();


    for (int i = 0; i < 2; ++i) {
        Enemy* enemy = gameMap->getEnemy(i);
        if (enemy) {
            enemy->move(gameMap, player1);  // 让敌人向玩家移动
            enemy->attack(player1);  // 敌人攻击玩家
        }
    }

    if (player1) {
        player1->addExperience(10);  // 假设每次游戏循环，玩家获得经验
        updatePlayerStatus(player1->getExperience(), player1->getLevel());
    }

    // 检查当前关卡是否已完成
    bool allEnemiesDead = true;
    for (auto enemy : enemies) {
        if (enemy->getHealth() > 0) {
            allEnemiesDead = false;
            break;
        }
    }

    // 如果所有敌人都死了，提示游戏结束
    if (allEnemiesDead) {
        // 显示消息框
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Level Complete",
                                                                  "You have cleared all enemies! Do you want to continue to the next level?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            updateGame();
        } else {
            close();  // 玩家选择退出
        }
    }

    // 应该等上一个连接信号完成后才调用gameloop和定时器方法
    // 设置定时器，100毫秒调用一次 gameLoop 方法
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);
    timer->start(5000);  // 每 100 毫秒调用一次
}


void MainWindow::updateMap() { //绘制文本版本地图
    QString mapString = "";
    // 直接从 Map 中获取敌人
    for (int i = 0; i < gameMap->getRowCount(); ++i) {
        for (int j = 0; j < gameMap->getColCount(); ++j) {
            bool enemyFound = false;

            // 检查是否有敌人在当前位置
            for (int k = 0; k < 2; ++k) {  // 这里只有两个敌人
                Enemy* enemy = gameMap->getEnemy(k);
                if (enemy && enemy->getX() == j && enemy->getY() == i) {
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
    // 在文本框中更新地图显示
    mapDisplay->setText(mapString);

    // 更新玩家的状态信息
    updatePlayerStatus(player1->getExperience(), player1->getLevel());
}

void MainWindow::generateEnemiesForCurrentLevel(){
    // 清空当前敌人
    enemies.clear();

    // 根据关卡生成敌人
    int numEnemies = 2;  // 每关敌人数量随关卡数增加

    // 创建敌人
    for (int i = 0; i < numEnemies; ++i) {
        Enemy* enemy = gameMap->getEnemy(i); // 血量、攻击力逐步增加
    }
}


void MainWindow::updateGame(){
    // 继续下一关
    currentLevel++;
    if (currentLevel <= totalLevels) {
        generateEnemiesForCurrentLevel();  // 生成新的敌人
        updateMap();  // 更新地图
    } else {
        QMessageBox::information(this, "Game Over", "Congratulations! You've completed all levels!");
        close();  // 游戏结束，关闭窗口
    }
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
    // 玩家攻击敌人
    player1->attack(gameMap);  // 添加玩家攻击

    // 每次按键后，更新地图
    updateMap();
}






// MainWindow.cpp
void MainWindow::levelComplete() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Level Completed", "Do you want to enter the next level?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (currentLevel < totalLevels) {
            currentLevel++;  // 增加关卡
            // generateEnemiesForCurrentLevel();  // 生成新的敌人
            updateMap();  // 更新地图
        } else {
            QMessageBox::information(this, "Game Over", "You have completed all the levels!");
        }
    } else {
        // 结束游戏
        QApplication::quit();
    }
}



