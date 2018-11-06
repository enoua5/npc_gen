/*
 * This project uses material from several OGL sources
 * Please read the file `LICENSE` for more details
 */

#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;

#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <string>

#include "lib/npc.h"

NPC_Fields make_npc(string race_table)
{
  if(race_table=="") race_table="EhDHdGeOT::";
  
  NPC npc(race_table);
  
  return npc.packFields();
}

EMSCRIPTEN_BINDINGS(i_dont_know_what_this_variable_does)
{
  value_object<Stat_Fields>("Stat_Fields")
    .field("roll", &Stat_Fields::roll)
    .field("adj", &Stat_Fields::adj)
    .field("score", &Stat_Fields::score)
    .field("mod", &Stat_Fields::mod)
    ;

  value_object<NPC_Fields>("NPC_Fields")
    .field("name", &NPC_Fields::name)
    .field("sex", &NPC_Fields::sex)
    .field("race", &NPC_Fields::race)
    .field("occupation", &NPC_Fields::occupation)
    .field("hp", &NPC_Fields::hp)
    .field("ac", &NPC_Fields::ac)
    .field("speed", &NPC_Fields::speed)
    .field("str", &NPC_Fields::str)
    .field("dex", &NPC_Fields::dex)
    .field("con", &NPC_Fields::con)
    .field("int", &NPC_Fields::intel)
    .field("wis", &NPC_Fields::wis)
    .field("cha", &NPC_Fields::cha)
    .field("weapon", &NPC_Fields::weapon)
    .field("toHit", &NPC_Fields::toHit)
    .field("dmg", &NPC_Fields::dmg)
    .field("gear", &NPC_Fields::gear)
    .field("traits", &NPC_Fields::traits)
    .field("quirks", &NPC_Fields::quirks)
    .field("bonds", &NPC_Fields::bonds)
    ;
    
  emscripten::function("make_npc", &make_npc);
}
