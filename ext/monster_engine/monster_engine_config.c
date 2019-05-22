#include "monster_engine.h"
#include "wrapper.h"

VALUE rb_cMonsterEngineConfig;

static void deallocate(Wrapper *wrapper) {
  monster_engine_config_destroy(wrapper->inner);
  free(wrapper);
}

static VALUE allocate(VALUE klass) {
  return Data_Wrap_Struct(klass, NULL, deallocate, malloc(sizeof(Wrapper)));
}

static VALUE initialize(VALUE self, VALUE rb_bind, VALUE rb_workers) {
  Wrapper *wrapper;
  Data_Get_Struct(self, Wrapper, wrapper);
  wrapper->inner = monster_engine_config_new(StringValueCStr(rb_bind), FIX2UINT(rb_workers));
  return self;
}

void Init_monster_engine_config(void) {
  rb_cMonsterEngineConfig = rb_define_class_under(rb_mMonsterEngine, "Config", rb_cObject);
  rb_define_alloc_func(rb_cMonsterEngineConfig, allocate);
  rb_define_method(rb_cMonsterEngineConfig, "initialize", initialize, 2);
}
