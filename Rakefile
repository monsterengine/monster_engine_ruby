require "bundler/gem_tasks"
require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("monster_engine") do |ext|
  ext.lib_dir = "lib/monster_engine"
end

task :default => [:clobber, :compile, :spec]
