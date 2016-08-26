# 2016, Georg Sauthoff <mail@georg.so>

cmake_minimum_required(VERSION 3.0)
project(somake C CXX)

include(CheckFunctionExists)
check_function_exists(wcsdup HAVE_WCSDUP)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(PkgConfig)
  pkg_search_module(BSD libbsd)
  if (BSD_LIBRARIES)
    set(HAVE_CLOSEFROM 1)
  endif()
else()
  check_function_exists(closefrom HAVE_CLOSEFROM)
endif()

configure_file(config.h.in config.h)

if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  find_library(NSL_LIB NAMES nsl)
  # prefer the OpenCSW libintl over the one from /usr/lib
  find_library(INTL_LIB NAMES intl HINTS /opt/csw/lib)
endif()

add_library(mksh STATIC
  lib/mksh/dosys.cc lib/mksh/globals.cc lib/mksh/i18n.cc lib/mksh/macro.cc
  lib/mksh/misc.cc  lib/mksh/mksh.cc    lib/mksh/read.cc
)

add_library(vroot STATIC
  lib/vroot/access.cc    lib/vroot/chown.cc    lib/vroot/lock.cc
  lib/vroot/open.cc      lib/vroot/setenv.cc   lib/vroot/utimes.cc
  lib/vroot/args.cc      lib/vroot/chroot.cc   lib/vroot/lstat.cc 
  lib/vroot/readlink.cc  lib/vroot/stat.cc     lib/vroot/vroot.cc 
  lib/vroot/chdir.cc     lib/vroot/creat.cc    lib/vroot/mkdir.cc
  lib/vroot/report.cc    lib/vroot/truncate.cc lib/vroot/chmod.cc
  lib/vroot/execve.cc    lib/vroot/mount.cc    lib/vroot/rmdir.cc
  lib/vroot/unlink.cc
)

add_executable(${PROJECT_NAME}
  bin/ar.cc       bin/depvar.cc  bin/doname.cc       bin/dosys.cc
  bin/files.cc    bin/globals.cc bin/implicit.cc     bin/macro.cc
  bin/main.cc     bin/misc.cc    bin/nse_printdep.cc bin/parallel.cc
  bin/pmake.cc    bin/read.cc    bin/read2.cc        bin/rep.cc
  bin/state.cc
  lib/bsd/bsd.cc
  comp/progname.c comp/wcsdup.c
)

set_property(TARGET ${PROJECT_NAME} mksh vroot PROPERTY INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${BSD_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
  mksh vroot pthread ${BSD_LIBRARIES} ${NSL_LIB} ${INTL_LIB}
)

# default install prefix is /usr/local
# change it with e.g. -DCMAKE_INSTALL_PREFIX=/usr
install(TARGETS   ${PROJECT_NAME}  RUNTIME DESTINATION bin)
install(FILES     man/man1/make.1          DESTINATION share/man/man1)
install(FILES     bin/make.rules.file      DESTINATION share/${PROJECT_NAME}
        RENAME        make.rules)
install(FILES     bin/svr4.make.rules.file DESTINATION share/${PROJECT_NAME}
        RENAME        svr4.make.rules)
install(FILES     COPYING
                  README.md
                                           DESTINATION share/doc/${PROJECT_NAME}
)
install(DIRECTORY example                  DESTINATION share/doc/${PROJECT_NAME}
)

# select the package generator with e.g. -DCPACK_GENERATOR='RPM;DEB'
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "illumos dmake, Sun make compatible")
set(CPACK_PACKAGE_CONTACT "mail@georg.so")
set(CPACK_PACKAGE_VENDOR "misc")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README_pkg.txt")
set(CPACK_RPM_PACKAGE_LICENSE "CDDL")
# used by some generators like NSI, is only displayed in the installer
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
# only used by some generators
set(CPACK_PACKAGE_INSTALL_DIRECTORY
    "${PROJECT_NAME}-${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_PACKAGE_URL "https://github.com/gsauthof/somake")
# CPackDeb default to CPACK_PACKAGE_DESCRIPTION_SUMMARY, although
# the long description makes more sense ...
file(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)
include(CPack)
