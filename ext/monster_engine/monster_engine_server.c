#include "monster_engine.h"
#include "wrapper.h"
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

static void callback_ubf(void *value) {

}

static VALUE start(VALUE self, VALUE rb_plamo_app, VALUE rb_monster_engine_config) {
  Wrapper *plamo_app_wrapper;
  Data_Get_Struct(rb_plamo_app, Wrapper, plamo_app_wrapper);
  Wrapper *monster_engine_config_wrapper;
  Data_Get_Struct(rb_monster_engine_config, Wrapper, monster_engine_config_wrapper);
  MonsterEngineRbCallbackSetting *monster_engine_rb_callback_setting = malloc(sizeof(MonsterEngineRbCallbackSetting));
  monster_engine_rb_callback_setting->plamo_app = plamo_app_wrapper->inner;
  monster_engine_rb_callback_setting->monster_engine_config = monster_engine_config_wrapper->inner;

  // rb_thread_call_with_gvl(callback, monster_engine_rb_callback_setting);
  // rb_thread_call_without_gvl(callback, monster_engine_rb_callback_setting, callback_ubf, NULL);
  rb_thread_call_without_gvl(callback, monster_engine_rb_callback_setting, RUBY_UBF_PROCESS, NULL);
  // monster_engine_server_start(plamo_app_wrapper->inner, monster_engine_config_wrapper->inner);
  return Qnil;
}

void Init_monster_engine_server(void) {
  rb_cMonsterEngineServer = rb_define_class_under(rb_mMonsterEngine, "Server", rb_cObject);
  rb_define_singleton_method(rb_cMonsterEngineServer, "start", start, 2);
}
