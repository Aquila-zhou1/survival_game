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

    int getMoney() const {return money;}
    void setMoney(int m) {money = m;}
    void addMoney(int m) {money = money+m;}

    int getaddHealth() const {return add_health;}
    void setaddHealth(int h) {add_health = h;}



    QString getName() const { return name; }
    void setName(const QString &newName) {name = newName;}

    int getHealth() const { return health; }
    void setHealth(int newHealth) {health = newHealth;}

    int getExperience() const { return experience; }
    void setExperience(int newExperience) {experience = newExperience;}

    int getLevel() const { return level; }
    void setLevel(int newLevel) {level = newLevel;}

    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) {x = newX;y = newY;}
    void setWeapon(Weapon* w) {weapon = w;}

    void takeDamage(int damage);
    void heal(int amount);
    void move(int dx, int dy, Map *gameMap);
    void attack(Map *gameMap);

    void addExperience(int amount); //增加经验
    void levelUp();     // 升级
    PowerUp getPowerUp();       //随机选择强化
    void applyPowerUp(const PowerUp& powerUp);      //应用强化

    Weapon* getWeapon() const {return weapon;}


private:
    QString name;
    int health;
    int add_health;
    int attackPower;
    int speed;
    int money;
    Weapon* weapon;
    int damageArea;
    int duration;
    int cooldown;
    int x, y;

    int experience;
    int level;
};

#endif // PLAYER_H
