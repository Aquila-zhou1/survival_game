#include "player.h"
#include "enemy.h"
#include "mainwindow.h"
#include <cstdlib>
#include <ctime>

Player::Player(QString name, int health, int attackPower, int speed, Weapon* weapon, int damageArea, int duration, int cooldown, QObject *parent)
    : QObject(parent), name(name), health(health), attackPower(attackPower), speed(speed),
    weapon(weapon), damageArea(damageArea), duration(duration), cooldown(cooldown), x(0), y(0), experience(0), level(1) {
    std::srand(std::time(nullptr));  // 初始化随机数生成器
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
}

void Player::move(int dx, int dy, Map *gameMap) {
    int newX = x + dx * speed;
    int newY = y + dy * speed;

    if (newX >= 0 && newX < gameMap->getColCount() && newY >= 0 && newY < gameMap->getRowCount()) {
        if (gameMap->getCell(newY, newX).cellType == CellType::Obstacle) {
            return;
        }
        x = newX;
        y = newY;
    }

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= gameMap->getColCount()) x = gameMap->getColCount() - 1;
    if (y >= gameMap->getRowCount()) y = gameMap->getRowCount() - 1;
}

void Player::attack(Map* gameMap) {
    qDebug() << "Attack started";

    if (weapon->getType() == WeaponType::Melee) {
        for (int i = 0; i < 2; ++i) {
            Enemy* enemy = gameMap->getEnemy(i);
            if (!enemy) continue;
            if (enemy->getX() == x && enemy->getY() == y) {
                enemy->takeDamage(attackPower);
                addExperience(10);  // 击杀敌人增加经验
            }
        }
    } else if (weapon->getType() == WeaponType::Ranged) {
        for (int i = 0; i < 2; ++i) {
            Enemy* enemy = gameMap->getEnemy(i);
            if (!enemy) continue;
            if (abs(enemy->getX() - x) <= weapon->getDamageArea() && abs(enemy->getY() - y) <= weapon->getDamageArea()) {
                enemy->takeDamage(attackPower);
                addExperience(10);  // 击杀敌人增加经验
            }
        }
    }

    qDebug() << "Attack ended";
    weapon->useWeapon();
}

void Player::addExperience(int amount) {
    experience += amount;
    if (experience >= 100 * level) {  // 假设每级100经验
        levelUp();
    }
}

void Player::levelUp() {
    level++;
    experience = 0;  // 升级后经验清零

    // 随机选择一个强化选项
    PowerUp powerUp = getPowerUp();
    applyPowerUp(powerUp);
    qDebug() << "Level Up! New Level: " << level;
}

PowerUp Player::getPowerUp() {
    // 创建PowerUpDialog并弹出
    // PowerUpDialog dialog(this);
    // connect(&dialog, &PowerUpDialog::powerUpChosen, this, [this](int choice) {
    //     switch (choice) {
    //     case 0:
    //         health += 10;  // 增加生命值
    //         break;
    //     case 1:
    //         attackPower += 10;  // 增加攻击力
    //         break;
    //     case 2:
    //         speed += 5;  // 增加移动速度
    //         break;
    //     default:
    //         break;
    //     }
    // });
    // dialog.exec();  // 弹出对话框
}

void Player::applyPowerUp(const PowerUp& powerUp) {
    health += powerUp.healthIncrease;
    attackPower += powerUp.attackPowerIncrease;
    speed += powerUp.speedIncrease;
    cooldown -= powerUp.cooldownReduction;

    if (health > 100) health = 100;  // 假设最大生命值是100
    if (speed < 1) speed = 1;  // 最小移动速度为1
    if (cooldown < 0) cooldown = 0;  // 冷却时间不能为负
}
