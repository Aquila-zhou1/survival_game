#include "mainwindow.h"
#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRandomGenerator>

class Cell : public QPushButton {
    Q_OBJECT
public:
    enum Type {
        Normal,
        Obstacle
    };

    explicit Cell(int row, int col, QWidget *parent = nullptr)
        : QPushButton(parent), row(row), col(col), cellType(Normal) {
        setFixedSize(50, 50);  // 设置每个格子的大小
        setStyleSheet("background-color: lightgray;");  // 普通格子背景色
    }

    void setObstacle(bool isObstacle) {
        if (isObstacle) {
            setStyleSheet("background-color: darkgray;");  // 障碍物背景色
            cellType = Obstacle;
        }
    }

    Type getType() const {
        return cellType;
    }

    int getRow() const { return row; }
    int getCol() const { return col; }

private:
    int row, col;
    Type cellType;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建一个中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建一个 10x10 的网格布局
    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    // 创建一个 10x10 的地图（Cell 是我们定义的格子类）
    QList<Cell*> cells;
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            Cell *cell = new Cell(row, col, centralWidget);
            gridLayout->addWidget(cell, row, col);
            cells.append(cell);
        }
    }

    // 随机添加至少 5 个障碍物
    int obstacleCount = 0;
    while (obstacleCount < 5) {
        int index = QRandomGenerator::global()->bounded(cells.size());
        Cell *cell = cells[index];
        if (cell->getType() == Cell::Normal) {
            cell->setObstacle(true);
            obstacleCount++;
        }
    }

    // 设置窗口大小
    setWindowTitle("Game Map");
    resize(600, 600);
}

MainWindow::~MainWindow()
{
}
#include "mainwindow.moc"

