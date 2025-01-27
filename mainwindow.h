#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <Qlabel>
#include <QScrollArea>
#include <QGridLayout>
#include "map.h"
#include "player.h"
#include "enemy.h"
#include "MapDisplay.h"
#include <QMutex>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int currentLevel = 1;      // 当前关卡
    int totalLevels = 20;       // 总关卡数
    bool can_create_new = true;
    bool can_mid = false;
    bool can_end = false;
    QPushButton *startButton;
    QPushButton *middleButton;
    QPushButton *endButton;
    QTimer *gameTimer;  // 定义一个成员变量用于管理定时器


public:
    explicit  MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 声明 initializeGame() 函数
private slots:
    void initializeGame(bool need_archive = false);
    void generateEnemiesForCurrentLevel();
    void levelComplete();
    void gameLoop();
    void updateMap();
    void updateGame();
    void updatePlayerStatus(int experience, int level);
    void saveArchive();
    void loadArchive(Enemy*& enemy1, Enemy*& enemy2);
    void start_setting();
    void mid_setting();
    void end_setting();


protected:
    void keyPressEvent(QKeyEvent *event);  // 键盘按下事件


private:
    Ui::MainWindow *ui;
    Map *gameMap;
    // QTextEdit *mapDisplay;
    MapDisplay *mapDisplay;
    QGridLayout *gridLayout;  // 声明 gridLayout 作为成员变量
    QScrollArea *scrollArea;  // 声明 QScrollArea 作为成员变量
    Player *player1;
    QList<Enemy*> enemies;
    QMutex mutex;

private:
    QLabel* playerStatusLabel;  // 显示玩家的状态（经验、等级等）
};
#endif // MAINWINDOW_H
