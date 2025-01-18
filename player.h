// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QObject>
#include "map.h"
#include "weapon.h"
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class PowerUpDialog : public QDialog {// 强化选项的窗口
    Q_OBJECT

public:
    explicit PowerUpDialog(QWidget* parent = nullptr)
        : QDialog(parent) {
        // 设置窗口的标题
        setWindowTitle("Choose Power Up");

        // 创建按钮
        QPushButton* btnHealth = new QPushButton("Increase Health", this);
        QPushButton* btnAttack = new QPushButton("Increase Attack", this);
        QPushButton* btnSpeed = new QPushButton("Increase Speed", this);

        // 布局管理
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(new QLabel("Choose one power-up:"));
        layout->addWidget(btnHealth);
        layout->addWidget(btnAttack);
        layout->addWidget(btnSpeed);
        setLayout(layout);

        // 连接按钮的点击信号
        connect(btnHealth, &QPushButton::clicked, this, &PowerUpDialog::onHealthChosen);
        connect(btnAttack, &QPushButton::clicked, this, &PowerUpDialog::onAttackChosen);
        connect(btnSpeed, &QPushButton::clicked, this, &PowerUpDialog::onSpeedChosen);
    }

signals:
    void powerUpChosen(int choice);

private slots:
    void onHealthChosen() {
        emit powerUpChosen(0);  // 假设0是增加生命值
        accept();  // 关闭窗口
    }
    void onAttackChosen() {
        emit powerUpChosen(1);  // 假设1是增加攻击力
        accept();  // 关闭窗口
    }
    void onSpeedChosen() {
        emit powerUpChosen(2);  // 假设2是增加移动速度
        accept();  // 关闭窗口
    }
};



class PowerUp {  // 强化选项类
public:
    int healthIncrease;
    int attackPowerIncrease;
    int speedIncrease;
    int cooldownReduction;
    PowerUp(int health, int attackPower, int speed, int cooldown)
        : healthIncrease(health), attackPowerIncrease(attackPower), speedIncrease(speed), cooldownReduction(cooldown) {}
};

class Player : public QObject {
    Q_OBJECT
public:
    Player(QString name, int health, int attackPower, int speed, Weapon* weapon, int damageArea, int duration, int cooldown, QObject *parent = nullptr);

    QString getName() const { return name; }
    int getHealth() const { return health; }
    int getExperience() const { return experience; }
    int getLevel() const { return level; }

    void takeDamage(int damage);
    void heal(int amount);
    void move(int dx, int dy, Map *gameMap);
    void attack(Map *gameMap);

    int getX() const { return x; }  // 获取玩家的X坐标
    int getY() const { return y; }  // 获取玩家的Y坐标

    void addExperience(int amount);  // 增加经验
    void levelUp();  // 升级
    PowerUp getPowerUp();  // 随机选择强化
    void applyPowerUp(const PowerUp& powerUp);  // 应用强化

private:
    QString name;
    int health;
    int attackPower;
    int speed;
    Weapon* weapon;
    int damageArea;
    int duration;
    int cooldown;
    int x, y;

    int experience;  // 经验值
    int level;  // 等级
};

#endif // PLAYER_H
