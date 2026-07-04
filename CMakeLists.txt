cmake_minimum_required(VERSION 3.20)

project(MarketplaceMicroservices LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_PRODUCT_SERVICE "Build ProductService" ON)
option(BUILD_CART_SERVICE "Build CartService" ON)
option(BUILD_INVENTORY_SERVICE "Build InventoryService" ON)
option(BUILD_PAYMENT_SERVICE "Build PaymentService" ON)

find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

add_subdirectory(Shared)

if(BUILD_PRODUCT_SERVICE)
    add_subdirectory(ProductService)
endif()

if(BUILD_CART_SERVICE)
    add_subdirectory(CartService)
endif()

if(BUILD_INVENTORY_SERVICE)
    add_subdirectory(InventoryService)
endif()

if(BUILD_PAYMENT_SERVICE)
    add_subdirectory(PaymentService)
endif()
    