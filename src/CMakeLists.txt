include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# except library
add_library(except assert.c except.c)
# test for except library
add_executable(test_except test_except.c)
target_link_libraries(test_except except)
add_executable(test_signal test_signal.c)
target_link_libraries(test_signal except)
# memory management library (two implementations)
add_library(mem mem.c)
target_link_libraries(mem except)
add_library(memchk memchk.c)
target_link_libraries(memchk except)
# atom library
add_library(atom atom.c scatter.c)
target_link_libraries(atom memchk except)
# arena memory management library
add_library(arena arena.c)
target_link_libraries(arena except)
# list library
add_library(list list.c)
target_link_libraries(list except memchk)
add_executable(test_list test_list.c)
target_link_libraries(test_list list atom)
# table libray
add_library(table table.c)
target_link_libraries(table except memchk)
add_executable(wf getword.c wf.c)
target_link_libraries(wf table atom)
# text library
add_library(text text.c)
target_link_libraries(text except memchk)
add_executable(test_text test_text.c)
target_link_libraries(test_text text)
# thread library
add_library(thread thread.c chan.c)
target_link_libraries(thread mem except)
