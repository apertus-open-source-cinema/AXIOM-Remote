CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(AXIOM_Remote_Firmware_Test)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic --coverage -fprofile-arcs -ftest-coverage")

SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

INCLUDE_DIRECTORIES("../Bootloader")

FILE(GLOB FIRMWARE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/Painter/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/UI/Screens/*.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/Attribute.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/Attribute.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/CentralDB.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/CentralDB/CentralDBObserver.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../Firmware/GlobalSettings.cpp")

SET(TEST_SOURCES Main.cpp PainterTest.cpp MenuSystemTest.cpp CentralDBObserverTest.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_SOURCES} ${FIRMWARE_SOURCES})

# Code coverage
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ./${PROJECT_NAME}
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/coverage_report
                   COMMAND gcovr -r ../../Firmware/ . --html --html-details -o ${PROJECT_BINARY_DIR}/coverage_report/coverage_report.html
                   COMMAND gcovr -r ../../Firmware/ . --xml -o ${PROJECT_BINARY_DIR}/coverage_report/coverage_report.xml)
