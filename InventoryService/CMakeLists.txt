set(INVENTORY_SERVICE_SOURCES
    src/main.cpp
    controllers/InventoryController.cpp
    services/InventoryService.cpp
    repositories/InventoryRepository.cpp
    routes/InventoryRoutes.cpp
)

add_executable(inventory_service ${INVENTORY_SERVICE_SOURCES})

target_include_directories(inventory_service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(inventory_service
    PRIVATE
        shared_core
)
