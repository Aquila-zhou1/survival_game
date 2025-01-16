#include "map.h"
#include "mainwindow.h"
#include <iostream>
#include <cstdlib>  // 用于随机数生成
#include <vector>
#include "enemy.h"

// 构造函数：初始化地图
Map::Map(int rows, int cols)
    : rows(rows), cols(cols) {
    // 初始化每个格子的位置和类型
    grid.resize(rows);  // 为每行分配空间
    for (int i = 0; i < rows; ++i) {
        grid[i].resize(cols);  // 为每列分配空间
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = Cell(i, j);  // 使用带参数的构造函数初始化
        }
    }
}


// 获取指定位置的格子
Cell& Map::getCell(int row, int col) {
    return grid[row][col];
}

// 随机生成障碍物
void Map::generateObstacles(int count) {
    int obstaclesGenerated = 0;
    while (obstaclesGenerated < count) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (grid[row][col].cellType == CellType::Normal) {
            grid[row][col].cellType = CellType::Obstacle;
            ++obstaclesGenerated;
        }
    }
}

// 打印地图（用于调试）
void Map::printMap() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j].cellType == CellType::Normal) {
                std::cout << " . ";  // 普通格子
            } else if (grid[i][j].cellType == CellType::Obstacle) {
                std::cout << " # ";  // 障碍物
            } else if (grid[i][j].cellType == CellType::Player) {
                std::cout << " P ";  // 玩家
            } else if (grid[i][j].cellType == CellType::Enemy) {
                std::cout << " E ";  // 敌人
            }
        }
        std::cout << std::endl;
    }
}

// 获取地图的行数
int Map::getRowCount() const {
    return rows;
}

// 获取地图的列数
int Map::getColCount() const {
    return cols;
}
