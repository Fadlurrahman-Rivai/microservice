set(PRODUCT_SERVICE_SOURCES
    src/main.cpp
    controllers/ProductController.cpp
    services/ProductService.cpp
    repositories/ProductRepository.cpp
    routes/ProductRoutes.cpp
    models/ProductModels.cpp
)

add_executable(product_service ${PRODUCT_SERVICE_SOURCES})

target_include_directories(product_service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(product_service
    PRIVATE
        shared_core
)
