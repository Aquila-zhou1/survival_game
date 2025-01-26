#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include "map.h"
#include "enemy.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QDebug>

// 动画精灵函数
void createAnimatedSprite(QGraphicsScene* scene, const QString& spriteSheetPath, int frameCount, int frameWidth, int frameHeight, int x, int y, QGraphicsPixmapItem** item);

class MapDisplay : public QGraphicsView {
    Q_OBJECT

public:
    // 构造函数不再直接接收 gameMap，而是通过 renderMap 方法传入
    MapDisplay(QWidget *parent = nullptr)
        : QGraphicsView(parent) {

        // 创建 QGraphicsScene
        scene = new QGraphicsScene(this);
        this->setScene(scene);  // 将场景设置给视图

        // 设置视图的大小（可以根据实际需求调整）
        this->setFixedSize(8000, 6000);

        // 加载图像资源
        normalBlockPixmap = QPixmap(":/way/images/block/way.png");
        obstacleBlockPixmap = QPixmap(":/way/images/block/box.png");
        playerPixmap = QPixmap(":/green_one/images/Goblin/Run.png");
        enemyMeleePixmap = QPixmap(":/green_one/images/Goblin/Run.png");
        enemyRangedPixmap = QPixmap(":/eyes/images/eyes/Flight.png");
    }

    // 渲染地图的方法，接收 gameMap 作为参数
    void renderMap(Map *gameMap) {
        // qDebug() << "start rendering map!";
        if (!gameMap) {
            qDebug() << "Error: gameMap is null!";
            return;
        }

        // 清空场景
        scene->clear();

        int cellWidth = 128;  // 每个格子的宽度为 128 像素
        int cellHeight = 128; // 每个格子的高度为 128 像素


        for (int row = 0; row < gameMap->getRowCount(); ++row) {
            for (int col = 0; col < gameMap->getColCount(); ++col) {
                Cell &cell = gameMap->getCell(row, col);

                QGraphicsPixmapItem *item;
                QRect rect(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
                switch (cell.cellType) {
                case CellType::Normal:
                    item = new QGraphicsPixmapItem(normalBlockPixmap);
                    item->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(item);  // 添加到场景中
                    break;
                case CellType::Obstacle:
                    item = new QGraphicsPixmapItem(obstacleBlockPixmap);
                    item->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(item);  // 添加到场景中
                    break;
                case CellType::Player:
                    item = new QGraphicsPixmapItem(normalBlockPixmap);
                    item->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(item);  // 添加到场景中
                    QGraphicsPixmapItem *playerItem = new QGraphicsPixmapItem(playerPixmap);
                    playerItem->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(playerItem);  // 添加到场景中
                    break;
                }

            }
        }
        if(gameMap->getEnemy(1)!=nullptr){
            for(int k =0; k<2; ++k){
                qDebug() << "Enemy 是空指针吗？" << (gameMap->getEnemy(k)==nullptr);
                Enemy* enemy = gameMap->getEnemy(k);
                qDebug() << "敌人！";
                int row = enemy->getY();
                int col = enemy->getX();
                Cell &cell = gameMap->getCell(row, col);
                qDebug() << "敌人！1";
                QGraphicsPixmapItem *item;
                QRect rect(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
                qDebug() << "敌人！2";
                if (enemy->getType() == EnemyType::Melee) {
                    item = new QGraphicsPixmapItem(enemyMeleePixmap);
                    item->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(item);  // 添加到场景中
                } else{
                    item = new QGraphicsPixmapItem(enemyRangedPixmap);
                    item->setPos(rect.topLeft());  // 设置位置
                    scene->addItem(item);  // 添加到场景中
                }
                qDebug() << "敌人！3";
            }
        }

}

private:
    QGraphicsScene *scene;  // 场景
    QPixmap normalBlockPixmap;
    QPixmap obstacleBlockPixmap;
    QPixmap playerPixmap;
    QPixmap enemyMeleePixmap;
    QPixmap enemyRangedPixmap;
    QGraphicsPixmapItem* enemyItem1;
    QGraphicsPixmapItem* enemyItem2;
};

#endif // MAPDISPLAY_H
