project('compoui', ['cpp', 'c'],
  version: '0.1',
  default_options : ['cpp_std=c++23']
)

subdir('src/compoui')

# Test application
executable('test_app',
  ['src/app/main.cpp'],
  dependencies : [dependency('sfml')],
  link_with : lib_compoui
)
