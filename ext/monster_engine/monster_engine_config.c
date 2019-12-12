#include "monster_engine.h"

VALUE rb_cMonsterEngineConfig;

static void deallocate(void *monster_engine_config) {
  monster_engine_config_destroy(monster_engine_config);
}

const rb_data_type_t rb_monster_engine_config_type = {
  "Config",
  {
    NULL,
    deallocate,
    NULL,
  },
  NULL,
  NULL,
  0,
};

static VALUE allocate(VALUE klass) {
  return TypedData_Wrap_Struct(klass, &rb_monster_engine_config_type, NULL);
}

static VALUE initialize(VALUE self, VALUE rb_bind, VALUE rb_workers) {
  DATA_PTR(self) = monster_engine_config_new(StringValueCStr(rb_bind), FIX2UINT(rb_workers));
  return self;
}

void Init_monster_engine_config(void) {
  rb_cMonsterEngineConfig = rb_define_class_under(rb_mMonsterEngine, "Config", rb_cObject);
  rb_define_alloc_func(rb_cMonsterEngineConfig, allocate);
  rb_define_method(rb_cMonsterEngineConfig, "initialize", initialize, 2);
}
