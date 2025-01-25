#ifndef MAP_H
#define MAP_H
//#include "enemy.h"
#include <vector>
#include <QMessageBox>
// 定义格子的类型
enum class CellType {
    Normal,      // 普通格子
    Obstacle,    // 障碍物
    Player,      // 玩家
    Enemy        // 敌人
};

class Enemy;

// Cell 类，表示地图上的每个格子
class Cell {
public:
    int row;
    int col;
    CellType cellType;

    // 默认构造函数
    Cell() : row(0), col(0), cellType(CellType::Normal) {}

    // 带参数的构造函数
    Cell(int row, int col, CellType type = CellType::Normal)
        : row(row), col(col), cellType(type) {}
};


class Map {
public:
    Map(int rows, int cols);                // 构造函数：初始化地图
    Cell& getCell(int row, int col);        // 获取指定位置的格子
    void generateObstacles(int count);      // 随机生成障碍物
    std::vector<std::pair<int, int>> getObstacles() const; // 获取障碍物位置信息
    void addObstacle(int row, int col);
    void printMap() const;                  // 打印地图（调试用）
    int getRowCount() const;                // 获取地图的行数
    int getColCount() const;                // 获取地图的列数
    // 获取指定索引的敌人（这里只有两个敌人）
    Enemy* getEnemy(int index) {
        if (index == 0) return enemy1;
        if (index == 1) return enemy2;
        qDebug() << "索引越界";
        return nullptr;  // 索引越界时返回空指针
    }
    // 设置敌人
    void setEnemy(Enemy* e1, Enemy* e2) {
        enemy1 = e1;
        enemy2 = e2;
    }



private:
    int rows, cols;                          // 地图的行数和列数
    std::vector<std::vector<Cell>> grid;     // 二维格子数组
    Enemy* enemy1 = nullptr;  // 第一个敌人
    Enemy* enemy2 = nullptr;  // 第二个敌人
};

#endif // MAP_H
