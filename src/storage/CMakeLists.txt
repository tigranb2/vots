add_library(
        vots_storage
        OBJECT
        list_node.cpp
        red_black_tree.cpp
    )

target_link_libraries(
        vots_storage
        # lib
    )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:vots_storage>
        PARENT_SCOPE)
