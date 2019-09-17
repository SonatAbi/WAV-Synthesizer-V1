
add_executable(compare compare.cpp)
INSTALL(TARGETS compare DESTINATION bin)

if(WIN32)
  set(EXE_NAME synth.exe)
  set(COMPARE_EXE_NAME compare.exe)
else()
  set(EXE_NAME synth)
  set(COMPARE_EXE_NAME compare)
endif()

# these tests should pass
foreach(ID RANGE 9)
add_test(NAME test${ID}
  COMMAND ${CMAKE_COMMAND}
  -DCOMPARE_EXE_FILE=${CMAKE_BINARY_DIR}/bin/${COMPARE_EXE_NAME}
  -DEXE_FILE=${CMAKE_BINARY_DIR}/bin/${EXE_NAME}
    -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/test${ID}.csv
    -DOUTPUT_FILE=test${ID}.wav
    -DERR_FILE=test${ID}.stderr
    -DOUT_FILE=test${ID}.stdout
    -DTEST_FILE=${CMAKE_SOURCE_DIR}/tests/test${ID}.wav
    -P ${CMAKE_CURRENT_SOURCE_DIR}/Test.cmake
    )
endforeach(ID)

# these tests should not succeeed
add_test(test10 ${CMAKE_BINARY_DIR}/bin/${EXE_NAME} ${CMAKE_SOURCE_DIR}/tests/test10.csv test10.wav)
add_test(test11 ${CMAKE_BINARY_DIR}/bin/${EXE_NAME} ${CMAKE_SOURCE_DIR}/tests/test11.csv test11.wav)
add_test(test12 ${CMAKE_BINARY_DIR}/bin/${EXE_NAME} ${CMAKE_SOURCE_DIR}/tests/test12.csv test12.wav)

set_tests_properties(test10 PROPERTIES WILL_FAIL ON)
set_tests_properties(test11 PROPERTIES WILL_FAIL ON)
set_tests_properties(test12 PROPERTIES WILL_FAIL ON)
