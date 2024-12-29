cmake_minimum_required(VERSION 3.10)
project(facebook_system)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler paths (only if not already set)
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "D:/Msys2/mingw64/bin/gcc.exe" CACHE STRING "C compiler" FORCE)
endif()
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "D:/Msys2/mingw64/bin/g++.exe" CACHE STRING "C++ compiler" FORCE)
endif()

# wxWidgets configuration
set(wxWidgets_ROOT_DIR "D:/Msys2/mingw64")
set(wxWidgets_LIB_DIR "D:/Msys2/mingw64/lib")
set(wxWidgets_CONFIGURATION mswu)
set(wxWidgets_VERSION 3.0)

# Add definitions for wxWidgets
add_definitions(-D__WXMSW__ -DWXUSINGDLL -D_UNICODE -DwxUSE_UNICODE)

# Add source files
set(SOURCE_FILES
    src/FacebookSystem.cpp
    src/User.cpp
    src/Post.cpp
    src/Comment.cpp
    src/Message.cpp
    src/Conversation.cpp
    src/FileManager.cpp
)

# Add GUI files
set(GUI_FILES
    gui/MainWindow.cpp
    gui/CommentDialog.cpp
    gui/MessageDialog.cpp
    gui/ResetPasswordDialog.cpp
)

# Add header files
set(HEADER_FILES
    include/User.h
    include/Post.h
    include/Comment.h
    include/FacebookSystem.h
    include/FileManager.h
    gui/MainWindow.h
    gui/CommentDialog.h
    gui/MessageDialog.h
    gui/ResetPasswordDialog.h
    gui/gui_manager.h
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/gui)
include_directories("D:/Msys2/mingw64/include")
include_directories("D:/Msys2/mingw64/include/wx-3.0")
include_directories("D:/Msys2/mingw64/lib/wx/include/msw-unicode-3.0")

# Add executable
add_executable(facebook_app
    src/main.cpp
    ${SOURCE_FILES}
    ${GUI_FILES}
)

# Link wxWidgets libraries
target_link_libraries(facebook_app
    "D:/Msys2/mingw64/lib/libwx_mswu_core-3.0.dll.a"
    "D:/Msys2/mingw64/lib/libwx_baseu-3.0.dll.a"
    "D:/Msys2/mingw64/lib/libwx_mswu_richtext-3.0.dll.a"
    "D:/Msys2/mingw64/lib/libwx_mswu_adv-3.0.dll.a"
)

# Enable testing
enable_testing()

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Add test executable
add_executable(unit_tests
    tests/user_tests.cpp
    tests/post_comment_tests.cpp
    tests/facebook_system_tests.cpp
    tests/messaging_tests.cpp
    ${SOURCE_FILES}
)

target_link_libraries(unit_tests
    gtest_main
    gmock_main
    ${wxWidgets_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(unit_tests)