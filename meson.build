project('cataclysm-bn', 'cpp', default_options: ['default_library=static', 'cpp_std=c++17'])

cpp_args = [ '-DTILES' ]

if meson.get_compiler('cpp').get_id() == 'msvc'
    cpp_args += [
        '/wd4068',
        '/wd4244',
        '/wd4305'
    ]
endif

subdir('src/')
subdir('tools/format')

precompiled_hdr = 'pch/main-pch.hpp'

# SDL build dependencies
thread_dep = dependency('threads')
opengl_dep = dependency('GL')
sdl2_dep = subproject('sdl2').get_variable('sdl2_dep')
sdl2_image_dep = subproject('sdl2_image').get_variable('sdl2_image_dep')
sdl2_ttf_dep = subproject('sdl2_ttf').get_variable('sdl2_ttf_dep')
imgui_dep = subproject('imgui').get_variable('imgui_dep')

if meson.get_compiler('cpp').has_argument('-Wno-shadow')
    cpp_args += [ '-Wno-shadow' ]
endif

lib_dependencies = [
    sdl2_dep,
    sdl2_image_dep,
    sdl2_ttf_dep,
    thread_dep,
    opengl_dep,
    imgui_dep,
]

lib_include_dir = include_directories( 'src/' )

libcata = static_library(
    'cata-lib',
    sources: editor_sources + lib_sources,
    include_directories: lib_include_dir,
    dependencies : lib_dependencies,
    cpp_pch : precompiled_hdr,
    cpp_args : cpp_args,
)

libcata_dep = declare_dependency(
    link_with : libcata,
    include_directories: lib_include_dir,
    dependencies : lib_dependencies,
)

executable(
    'cataclysm-tiles',
    sources : files( 'src/main.cpp' ),
    dependencies : libcata_dep,
    cpp_pch : precompiled_hdr,
    cpp_args : cpp_args,
)
