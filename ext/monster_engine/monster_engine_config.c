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

static VALUE initialize(VALUE self) {
  DATA_PTR(self) = monster_engine_config_new();
  rb_iv_set(self, "@workers", INT2FIX(1));
  return self;
}

static VALUE get_bind(VALUE self) {
  return rb_str_new2(monster_engine_config_get_bind(DATA_PTR(self)));
}

static VALUE set_bind(VALUE self, VALUE rb_bind) {
  monster_engine_config_set_bind(DATA_PTR(self), StringValueCStr(rb_bind));
  return Qnil;
}

static VALUE get_workers(VALUE self) {
  return rb_iv_get(self, "@workers");
}

static VALUE set_workers(VALUE self, VALUE rb_workers) {
  return rb_iv_set(self, "@workers", rb_workers);
}

void Init_monster_engine_config(void) {
  rb_cMonsterEngineConfig = rb_define_class_under(rb_mMonsterEngine, "Config", rb_cObject);
  rb_define_alloc_func(rb_cMonsterEngineConfig, allocate);
  rb_define_method(rb_cMonsterEngineConfig, "initialize", initialize, 0);
  rb_define_method(rb_cMonsterEngineConfig, "bind", get_bind, 0);
  rb_define_method(rb_cMonsterEngineConfig, "bind=", set_bind, 1);
  rb_define_method(rb_cMonsterEngineConfig, "workers", get_workers, 0);
  rb_define_method(rb_cMonsterEngineConfig, "workers=", set_workers, 1);
}
