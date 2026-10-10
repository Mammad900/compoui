project('compoui', ['cpp', 'c'],
  version: '0.1',
  default_options : ['cpp_std=c++23']
)

subdir('src/compoui')
fs = import('fs')

if fs.exists('src/app/meson.build')
    subdir('src/app')
endif