#include "monster_engine.h"

VALUE rb_mMonsterEngine;

void Init_monster_engine(void) {
  rb_mMonsterEngine = rb_define_module("MonsterEngine");
  Init_monster_engine_config();
  Init_monster_engine_server();
}
