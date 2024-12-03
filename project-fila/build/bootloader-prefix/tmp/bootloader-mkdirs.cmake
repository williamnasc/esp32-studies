# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/ESP/v5.3.1/esp-idf/components/bootloader/subproject"
  "C:/ESP/projects/project-fila/build/bootloader"
  "C:/ESP/projects/project-fila/build/bootloader-prefix"
  "C:/ESP/projects/project-fila/build/bootloader-prefix/tmp"
  "C:/ESP/projects/project-fila/build/bootloader-prefix/src/bootloader-stamp"
  "C:/ESP/projects/project-fila/build/bootloader-prefix/src"
  "C:/ESP/projects/project-fila/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/ESP/projects/project-fila/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/ESP/projects/project-fila/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
