#pragma once

#include <string>

enum WeaponStat
{
  STR,
  DEX,
  FIN
};

struct Weapon
{
  std::string name;
  WeaponStat stat;
  
  int dmg_die;
};

Weapon default_weapon={"Unarmed Strike", STR, 0};
Weapon light_crossbow={"Light Crossbow", DEX, 8};
Weapon shortbow={"Shortbow", DEX, 6};
Weapon club={"Club", STR, 4};
Weapon longsword={"Longsword", STR, 8};
Weapon hand_axe={"Hand axe", STR, 6};
Weapon spear={"Spear", STR, 6};
Weapon longbow={"Longbow", STR, 8};
Weapon shortsword={"Shortsword", FIN, 6};
