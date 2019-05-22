require "mkmf"

dir_config('plamo', "/usr/include", "/usr/lib")

unless find_header("libmonsterengine.h")
  abort "libplamo is missing. please install libmonsterengine"
end

unless find_library("monsterengine", "monster_engine_server_start")
  abort "libplamo is missing. please install libmonsterengine"
end

create_makefile("monster_engine/monster_engine")
