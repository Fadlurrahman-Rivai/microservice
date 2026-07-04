set(PAYMENT_SERVICE_SOURCES
    src/main.cpp
    controllers/PaymentController.cpp
    services/PaymentService.cpp
    repositories/PaymentRepository.cpp
    routes/PaymentRoutes.cpp
)

add_executable(payment_service ${PAYMENT_SERVICE_SOURCES})

target_include_directories(payment_service
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(payment_service
    PRIVATE
        shared_core
)
