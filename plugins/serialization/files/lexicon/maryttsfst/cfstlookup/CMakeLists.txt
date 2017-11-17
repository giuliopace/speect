cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

set(CFSTLOOKUP_MAJOR_VERSION 1)
set(CFSTLOOKUP_MINOR_VERSION 0)
set(CFSTLOOKUP_PATCH_VERSION 0)
set(CFSTLOOKUP_VERSION ${CFSTLOOKUP_MAJOR_VERSION}.${CFSTLOOKUP_MINOR_VERSION}.${CFSTLOOKUP_PATCH_VERSION})
project(cfstlookup VERSION ${CFSTLOOKUP_VERSION} LANGUAGES C)

set(HEADER_FILES
  src/cfstlookup/cfstlookup.h
  src/cfstlookup/defines.h
  src/stuffd/common.h
  src/stuffd/read.h
  src/template-private.h
  src/ieee754.h
  )

add_library(cfstlookup STATIC
  src/read.c
  src/fstlib.c
  src/ieee754.c
  ${HEADER_FILES}
  )
target_include_directories(cfstlookup PRIVATE src)
set_target_properties(cfstlookup
  PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  )

add_executable(cfstlookup-dictionary-lookup-test tests/cfstlookup_dictionary_lookup_test.c)
target_link_libraries(cfstlookup-dictionary-lookup-test cfstlookup)
target_include_directories(cfstlookup-dictionary-lookup-test PRIVATE src)

install(TARGETS cfstlookup EXPORT cfstlookuport-targets
  RUNTIME DESTINATION bin COMPONENT bin
  ARCHIVE DESTINATION lib COMPONENT dev
  LIBRARY DESTINATION lib COMPONENT shlib
  PUBLIC_HEADER DESTINATION include COMPONENT dev)
install(DIRECTORY src/cfstlookup DESTINATION include COMPONENT dev FILES_MATCHING PATTERN "*.h")

export(TARGETS cfstlookup FILE "${PROJECT_BINARY_DIR}/cfstlookup-targets.cmake")
export(PACKAGE cfstlookup)

# Create the Config.cmake and ConfigVersion.cmake files

get_filename_component(CMAKE_INSTALL_INCLUDEDIR_ABS include ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
set(CONF_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR_ABS})
configure_file(cfstlookupConfig.cmake.in "cmake/cfstlookupConfig.cmake" @ONLY)
configure_file(cfstlookupConfigVersion.cmake.in "cmake/cfstlookupConfigVersion.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/cmake/cfstlookupConfig.cmake" "${PROJECT_BINARY_DIR}/cmake/cfstlookupConfigVersion.cmake" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cfstlookup" COMPONENT dev)

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
configure_file(cfstlookupConfig.cmake.in "cfstlookupConfig.cmake" @ONLY)
configure_file(cfstlookupConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/cfstlookupConfigVersion.cmake" @ONLY)

enable_testing()

macro(add_lookup_test dictionary lookup_string found_string)
  get_filename_component(tmp ${dictionary} NAME)
  set(test_name "cfstlookup-dictionary-lookup-${tmp}-${lookup_string}")
  foreach(arg ${ARGN})
    set(test_name "${test_name}-${arg}")
  endforeach()
  add_test("${test_name}" ${TARGET_SYSTEM_EMULATOR} cfstlookup-dictionary-lookup-test${CMAKE_EXECUTABLE_SUFFIX} ${dictionary} ${lookup_string})
  set_tests_properties("${test_name}" PROPERTIES PASS_REGULAR_EXPRESSION ${lookup_string}.*found.*${found_string})
endmacro(add_lookup_test)

add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0AB1ABIABWACR functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABEABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABGABZABIABE functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABJACPABJABKABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABKACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABLACPABLABM functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB0ABZACR functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB3ABBABIABJABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst AB3ABNABBAC2 functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABBABNAB5ABN functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABBABNABL functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABD functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABEACFABGABHABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABEACFABGABHACR functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABHABNABYABN functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABHACRABTABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABHACRABTACPABWACG functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABHACRABTACR functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABJABNABKACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABBABLACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABGABKACR functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABHACPABHABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABIABGABZ functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABNABKABIABEABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABYABIABJABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABYACPABY functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLABZACPABZ functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLAC6ABGABW functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLACDABJACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLACGABMABN functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLACPABBABIABWABG functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABLACSABTABG functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABOABEACPABEABNAB1ABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABOABEACPABE functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABOABGABZABEABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABQAB4ACPAB1 functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABQABBACS functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABQACRABBABLACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABRABKACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABRABYABN functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABU functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABVABLACPABL functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABVABL functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABWABIABBABIAB5ABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABWACPABLABBABYACS functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABX functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABYACRABEABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABYACRABZABEABI functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABYACRABZABN functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABYACRABZACF functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABYACS functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABZABNABGABWACS functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABZAC2ABGABWACS functional)
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/new.fst ABZACPABLABNABBABN functional)

add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "NZAMBIBHI" "NZAMBIBHI")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "Zü3DBEF" "Zü3DBEF")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "ZBIA3LBEFBH" "ZBIA3LBEFBH")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "MEIBABHHMBABEFBA" "MEIBABHHMBABEFBA")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "NOHLBAODI" "NOHLBAODI")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "NZSNGBE" "NZSNGBE")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "FOACDTOSPGGBEFBA" "FOACDTOSPGGBEFBA")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "LBüOELBEBA" "LBüOELBEBA")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "LMB2BEF" "LMB2BEF")
add_lookup_test(${CMAKE_CURRENT_SOURCE_DIR}/tests/data/old.fst "HCDEBM" "HCDEBM")
