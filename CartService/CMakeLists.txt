set(CART_SERVICE_SOURCES
    src/main.cpp
    controllers/CartController.cpp
    services/CartService.cpp
    repositories/CartRepository.cpp
    routes/CartRoutes.cpp
)

add_executable(cart_service ${CART_SERVICE_SOURCES})

target_include_directories(cart_service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cart_service
    PRIVATE
        shared_core
)
