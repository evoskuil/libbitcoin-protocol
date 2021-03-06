###############################################################################
#  Copyright (c) 2014-2020 libbitcoin-protocol developers (see COPYING).
#
#         GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
#
###############################################################################

# libbitcoin-protocol project configuration.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(libbitcoin-protocol LANGUAGES C CXX)

enable_testing()

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )
include(CheckIncludeFiles)
include(CheckSymbolExists)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
  set( CANONICAL_LIB_NAME libbitcoin-protocol )
else ()
  set( CANONICAL_LIB_NAME bitcoin-protocol )
  find_package( PkgConfig REQUIRED )

  set( prefix "${CMAKE_PREFIX_PATH}" )
  set( exec_prefix "\${prefix}" )
  set( libdir "\${exec_prefix}/lib" )
  set( includedir "\${exec_prefix}/include" )

  set( PACKAGE_VERSION "4.0.0" )
  set( VERSION "${PACKAGE_VERSION}" )
endif ()

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Add compiler options
#------------------------------------------------------------------------------
# Warn on all stuff.
add_compile_options( "-Wall" )

# Warn on extra stuff.
add_compile_options( "-Wextra" )

# Be really annoying.
add_compile_options( "-Wpedantic" )

# Disallow warning on style order of declarations.
add_compile_options( "-Wno-reorder" )

# Suppress warning for incomplete field initialization.
add_compile_options( "-Wno-missing-field-initializers" )

# Conform to style.
add_compile_options( "-Wno-missing-braces" )

# Ignore comments within comments or commenting of backslash extended lines.
add_compile_options( "-Wno-comment" )

# Conflict in stdlib under clang.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options( "-Wno-mismatched-tags" )
endif()

# Implement -Dpkgconfigdir and output ${pkgconfigdir}.
#------------------------------------------------------------------------------
set( pkgconfigdir "${libdir}/pkgconfig" CACHE PATH "Path to pkgconfig directory." )

# Implement -Dwith-tests and declare with-tests.
#------------------------------------------------------------------------------
set( with-tests "yes" CACHE BOOL "Compile with unit tests." )

# Implement -Dwith-mbedtls and output ${mbedtls}.
#------------------------------------------------------------------------------
set( with-mbedtls "no" CACHE BOOL "Compile with MbedTLS." )

if (with-mbedtls)
    set( mbedtls "-DWITH_MBEDTLS" )
endif()

# Implement -Denable-ndebug and define NDEBUG.
#------------------------------------------------------------------------------
set( enable-ndebug "yes" CACHE BOOL "Compile without debug assertions." )

if (enable-ndebug)
    add_definitions( -DNDEBUG )
endif()

# Inherit -Denable-shared and define BOOST_ALL_DYN_LINK.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

# Find boost
#------------------------------------------------------------------------------
find_package( Boost 1.72.0 REQUIRED COMPONENTS
    unit_test_framework )

set( boost_unit_test_framework_LIBS "-lboost_unit_test_framework" )

if (enable-ndebug)
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_DEBUG}" )
else ()
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_RELEASE}" )
endif()

set( boost_CPPFLAGS "-I${Boost_INCLUDE_DIR}" )
set( boost_LDFLAGS "-L${Boost_LIBRARY_DIR}" )

# Find zmq
#------------------------------------------------------------------------------
find_package( Zmq 4.3.4 REQUIRED )

# Find mbedtls
#------------------------------------------------------------------------------
if (with-mbedtls)
    find_package( Mbedtls REQUIRED )
endif()

# Find bitcoin-system
#------------------------------------------------------------------------------
find_package( Bitcoin-System 4.0.0 REQUIRED )

# Define project common includes directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    include_directories( SYSTEM
        ${zmq_INCLUDE_DIRS}
        ${mbedtls_INCLUDE_DIRS}
        ${bitcoin_system_INCLUDE_DIRS} )
else()
    include_directories( SYSTEM
        ${zmq_STATIC_INCLUDE_DIRS}
        ${mbedtls_STATIC_INCLUDE_DIRS}
        ${bitcoin_system_STATIC_INCLUDE_DIRS} )
endif()

# Define project common library directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_directories(
        ${zmq_LIBRARY_DIRS}
        ${mbedtls_LIBRARY_DIRS}
        ${bitcoin_system_LIBRARY_DIRS} )
else()
    link_directories(
        ${zmq_STATIC_LIBRARY_DIRS}
        ${mbedtls_STATIC_LIBRARY_DIRS}
        ${bitcoin_system_STATIC_LIBRARY_DIRS} )
endif()

# Define project common libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_libraries(
        "-fstack-protector"
        "-fstack-protector-all"
        ${zmq_LIBRARIES}
        ${mbedtls_LIBRARIES}
        ${bitcoin_system_LIBRARIES} )
else()
    link_libraries(
        "-fstack-protector"
        "-fstack-protector-all"
        ${zmq_STATIC_LIBRARIES}
        ${mbedtls_STATIC_LIBRARIES}
        ${bitcoin_system_STATIC_LIBRARIES} )
endif()

add_definitions(
    ${mbedtls} )

# Define ${CANONICAL_LIB_NAME} project.
#------------------------------------------------------------------------------
add_library( ${CANONICAL_LIB_NAME}
    "../../src/settings.cpp"
    "../../src/web/connection.cpp"
    "../../src/web/http_reply.cpp"
    "../../src/web/http_request.cpp"
    "../../src/web/json_string.cpp"
    "../../src/web/manager.cpp"
    "../../src/web/socket.cpp"
    "../../src/web/utilities.cpp"
    "../../src/web/websocket_frame.cpp"
    "../../src/zmq/authenticator.cpp"
    "../../src/zmq/certificate.cpp"
    "../../src/zmq/context.cpp"
    "../../src/zmq/frame.cpp"
    "../../src/zmq/identifiers.cpp"
    "../../src/zmq/message.cpp"
    "../../src/zmq/poller.cpp"
    "../../src/zmq/socket.cpp"
    "../../src/zmq/worker.cpp"
    "../../src/zmq/zeromq.cpp" )

# ${CANONICAL_LIB_NAME} project specific include directories.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${zmq_INCLUDE_DIRS}
        ${mbedtls_INCLUDE_DIRS}
        ${bitcoin_system_INCLUDE_DIRS} )
else()
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${zmq_STATIC_INCLUDE_DIRS}
        ${mbedtls_STATIC_INCLUDE_DIRS}
        ${bitcoin_system_STATIC_INCLUDE_DIRS} )
endif()

target_include_directories( ${CANONICAL_LIB_NAME} PUBLIC
"../../include" )

# ${CANONICAL_LIB_NAME} project specific libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${zmq_LIBRARIES}
        ${mbedtls_LIBRARIES}
        ${bitcoin_system_LIBRARIES} )
else()
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${zmq_STATIC_LIBRARIES}
        ${mbedtls_STATIC_LIBRARIES}
        ${bitcoin_system_STATIC_LIBRARIES} )
endif()

# Define libbitcoin-protocol-test project.
#------------------------------------------------------------------------------
if (with-tests)
    add_executable( libbitcoin-protocol-test
        "../../test/converter.cpp"
        "../../test/main.cpp"
        "../../test/utility.hpp"
        "../../test/zmq/authenticator.cpp"
        "../../test/zmq/certificate.cpp"
        "../../test/zmq/context.cpp"
        "../../test/zmq/frame.cpp"
        "../../test/zmq/identifiers.cpp"
        "../../test/zmq/message.cpp"
        "../../test/zmq/poller.cpp"
        "../../test/zmq/socket.cpp"
        "../../test/zmq/worker.cpp" )

    add_test( NAME libbitcoin-protocol-test COMMAND libbitcoin-protocol-test
            --run_test=*
            --show_progress=no
            --detect_memory_leak=0
            --report_level=no
            --build_info=yes )

#     libbitcoin-protocol-test project specific include directories.
#------------------------------------------------------------------------------
    target_include_directories( libbitcoin-protocol-test PRIVATE
        "../../include" )

#     libbitcoin-protocol-test project specific libraries/linker flags.
#------------------------------------------------------------------------------
    target_link_libraries( libbitcoin-protocol-test
        ${CANONICAL_LIB_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

endif()

# Manage pkgconfig installation.
#------------------------------------------------------------------------------
configure_file(
  "../../libbitcoin-protocol.pc.in"
  "libbitcoin-protocol.pc" @ONLY )

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libbitcoin-protocol.pc"
    DESTINATION "${pkgconfigdir}" )

# Manage installation of docs.
#------------------------------------------------------------------------------
install( FILES
    "../../AUTHORS"
    "../../COPYING"
    "../../ChangeLog"
    "../../INSTALL"
    "../../NEWS"
    "../../README"
    DESTINATION share/doc/libbitcoin-protocol )

# Manage ${CANONICAL_LIB_NAME} installation.
#------------------------------------------------------------------------------
install( TARGETS ${CANONICAL_LIB_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include )

# Manage include installation.
#------------------------------------------------------------------------------
install( DIRECTORY "../../include/bitcoin"
    DESTINATION include )

