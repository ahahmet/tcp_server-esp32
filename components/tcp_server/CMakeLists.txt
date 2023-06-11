set(SOURCE_FILE_LIST
    ./src/tcp_server.c
    ./src/parser.c
)

idf_component_register(
    SRCS 
        "${SOURCE_FILE_LIST}"
    INCLUDE_DIRS
        "inc"
    REQUIRES
        lwip
        periphs
        json
)


idf_component_set_property(${COMPONENT_NAME} COMPILE_OPTIONS "-Wall" APPEND)
idf_component_set_property(${COMPONENT_NAME} COMPILE_OPTIONS "-Wextra" APPEND)
idf_component_set_property(${COMPONENT_NAME} COMPILE_OPTIONS "-Werror" APPEND)

set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)
