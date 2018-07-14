aux_source_directory(. DIR_CORE)
include_directories(../../include)
add_compile_options(-O2)
add_library(core_obj ${DIR_CORE})