# Library tests:
add_executable(test-evmjit-standalone test-evmjit-standalone.c)
target_include_directories(test-evmjit-standalone PRIVATE ../include)
if (MSVC)
    set(SYSTEM_LIBS "")
elseif (APPLE)
    set(SYSTEM_LIBS stdc++ curses z pthread m)
else()
    set(SYSTEM_LIBS stdc++ ncurses z pthread dl m)
endif()
target_link_libraries(test-evmjit-standalone PRIVATE ${EVMJIT_STANDALONE_LIB} evmc ${SYSTEM_LIBS})
add_dependencies(test-evmjit-standalone evmjit-standalone)


add_test(evmjit-standalone test-evmjit-standalone)
