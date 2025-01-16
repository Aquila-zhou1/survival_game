#include "player.h"
#include "mainwindow.h"

Player::Player(QString name, int health, int attackPower, int speed, WeaponType weaponType, int damageArea, int duration, int cooldown, QObject *parent)
    : QObject(parent), name(name), health(health), attackPower(attackPower), speed(speed),
    weaponType(weaponType), damageArea(damageArea), duration(duration), cooldown(cooldown), x(0), y(0) {}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;  // 假设最大生命值是100
}

void Player::move(int dx, int dy, Map *gameMap) {
    int newX = x + dx * speed;
    int newY = y + dy * speed;

    // 检查目标位置是否为障碍物
    if (newX >= 0 && newX < gameMap->getColCount() && newY >= 0 && newY < gameMap->getRowCount()) {
        // 如果目标位置是障碍物，则不能移动
        if (gameMap->getCell(newY, newX).cellType == CellType::Obstacle) {
            return;  // 不进行移动
        }

        // 更新玩家位置
        x = newX;
        y = newY;
    }

    // 确保玩家坐标在地图范围内
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= gameMap->getColCount()) x = gameMap->getColCount() - 1;
    if (y >= gameMap->getRowCount()) y = gameMap->getRowCount() - 1;
}


void Player::attack(Map *gameMap) {
    // if (weaponType == WeaponType::Melee) {
    //     // 近战攻击：检查玩家当前位置的敌人
    //     for (auto enemy : gameMap->getEnemies()) {
    //         if (enemy->getX() == x && enemy->getY() == y) {
    //             enemy->takeDamage(attackPower);  // 玩家攻击敌人
    //             return;
    //         }
    //     }
    // } else if (weaponType == WeaponType::Ranged) {
    //     // 远程攻击：检查范围内的敌人
    //     for (auto enemy : gameMap->getEnemies()) {
    //         if (abs(enemy->getX() - x) <= damageArea && abs(enemy->getY() - y) <= damageArea) {
    //             enemy->takeDamage(attackPower);  // 玩家攻击敌人
    //         }
    //     }
    // }
}
