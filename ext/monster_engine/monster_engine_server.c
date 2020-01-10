#include "monster_engine.h"
#include <ruby/thread.h>
#include <sys/types.h>
#include <unistd.h>

extern const rb_data_type_t rb_plamo_app_type;
extern VALUE plamo_rb_event_thread(void*);

VALUE rb_cMonsterEngineServer;

static void deallocate(void *monster_engine_server) {
  monster_engine_server_destroy(monster_engine_server);
}

const rb_data_type_t rb_monster_engine_server_type = {
  "Server",
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
  return TypedData_Wrap_Struct(klass, &rb_monster_engine_server_type, NULL);
}

static VALUE initialize(VALUE self, VALUE rb_plamo_app, VALUE rb_monster_engine_config) {
  rb_iv_set(self, "@config", rb_monster_engine_config);
  PlamoApp *plamo_app;
  TypedData_Get_Struct(rb_plamo_app, PlamoApp, &rb_plamo_app_type, plamo_app);
  MonsterEngineConfig *monster_engine_config;
  TypedData_Get_Struct(rb_monster_engine_config, MonsterEngineConfig, &rb_monster_engine_config_type, monster_engine_config);
  DATA_PTR(self) = monster_engine_server_new(plamo_app, monster_engine_config);
  return self;
}

typedef struct MonsterEngineRbCallbackSetting {
  PlamoApp *plamo_app;
  MonsterEngineConfig *monster_engine_config;
} MonsterEngineRbCallbackSetting;

static void* callback(void *monster_engine_server) {
  monster_engine_server_start(monster_engine_server);
  return NULL;
}

static VALUE start(VALUE self) {
  VALUE rb_config = rb_iv_get(self, "@config");
  const unsigned int workers = FIX2UINT(rb_iv_get(rb_config, "@workers"));
  for (int i = 0; i < workers; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      rb_thread_create(plamo_rb_event_thread, NULL);
      rb_thread_call_without_gvl(callback, DATA_PTR(self), RUBY_UBF_PROCESS, NULL);
      break;
    } else if (pid == -1) {
      break;
    }
  }

  while (true) {
    sleep(1);
  }

  return Qnil;
}

void Init_monster_engine_server(void) {
  rb_cMonsterEngineServer = rb_define_class_under(rb_mMonsterEngine, "Server", rb_cObject);
  rb_define_method(rb_cMonsterEngineServer, "initialize", initialize, 2);
  rb_define_method(rb_cMonsterEngineServer, "start", start, 0);
}
