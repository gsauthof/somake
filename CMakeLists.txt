# 2016, Georg Sauthoff <mail@georg.so>

cmake_minimum_required(VERSION 2.8)
project(somake C CXX)

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

add_executable(somake
  bin/ar.cc       bin/depvar.cc  bin/doname.cc       bin/dosys.cc
  bin/files.cc    bin/globals.cc bin/implicit.cc     bin/macro.cc
  bin/main.cc     bin/misc.cc    bin/nse_printdep.cc bin/parallel.cc
  bin/pmake.cc    bin/read.cc    bin/read2.cc        bin/rep.cc
  bin/state.cc
)

set_property(TARGET somake mksh vroot PROPERTY INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(somake
  mksh vroot pthread
)

