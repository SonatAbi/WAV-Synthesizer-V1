# Run the program and capture output

execute_process( 
  COMMAND ${EXE_FILE} ${INPUT_FILE} ${OUTPUT_FILE}
  ERROR_FILE ${ERR_FILE}
  OUTPUT_FILE ${OUT_FILE}
  )

# Compare the output to the expected
execute_process( 
  COMMAND ${COMPARE_EXE_FILE} ${OUTPUT_FILE} ${TEST_FILE}
  RESULT_VARIABLE DIFFERENT
  )

if(DIFFERENT)
  message(FATAL_ERROR "Test failed - synth output incorrect")
endif()
