add_library(
        vots_storage
        OBJECT
        red_black_tree.cpp
    )

target_link_libraries(
        vots_storage
        Microsoft.GSL::GSL
    )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:vots_storage>
        PARENT_SCOPE)

