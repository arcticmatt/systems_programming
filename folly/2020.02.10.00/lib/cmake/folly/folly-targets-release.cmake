#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Folly::folly" for configuration "Release"
set_property(TARGET Folly::folly APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Folly::folly PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfolly.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libfolly.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Folly::folly )
list(APPEND _IMPORT_CHECK_FILES_FOR_Folly::folly "${_IMPORT_PREFIX}/lib/libfolly.dylib" )

# Import target "Folly::folly_test_util" for configuration "Release"
set_property(TARGET Folly::folly_test_util APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Folly::folly_test_util PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfolly_test_util.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libfolly_test_util.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Folly::folly_test_util )
list(APPEND _IMPORT_CHECK_FILES_FOR_Folly::folly_test_util "${_IMPORT_PREFIX}/lib/libfolly_test_util.dylib" )

# Import target "Folly::follybenchmark" for configuration "Release"
set_property(TARGET Folly::follybenchmark APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Folly::follybenchmark PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfollybenchmark.dylib"
  IMPORTED_SONAME_RELEASE "@rpath/libfollybenchmark.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Folly::follybenchmark )
list(APPEND _IMPORT_CHECK_FILES_FOR_Folly::follybenchmark "${_IMPORT_PREFIX}/lib/libfollybenchmark.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
