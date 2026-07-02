cmake_minimum_required(VERSION 3.14)

project(ShopApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Подключение необходимых модулей Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Charts PrintSupport)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/.env")
    message(STATUS "Файл .env не найден. Копируем шаблон .env.dev...")
    file(COPY "${CMAKE_SOURCE_DIR}/.env.dev" DESTINATION "${CMAKE_BINARY_DIR}")
    file(RENAME "${CMAKE_BINARY_DIR}/.env.dev" "${CMAKE_BINARY_DIR}/.env")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/views
    ${CMAKE_SOURCE_DIR}/src/views/auth
    ${CMAKE_SOURCE_DIR}/src/views/managers
    ${CMAKE_SOURCE_DIR}/src/views/reports
    ${CMAKE_SOURCE_DIR}/src/views/admin
)

set(CORE_SOURCES
    src/core/databasemanager.cpp
    src/core/databasemanager.h
    src/core/reportexporter.cpp
    src/core/reportexporter.h
    src/core/session.cpp
    src/core/session.h
    src/core/utils.h
)

set(VIEW_SOURCES
    src/views/mainwindow.cpp
    src/views/mainwindow.h
    src/views/mainwindow.ui
    src/views/auth/logindialog.cpp
    src/views/auth/logindialog.h
    src/views/auth/logindialog.ui
    src/views/admin/dictionarydialog.cpp
    src/views/admin/dictionarydialog.h
    src/views/admin/dictionarydialog.ui
    src/views/admin/productsdialog.cpp
    src/views/admin/productsdialog.h
    src/views/admin/productsdialog.ui
    src/views/admin/suppliersdialog.cpp
    src/views/admin/suppliersdialog.h
    src/views/admin/suppliersdialog.ui
    src/views/admin/suppliesdialog.cpp
    src/views/admin/suppliesdialog.h
    src/views/admin/suppliesdialog.ui
    src/views/admin/usersdialog.cpp
    src/views/admin/usersdialog.h
    src/views/admin/usersdialog.ui
    src/views/managers/clientsdialog.cpp
    src/views/managers/clientsdialog.h
    src/views/managers/clientsdialog.ui
    src/views/managers/orderprocessingdialog.cpp
    src/views/managers/orderprocessingdialog.h
    src/views/managers/orderprocessingdialog.ui
    src/views/managers/ordersdialog.cpp
    src/views/managers/ordersdialog.h
    src/views/managers/ordersdialog.ui
    src/views/managers/catalogdialog.cpp
    src/views/managers/catalogdialog.h
    src/views/managers/catalogdialog.ui
    src/views/reports/reportsdialog.cpp
    src/views/reports/reportsdialog.h
    src/views/reports/reportsdialog.ui
    src/views/reports/chartviewdialog.cpp
    src/views/reports/chartviewdialog.h
    src/views/reports/chartviewdialog.ui
)

set(MAIN_SOURCE src/main.cpp)

add_executable(ShopApp
    ${MAIN_SOURCE}
    ${CORE_SOURCES}
    ${VIEW_SOURCES}
)

target_link_libraries(ShopApp PRIVATE Qt5::Widgets Qt5::Sql Qt5::Charts Qt5::PrintSupport)
