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
