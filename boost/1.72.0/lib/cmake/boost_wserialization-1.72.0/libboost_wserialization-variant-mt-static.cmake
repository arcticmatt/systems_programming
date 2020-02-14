# Generated by Boost 1.72.0

# address-model=64

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  _BOOST_SKIPPED("libboost_wserialization-mt.a" "64 bit, need 32")
  return()
endif()

# layout=tagged

# toolset=xgcc42

# link=static

if(DEFINED Boost_USE_STATIC_LIBS)
  if(NOT Boost_USE_STATIC_LIBS)
    _BOOST_SKIPPED("libboost_wserialization-mt.a" "static, Boost_USE_STATIC_LIBS=${Boost_USE_STATIC_LIBS}")
    return()
  endif()
else()
  if(NOT WIN32)
    _BOOST_SKIPPED("libboost_wserialization-mt.a" "static, default is shared, set Boost_USE_STATIC_LIBS=ON to override")
    return()
  endif()
endif()

# runtime-link=shared

if(Boost_USE_STATIC_RUNTIME)
  _BOOST_SKIPPED("libboost_wserialization-mt.a" "shared runtime, Boost_USE_STATIC_RUNTIME=${Boost_USE_STATIC_RUNTIME}")
  return()
endif()

# runtime-debugging=off

if(Boost_USE_DEBUG_RUNTIME)
  _BOOST_SKIPPED("libboost_wserialization-mt.a" "release runtime, Boost_USE_DEBUG_RUNTIME=${Boost_USE_DEBUG_RUNTIME}")
  return()
endif()

# threading=multi

if(NOT "${Boost_USE_MULTITHREADED}" STREQUAL "" AND NOT Boost_USE_MULTITHREADED)
  _BOOST_SKIPPED("libboost_wserialization-mt.a" "multithreaded, Boost_USE_MULTITHREADED=${Boost_USE_MULTITHREADED}")
  return()
endif()

# variant=release

if(NOT "${Boost_USE_RELEASE_LIBS}" STREQUAL "" AND NOT Boost_USE_RELEASE_LIBS)
  _BOOST_SKIPPED("libboost_wserialization-mt.a" "release, Boost_USE_RELEASE_LIBS=${Boost_USE_RELEASE_LIBS}")
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "  [x] libboost_wserialization-mt.a")
endif()

# Target file name: libboost_wserialization-mt.a

get_target_property(__boost_imploc Boost::wserialization IMPORTED_LOCATION_RELEASE)
if(__boost_imploc)
  message(WARNING "Target Boost::wserialization already has an imported location '${__boost_imploc}', which will be overwritten with '${_BOOST_LIBDIR}/libboost_wserialization-mt.a'")
endif()
unset(__boost_imploc)

set_property(TARGET Boost::wserialization APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(Boost::wserialization PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE CXX
  IMPORTED_LOCATION_RELEASE "${_BOOST_LIBDIR}/libboost_wserialization-mt.a"
  )

set_target_properties(Boost::wserialization PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
  )

list(APPEND _BOOST_WSERIALIZATION_DEPS serialization headers)
