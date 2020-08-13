project('strliteral','cpp',default_options:['buildtype=release','cpp_std=c++17'],version:'1.0')
strliteral=executable('strliteral','strliteral.cpp',install:not get_option('build_tool'),pie:true,native:get_option('build_tool'))

if get_option('build_test')
subdir('test')
endif
