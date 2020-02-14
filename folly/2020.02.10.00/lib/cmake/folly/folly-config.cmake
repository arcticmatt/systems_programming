# CMake configuration file for folly
#
# This provides the Folly::folly target, which you can depend on by adding it
# to your target_link_libraries().
#
# It also defines the following variables, although using these directly is not
# necessary if you use the Folly::folly target instead.
#  FOLLY_INCLUDE_DIRS
#  FOLLY_LIBRARIES


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was folly-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

set_and_check(FOLLY_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include")
set_and_check(FOLLY_CMAKE_DIR "${PACKAGE_PREFIX_DIR}/lib/cmake/folly")

# find_dependency() ends up changing PACKAGE_PREFIX_DIR, so save
# folly's prefix directory in the FOLLY_PREFIX_DIR variable
set(FOLLY_PREFIX_DIR "${PACKAGE_PREFIX_DIR}")

# Include the folly-targets.cmake file, which is generated from our CMake rules
if (NOT TARGET Folly::folly)
  include("${FOLLY_CMAKE_DIR}/folly-targets.cmake")
endif()

# Set FOLLY_LIBRARIES from our Folly::folly target
set(FOLLY_LIBRARIES Folly::folly)

# Find folly's dependencies
find_dependency(fmt)

if (NOT folly_FIND_QUIETLY)
  message(STATUS "Found folly: ${FOLLY_PREFIX_DIR}")
endif()
