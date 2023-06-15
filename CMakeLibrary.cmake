
# Macro which ensures that certain compiler warnings are treated as compiler errors.
macro(dx_configure_warnings target)

if(MSVC)

  set(COMPILE_OPTIONS "")
  
  # C4090:  'opertion' : different 'modifier' qualifiers
  list(APPEND COMPILE_OPTIONS "/we4090")
  # C4133: 'function' incompatible types - from 'type1' to 'type2'
  list(APPEND COMPILE_OPTIONS "/we4133")
  # C4020_ 'function': too many actual parameters
  list(APPEND COMPILE_OPTIONS "/we4020")
  # C4013: 'function': undefined; assuming extern returning int
  list(APPEND COMPILE_OPTIONS "/we4013")
  # C4024: 'function': different types for formal and actual parameter 'number'
  list(APPEND COMPILE_OPTIONS "/we4024")
  
  # The quotes around "${COMPILE_OPTIONS}" concatenate the list elements to a single string separated by semicolons.
  set_source_files_properties(${${name}.sources} PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")
  set_source_files_properties(${${name}.headers} PROPERTIES COMPILE_OPTIONS "${COMPILE_OPTIONS}")

endif()

endmacro()
