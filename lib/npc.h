#pragma once

/*
    WARNING: DISGUSTING CODE AHEAD
 */

#include <iostream>
#include <vector>
#include <string>

#include "roll.h"
#include "weapon.h"
#include "occupations.h"
#include "female_names.h"
#include "male_names.h"
#include "quirks_phys.h"
#include "quirks_pers.h"

using namespace std;

struct Stat_Fields
{
  int roll;
  int adj;
  int score;
  int mod;
};
struct NPC_Fields
{
  string name;
  string sex;
  string race;
  string occupation;
  
  int hp;
  int ac;
  int speed;
  
  Stat_Fields str;
  Stat_Fields dex;
  Stat_Fields con;
  Stat_Fields intel;
  Stat_Fields wis;
  Stat_Fields cha;
  
  string weapon;
  string toHit;
  string dmg;
  
  string gear;
  string traits;
  string quirks;
  string bonds;
};

enum Race
{
  ELF='E',
  HALFLING='h',
  DWARF='D',
  HUMAN='H',
  DRAGONBORN='d',
  GNOME='G',
  HALFELF='e',
  HALFORC='O',
  TIEFLING='T'
};

string raceToString(Race r)
{
  switch(r)
  {
    case ELF:
      return "Elf";
      break;
    case HALFLING:
      return "Halfling";
      break;
    case DWARF:
      return "Dwarf";
      break;
    case HUMAN:
      return "Human";
      break;
    case DRAGONBORN:
      return "Dragonborn";
      break;
    case GNOME:
      return "Gnome";
      break;
    case HALFELF:
      return "Half-Elf";
      break;
    case HALFORC:
      return "Half-Orc";
      break;
    case TIEFLING:
      return "Tiefling";
      break;
    default:
      return "Error";
  }
}

class Stat
{
  public:
    Stat()
    {
      dice=roll(3,6);
    }
    
    Stat_Fields packFields()
    {
      return {
        dice,
        adj,
        getTot(),
        getMod()
      };
    }
    
    char adj=0;
    char getTot() const
    {
      return dice+adj;
    }
    char getMod() const
    {
      return (getTot()/2)-5;
    }
    
    friend ostream &operator<<(ostream &out, const Stat &me)
    {
      out << me.toString();
      return out;
    }
    
    string toString() const
    {
      return to_string((int)getTot()) + " (" + ((getMod()>0)?"+":"") + to_string((int)getMod()) + ")";
    }
  private:
    char dice;
};

class NPC
{
  public:
    NPC(string raceTable)
    {
      sex=roll(1, 2, -1);
      name=sex?male_names[roll(1, male_names.size(), -1)]:female_names[roll(1, male_names.size(), -1)];
      race=getRace(raceTable);
      giveRacialBonuses(race);
      occupation=occupation_table[roll(1, occupation_table.size(), -1)];
      quirk_phys=quirks_phys[roll(1, quirks_phys.size(), -1)];
      quirk_pers=quirks_pers[roll(1, quirks_pers.size(), -1)];
      setBond();
      calcOthers();
    }
    NPC_Fields packFields()
    {
      int weapon_bonus;
      if(occupation.weapon.stat==STR)
        weapon_bonus=str.getMod();
      else if(occupation.weapon.stat==DEX)
        weapon_bonus=dex.getMod();
      else
        weapon_bonus=(str.getMod()>dex.getMod()?str.getMod():dex.getMod());
        
      string gear_list="";
      for(int i=0; i<occupation.gear.size(); i++)
        gear_list+=occupation.gear[i] + (i!=occupation.gear.size()-1?", ":"");
        
      string trait_list="";
      for(int i=0; i<traits.size(); i++)
        trait_list+=traits[i] + (i!=traits.size()-1?", ":"");
        
      string dmg;
      if(occupation.weapon.dmg_die==0)
        dmg=to_string((str.getMod()+1>0)?weapon_bonus+1:1);
      else
        dmg="1d" + to_string(occupation.weapon.dmg_die) + (weapon_bonus>=0?"+":"") + to_string(weapon_bonus);
      
      return {
        name,
        sex?"Male":"Female",
        raceToString(race),
        occupation.name,
        hp,
        ac,
        speed,
        
        str.packFields(),
        dex.packFields(),
        con.packFields(),
        intel.packFields(),
        wis.packFields(),
        cha.packFields(),
        
        occupation.weapon.name,
        (weapon_bonus>0?"+":"") + to_string(weapon_bonus),
        dmg,
        
        gear_list,
        trait_list,
        quirk_phys + ", " + quirk_pers,
        bond_stat
      };
    }
    friend ostream &operator<<(ostream &out, const NPC &me)
    {
      out << "Name: " << me.name << endl;
      out << "Sex: " << (me.sex?"Male":"Female") << endl;
      out << "Race: " << raceToString(me.race) << endl;
      out << "Occupation: " << me.occupation.name << endl;
      out << endl;
      out << "Hit Points: " << me.hp << endl;
      out << "Armor Class: " << me.ac << endl;
      out << "Speed: " << me.speed << endl;
      out << "----------" << endl;
      out << "STR " << me.str << endl;
      out << "DEX " << me.dex << endl;
      out << "CON " << me.con << endl;
      out << "INT " << me.intel << endl;
      out << "WIS " << me.wis << endl;
      out << "CHA " << me.cha << endl;
      out << "----------" << endl;
      out << "Weapon: " << me.occupation.weapon.name << endl;
      
      int weapon_bonus;
      if(me.occupation.weapon.stat==STR)
        weapon_bonus=me.str.getMod();
      else if(me.occupation.weapon.stat==DEX)
        weapon_bonus=me.dex.getMod();
      else
        weapon_bonus=(me.str.getMod()>me.dex.getMod()?me.str.getMod():me.dex.getMod());
        
      out << "To Hit: " << (weapon_bonus>0?"+":"") << weapon_bonus << endl;
      
      if(me.occupation.weapon.dmg_die==0)
        out << "Dmg: " << ((me.str.getMod()+1>0)?weapon_bonus+1:1) << endl;
      else
          out << "Dmg: 1d" << me.occupation.weapon.dmg_die << (weapon_bonus>=0?"+":"") << weapon_bonus << endl;
        
      out << "----------" << endl;
      out << "Gear: ";
      for(int i=0; i<me.occupation.gear.size(); i++)
        out << me.occupation.gear[i] << (i!=me.occupation.gear.size()-1?", ":"");
      out << endl;
      
      out << "Racial Traits: ";
      for(int i=0; i<me.traits.size(); i++)
        out << me.traits[i] << (i!=me.traits.size()-1?", ":"");
      out << endl;
      
      out << "Physical Quirk: " << me.quirk_phys << endl;
      out << "Personality Quirk: " << me.quirk_pers << endl;
      out << "Bond Stat: " << me.bond_stat << endl;
      
      return out;
    }
  private:
    Race getRace(string raceTable)
    {
      string pool="";
      string exot=""; //exotic pool
      char section=0; //what part of the string are we on?
      for(int i=0; i<raceTable.length(); i++)
      {
        if(raceTable[i]==':') //section divider
        {
          section++;
          continue;
        }
        
        if(section==0) //section 0 adds normally
          pool+=raceTable[i];
        else if(section==1) //section 1 adds lots
          for(int x=0; x<16; x++)
            pool+=raceTable[i];
        else if(section==2) //section 2 is exotic
          exot+=raceTable[i];
      }
      if(exot!="")
        pool+="*"; //roll on exot
      
      char ret=pool[roll(1,pool.length(),-1)];
      if(ret=='*')
        ret=exot[roll(1,exot.length(),-1)];
      
      return (Race)ret;
    }
    void giveRacialBonuses(Race r)
    {
      speed=30;
      switch(r)
      {
        case 'D':
          con.adj+=2;
          speed=25;
          traits.push_back("Darkvision");
          traits.push_back("Dwarven Resilience");
          break;
        case 'E':
          dex.adj+=2;
          traits.push_back("Darkvision");
          traits.push_back("Fey Ancestry");
          break;
        case 'h':
          dex.adj+=2;
          speed=25;
          traits.push_back("Lucky");
          traits.push_back("Brave");
          traits.push_back("Small Size");
          break;
        case 'H':
          str.adj+=1;
          dex.adj+=1;
          con.adj+=1;
          wis.adj+=1;
          intel.adj+=1;
          cha.adj+=1;
          break;
        case 'T':
          intel.adj+=2;
          cha.adj+=1;
          traits.push_back("Darkvision");
          traits.push_back("Hellish Resistance");
          break;
        case 'O':
          str.adj+=2;
          con.adj+=1;
          traits.push_back("Darkvision");
          traits.push_back("Relentless Endurance");
          break;
        case 'd':
          str.adj+=2;
          cha.adj+=1;
          //variable block
          {
            //choose type of dragonborn
            string dragonTypes[10][2]={
              {"Black","Acid"},
              {"Blue","Lightning"},
              {"Brass","Fire"},
              {"Bronze","Lightning"},
              {"Copper","Acid"},
              {"Gold","Fire"},
              {"Green","Poison"},
              {"Red","Fire"},
              {"Silver","Cold"},
              {"White","Cold"}
            };
            
            int dt_i=roll(1, 10, -1);
            string dragonType[2]={dragonTypes[dt_i][0], dragonTypes[dt_i][1]};
            
            traits.push_back("Draconic Ancestry ("+dragonType[0]+")");
            traits.push_back("Damage Resitance ("+dragonType[1]+")");
          }
          break;
        case 'G':
          intel.adj+=2;
          con.adj+=1;
          speed=25;
          traits.push_back("Darkvision");
          traits.push_back("Gnome Cunning");
          break;
        case 'e':
          cha.adj+=2;
          //variable block
          {
            //need to pick 2 other stats to increase.
            string canInc="sdciw";
            for(int x=0; x<2; x++)
            {
              //pick an index
              char i=roll(1, canInc.length(), -1);
              //save the char from that index
              char s=canInc[i];
              //remove the choice from the pool
              canInc.erase(i, 1);
              
              switch(s)
              {
                case 's':
                  str.adj+=1;
                  break;
                case 'd':
                  dex.adj+=1;
                  break;
                case 'c':
                  con.adj+=1;
                  break;
                case 'i':
                  intel.adj+=1;
                  break;
                case 'w':
                  wis.adj+=1;
                  break;
              }
            }
          }
          
          traits.push_back("Darkvision");
          traits.push_back("Fey Ancestry");
          break;
      }
    }
    
    void setBond()
    {
      int choice=roll(1, 6);
      switch(choice)
      {
        case 1:
          str.adj+=1;
          bond_stat="Strength";
          break;
        case 2:
          dex.adj+=1;
          bond_stat="Dexterity";
          break;
        case 3:
          con.adj+=1;
          bond_stat="Constitution";
          break;
        case 4:
          intel.adj+=1;
          bond_stat="Intelligence";
          break;
        case 5:
          wis.adj+=1;
          bond_stat="Wisdom";
          break;
        case 6:
          cha.adj+=1;
          bond_stat="Charisma";
          break;
      }
    }
    
    void calcOthers()
    {
      hp=4+con.getMod();
      ac=10+dex.getMod();
      
    }
    
    string name = "Nameless";
    bool sex;
    Race race;
    Occupation occupation=default_occupation;
    
    int hp = 0;
    int ac = 0;
    int speed = 30;
    
    Stat str;
    Stat dex;
    Stat con;
    Stat intel; //can't be int
    Stat wis;
    Stat cha;
    
    vector<string> traits;
    string quirk_phys;
    string quirk_pers;
    string bond_stat;
};
