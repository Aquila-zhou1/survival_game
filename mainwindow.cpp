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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), player1(nullptr), gameTimer(new QTimer(this))  {
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
    connect(startButton, &QPushButton::clicked, this, &MainWindow::start_setting);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::gameLoop);

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


void MainWindow::start_setting(){
    bool need_archive = false;
    QMessageBox::StandardButton start_begin = QMessageBox::question(this, "Hello!", "Do you want to load the archive?",
                                                                    QMessageBox::Yes | QMessageBox::No);
    if(start_begin == QMessageBox::Yes){
        need_archive = true;
    }
    initializeGame(need_archive);
}

// 初始化游戏函数
void MainWindow::initializeGame(bool need_archive) {
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
    enemies.append(enemy1);
    enemies.append(enemy2);

    if(need_archive){
        loadArchive(enemy1, enemy2);
    }
    // 设置敌人 注意：在这之前enemies一直为空
    gameMap->setEnemy(enemy1, enemy2);
    gameTimer->start(500);  // 每 100 毫秒调用一次 gameLoop

    // gameLoop();
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
        gameTimer->stop();  // 停止定时器
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
    // else{
    //     // 应该等上一个连接信号完成后才调用gameloop和定时器方法
    //     // 设置定时器，100毫秒调用一次 gameLoop 方法
    //     QTimer *timer = new QTimer(this);
    //     connect(timer, &QTimer::timeout, this, &MainWindow::gameLoop);
    //     timer->start(5000);  // 每 100 毫秒调用一次
    // }

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


void MainWindow::updateGame(){
    // 继续下一关
    currentLevel++;
    saveArchive();      // 保存存档
    if (currentLevel <= totalLevels) {
        generateEnemiesForCurrentLevel();  // 生成新的敌人
        initializeGame();
    } else {
        QMessageBox::information(this, "Game Over", "Congratulations! You've completed all levels!");
        close();  // 游戏结束，关闭窗口
    }
}


void MainWindow::generateEnemiesForCurrentLevel(){
    // 清空当前敌人
    enemies.clear();
    delete player1;
    delete gameMap;
    // 根据关卡生成敌人
    int numEnemies = 2;  // 每关敌人数量随关卡数增加

    // 创建敌人
    for (int i = 0; i < numEnemies; ++i) {
        Enemy* enemy = gameMap->getEnemy(i); // 血量、攻击力逐步增加
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


void MainWindow::saveArchive() {
    QJsonObject gameState;

    // 保存当前关卡
    gameState["currentLevel"] = currentLevel;

    // 保存玩家数据
    QJsonObject playerData;
    playerData["name"] = player1->getName();
    playerData["health"] = player1->getHealth();
    playerData["experience"] = player1->getExperience();
    playerData["level"] = player1->getLevel();
    QJsonArray playerPosition = {player1->getX(), player1->getY()};
    playerData["position"] = playerPosition;

    QJsonObject weaponData;
    weaponData["type"] = player1->getWeapon()->getType() == WeaponType::Melee ? "Melee" : "Ranged";
    weaponData["damageArea"] = player1->getWeapon()->getDamageArea();
    weaponData["duration"] = player1->getWeapon()->getDuration();
    // weaponData["cooldown"] = player1->getWeapon()->getCooldown();
    playerData["weapon"] = weaponData;

    gameState["player"] = playerData;

    // 保存地图数据
    QJsonObject mapData;
    mapData["rows"] = gameMap->getRowCount();
    mapData["cols"] = gameMap->getColCount();

    // 保存障碍物
    QJsonArray obstacles;
    for (const auto& obstacle : gameMap->getObstacles()) {
        QJsonArray obstaclePos = {obstacle.first, obstacle.second};
        obstacles.append(obstaclePos);
    }
    mapData["obstacles"] = obstacles;

    // 保存敌人数据
    QJsonArray enemiesData;
    for (int i = 0; i < enemies.size(); ++i) {
        QJsonObject enemyData;
        enemyData["type"] = enemies[i]->getType() == EnemyType::Melee ? "Melee" : "Ranged";
        QJsonArray enemyPosition = {enemies[i]->getX(), enemies[i]->getY()};
        enemyData["position"] = enemyPosition;
        enemyData["health"] = 100;
        enemiesData.append(enemyData);
    }
    mapData["enemyPositions"] = enemiesData;

    gameState["map"] = mapData;

    // 写入 JSON 文件
    QFile file("archive.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(gameState);
        file.write(doc.toJson());
        file.close();
    }
    qDebug() << "write successfully!";
}


void MainWindow::loadArchive(Enemy*& enemy1, Enemy*& enemy2) {
    QFile file("archive.json");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Load Error", "No save file found.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject gameState = doc.object();

    // 加载当前关卡
    currentLevel = gameState["currentLevel"].toInt();

    // 加载玩家数据
    QJsonObject playerData = gameState["player"].toObject();
    player1->setName(playerData["name"].toString());
    player1->setHealth(playerData["health"].toInt());
    player1->setExperience(playerData["experience"].toInt());
    player1->setLevel(playerData["level"].toInt());

    QJsonArray playerPosition = playerData["position"].toArray();
    player1->setPosition(playerPosition[0].toInt(), playerPosition[1].toInt());

    QJsonObject weaponData = playerData["weapon"].toObject();
    WeaponType weaponType = weaponData["type"].toString() == "Melee" ? WeaponType::Melee : WeaponType::Ranged;
    player1->setWeapon(new Weapon(weaponType, weaponData["damageArea"].toInt(), weaponData["duration"].toInt(), 5));

    // 加载地图数据
    QJsonObject mapData = gameState["map"].toObject();
    delete gameMap;
    gameMap = new Map(mapData["rows"].toInt(), mapData["cols"].toInt());

    // 加载障碍物
    QJsonArray obstacles = mapData["obstacles"].toArray();
    for (const auto& obstacle : obstacles) {
        QJsonArray pos = obstacle.toArray();
        gameMap->addObstacle(pos[0].toInt(), pos[1].toInt());
    }

    // 加载敌人数据
    enemies.clear();


    QJsonArray enemiesData = mapData["enemyPositions"].toArray();
    for (const auto& enemy : enemiesData) {
        QJsonObject enemyData = enemy.toObject();
        EnemyType enemyType = enemyData["type"].toString() == "Melee" ? EnemyType::Melee : EnemyType::Ranged;
        if(enemyType == EnemyType::Melee){
            delete enemy1;
            enemy1 = new MeleeEnemy(1,1);
            QJsonArray pos = enemyData["position"].toArray();
            enemy1->setPosition(pos[0].toInt(), pos[1].toInt());
            enemy1->setHealth(enemyData["health"].toInt());

            enemies.append(enemy1);
        }
        else{
            delete enemy2;
            enemy2 = new RangedEnemy(8,8);
            QJsonArray pos = enemyData["position"].toArray();
            enemy2->setPosition(pos[0].toInt(), pos[1].toInt());
            enemy2->setHealth(enemyData["health"].toInt());

            enemies.append(enemy2);
        }
    }
}


