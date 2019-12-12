#include "monster_engine.h"
#include <ruby/thread.h>

VALUE rb_cMonsterEngineServer;

typedef struct MonsterEngineRbCallbackSetting {
  PlamoApp *plamo_app;
  MonsterEngineConfig *monster_engine_config;
} MonsterEngineRbCallbackSetting;

static void* callback(void *value) {
  MonsterEngineRbCallbackSetting *setting = (MonsterEngineRbCallbackSetting*)value;
  monster_engine_server_start(setting->plamo_app, setting->monster_engine_config);
  return NULL;
}

extern const rb_data_type_t rb_plamo_app_type;

static VALUE start(VALUE self, VALUE rb_plamo_app, VALUE rb_monster_engine_config) {
  PlamoApp *plamo_app;
  TypedData_Get_Struct(rb_plamo_app, PlamoApp, &rb_plamo_app_type, plamo_app);

  MonsterEngineConfig *monster_engine_config;
  TypedData_Get_Struct(rb_monster_engine_config, MonsterEngineConfig, &rb_monster_engine_config_type, monster_engine_config);
  MonsterEngineRbCallbackSetting *monster_engine_rb_callback_setting = malloc(sizeof(MonsterEngineRbCallbackSetting));
  monster_engine_rb_callback_setting->plamo_app = plamo_app;
  monster_engine_rb_callback_setting->monster_engine_config = monster_engine_config;

  rb_thread_call_without_gvl(callback, monster_engine_rb_callback_setting, RUBY_UBF_PROCESS, NULL);
  return Qnil;
}

void Init_monster_engine_server(void) {
  rb_cMonsterEngineServer = rb_define_class_under(rb_mMonsterEngine, "Server", rb_cObject);
  rb_define_singleton_method(rb_cMonsterEngineServer, "start", start, 2);
}
