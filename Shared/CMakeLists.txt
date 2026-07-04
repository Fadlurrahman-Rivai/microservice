set(SHARED_SOURCES
    src/Config.cpp
    src/Logger.cpp
    src/Database.cpp
    src/HttpClient.cpp
)

add_library(shared_core STATIC ${SHARED_SOURCES})

target_include_directories(shared_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(shared_core
    PUBLIC
        Crow::Crow
        cpr::cpr
        nlohmann_json::nlohmann_json
        unofficial::mysql-connector-cpp::connector
)
