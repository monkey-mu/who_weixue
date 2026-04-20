set(UI_DIR ../ui)
file(GLOB_RECURSE UI_SRCS ${UI_DIR}/*.c ${UI_DIR}/*.cpp)

idf_component_register(SRCS "main.c"
${UI_SRCS} 
INCLUDE_DIRS "." ${UI_DIR})
