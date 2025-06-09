add_library(
        vots_storage
        OBJECT
        linked_list.cpp
        red_black_tree.cpp
    )

target_link_libraries(
        vots_storage
        Boost::boost
        # lib
    )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:vots_storage>
        PARENT_SCOPE)
