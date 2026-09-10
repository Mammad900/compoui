lib_compoui = static_library(
  'compoui',
  [
    'glad/glad.c',
    'cui.cpp',
    'hitbox.cpp',
    'texture.cpp',
    'util/shader.cpp',
    'font/font.cpp',
    'elements/primitive/primitive.cpp',
    'elements/primitive/fill.cpp',
    'elements/primitive/image.cpp',
    'elements/primitive/text.cpp',
  ],
  dependencies: [
    dependency('gl'), 
    dependency('glu'),
    dependency('freetype2'),
  ],
  link_args: ['-lglad', '-ldl']
)