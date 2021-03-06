function(enable_doxygen SOURCE_DIR)
  option(ENABLE_DOXYGEN "Enable doxygen doc builds of source" OFF)
  if(ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_GENERATE_HTML NO)
	set(DOXYGEN_GENERATE_LATEX YES)
	set(OUTPUT_LANGUAGE "polish")
    find_package(Doxygen REQUIRED)
	doxygen_add_docs(doxygen-docs ${SOURCE_DIR})

  endif()
endfunction()
