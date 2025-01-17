#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include "map.h"
#include "player.h"

enum class EnemyType {
    Melee,  // 近战怪物
    Ranged  // 远程怪物
};

class Enemy : public QObject {
    Q_OBJECT
public:
    Enemy(int health, int attackPower, int speed, int x, int y, EnemyType type, QObject *parent = nullptr)
        : QObject(parent), health(health), attackPower(attackPower), speed(speed), x(x), y(y), type(type) {}

    virtual void move(Map *gameMap, Player *player) = 0;  // 纯虚函数，让派生类实现不同的移动策略
    virtual void attack(Player *player) = 0;  // 攻击玩家

    int getX() const { return x; }
    int getY() const { return y; }
    int getHealth() const { return health; }
    EnemyType getType() const { return type; }
    // takeDamage方法，用于敌人受到伤害
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;  // 确保生命值不小于0
    }

protected:
    int health;
    int attackPower;
    int speed;
    int x, y;
    EnemyType type;
};

// 近战敌人类
class MeleeEnemy : public Enemy {
public:
    MeleeEnemy(int x, int y)
        : Enemy(50, 10, 1, x, y, EnemyType::Melee) {}

    void move(Map *gameMap, Player *player) override {
        // 近战敌人向玩家方向移动
        if (player->getX() > x) {
            x++;
        } else if (player->getX() < x) {
            x--;
        }
        if (player->getY() > y) {
            y++;
        } else if (player->getY() < y) {
            y--;
        }
    }

    void attack(Player *player) override {
        if (x == player->getX() && y == player->getY()) {
            player->takeDamage(attackPower);  // 玩家受伤
        }
    }
};

// 远程敌人类
class RangedEnemy : public Enemy {
public:
    RangedEnemy(int x, int y)
        : Enemy(30, 5, 1, x, y, EnemyType::Ranged) {}

    void move(Map *gameMap, Player *player) override {
        // 远程敌人向玩家方向移动
        if (player->getX() > x) {
            x++;
        } else if (player->getX() < x) {
            x--;
        }
        if (player->getY() > y) {
            y++;
        } else if (player->getY() < y) {
            y--;
        }
    }

    void attack(Player *player) override {
        // 远程敌人攻击范围（可以定义为一定的距离）
        if (abs(x - player->getX()) <= 2 && abs(y - player->getY()) <= 2) {
            player->takeDamage(attackPower);  // 玩家受伤
        }
    }
};

#endif // ENEMY_H
