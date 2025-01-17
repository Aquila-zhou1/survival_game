#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QObject>
#include "map.h"
#include "weapon.h"

class Player : public QObject {
    Q_OBJECT
public:
    // Player 构造函数，初始化玩家的各项属性
    // 构造函数传递一个 Weapon 对象
    Player(QString name, int health, int attackPower, int speed, Weapon* weapon, int damageArea, int duration, int cooldown, QObject *parent = nullptr);

    QString getName() const { return name; }
    int getHealth() const { return health; }
    void takeDamage(int damage);  // 玩家受到伤害
    void heal(int amount);  // 玩家恢复生命
    void move(int dx, int dy, Map *gameMap);  // 玩家移动
    void attack(Map *gameMap);  // 玩家攻击
    int getX() const { return x; }  // 获取玩家的X坐标
    int getY() const { return y; }  // 获取玩家的Y坐标

private:
    QString name;       // 玩家名称
    int health;         // 玩家生命值
    int attackPower;    // 玩家攻击力
    int speed;          // 玩家移动速度
    Weapon* weapon;      // 玩家持有的武器
    int damageArea;     // 伤害区域
    int duration;       // 持续时间
    int cooldown;       // 冷却时间
    int x, y;           // 玩家位置
};

#endif // PLAYER_H
