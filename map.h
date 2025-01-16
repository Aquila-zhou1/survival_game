#ifndef MAP_H
#define MAP_H

#include <vector>
// 定义格子的类型
enum class CellType {
    Normal,      // 普通格子
    Obstacle,    // 障碍物
    Player,      // 玩家
    Enemy        // 敌人
};

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
    void printMap() const;                  // 打印地图（调试用）
    int getRowCount() const;                // 获取地图的行数
    int getColCount() const;                // 获取地图的列数


private:
    int rows, cols;                          // 地图的行数和列数
    std::vector<std::vector<Cell>> grid;     // 二维格子数组

};

#endif // MAP_H
