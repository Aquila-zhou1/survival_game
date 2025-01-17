// weapon.h
#ifndef WEAPON_H
#define WEAPON_H

enum class WeaponType {
    Melee,   // 近战武器
    Ranged   // 远程武器
};

class Weapon {
public:
    Weapon(WeaponType type, int damageArea, int duration, int cooldown)
        : type(type), damageArea(damageArea), duration(duration), cooldown(cooldown), lastUsed(0) {}

    WeaponType getType() const { return type; }
    int getDamageArea() const { return damageArea; }
    int getDuration() const { return duration; }
    int getCooldown() const { return cooldown; }

    void useWeapon() {
        lastUsed = duration;  // 使用武器时，记录武器上次使用的时间
    }

private:
    WeaponType type;
    int damageArea;  // 武器的攻击范围
    int duration;    // 持续时间
    int cooldown;    // 冷却时间
    int lastUsed;    // 上次使用时间
};

#endif // WEAPON_H
