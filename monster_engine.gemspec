
lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "monster_engine/version"

Gem::Specification.new do |spec|
  spec.name          = "monster_engine"
  spec.version       = MonsterEngine::VERSION
  spec.authors       = ["Shogo Otake"]
  spec.email         = ["shogo.otake@gmail.com"]

  spec.summary       = "Ruby Bindings for libmonsterengine"
  spec.homepage      = "https://monsterengine.org"
  spec.license       = "MIT"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/monsterengine/monster_engine_ruby"
  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/monster_engine/extconf.rb"]

  spec.add_development_dependency "bundler", "~> 2.0"
  spec.add_development_dependency "rake", "~> 13.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "plamo", "~> 0.4.0"
end
