project(
  'Pixine',
  'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++17',
    'buildtype=debug',
    'b_pch=false',
  ],
)

add_global_arguments('/utf-8', language: 'cpp')

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language: 'cpp')
endif

sfml_dep = dependency('sfml', fallback: ['sfml', 'sfml_dep'])
deps = [sfml_dep]

subdir('src')
inc = include_directories('include')

executable('main', src, include_directories: inc, dependencies: deps)
