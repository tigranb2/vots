add_library(
        vots_orderbook
        OBJECT
        order_book.cpp
    )

target_link_libraries(
        vots_orderbook
        Boost::boost
    )

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:vots_orderbook>
        PARENT_SCOPE)

