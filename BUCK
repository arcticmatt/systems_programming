prebuilt_cxx_library(
  name = "boost",
  header_dirs = [
    "boost/1.72.0/include/",
  ],
  # True for most Boost libraries.
  # Should get a linker error if we use a Boost library that must be built separately.
  header_only = True,
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name = "double_conversion",
  header_dirs = [
    "double-conversion/3.1.5/include/",
  ],
  static_lib = "double-conversion/3.1.5/lib/libdouble-conversion.a",
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name="folly",
  header_dirs = [
    "folly/2020.02.10.00/include/",
  ],
  static_lib = "folly/2020.02.10.00/lib/libfolly.a",
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name = "gflags",
  header_dirs = [
    "gflags/2.2.2/include/",
  ],
  static_lib = "gflags/2.2.2/lib/libgflags.2.2.2.dylib",
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name = "glog",
  header_dirs = [
    "glog/0.4.0/include/",
  ],
  static_lib = "glog/0.4.0/lib/libglog.0.4.0.dylib",
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name = "libevent_core",
  header_dirs = [
    "libevent/2.1.11_1/include/",
  ],
  static_lib = "libevent/2.1.11_1/lib/libevent_core.a",
  visibility = [
    'PUBLIC',
  ],
)

prebuilt_cxx_library(
  name = "openssl",
  header_dirs = [
    "openssl/1.0.2s/include/",
  ],
  static_lib = "openssl/1.0.2s/lib/libssl.a",
  visibility = [
    'PUBLIC',
  ],
)
