#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "map.h"
#include "player.h"
#include "enemy.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 声明 initializeGame() 函数
private slots:
    void initializeGame();
    void keyPressEvent(QKeyEvent *event);  // 键盘按下事件


private:
    Ui::MainWindow *ui;
    Map *gameMap;
    QTextEdit *mapDisplay;
    Player *player1;
    QList<Enemy*> enemies;

    void updateMap();
    void gameLoop();
};
#endif // MAINWINDOW_H
