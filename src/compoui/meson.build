lib_compoui = static_library(
  'compoui',
  [
    'glad/glad.c',
    'cui.cpp',
    'util/shader.cpp',
    'elements/primitive/primitive.cpp',
    'elements/primitive/fill.cpp',
  ],
  dependencies: [dependency('gl'), dependency('glu')],
  link_args: ['-lglad', '-ldl']
)