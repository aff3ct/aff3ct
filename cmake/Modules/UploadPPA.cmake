find_program(DEBUILD_EXECUTABLE debuild)
find_program(DPUT_EXECUTABLE dput)

if(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)
  message(WARNING "Debuild or dput not installed, please run sudo apt-get install devscripts")
  return()
endif(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)

if(NOT AFF3CT_DPUT_HOST)
  message(WARNING "AFF3CT_DPUT_HOST varibale note set. Please give the ppa host name.")
  return()
endif(NOT AFF3CT_DPUT_HOST)

execute_process(COMMAND git tag -n20 --points-at v${AFF3CT_VERSION}
                OUTPUT_VARIABLE CHANGELOG
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CHANGELOG_MESSAGE ${CHANGELOG})

if(NOT AFF3CT_PPA_DISTRIB)
execute_process(COMMAND lsb_release -cs
                OUTPUT_VARIABLE DISTRI
                OUTPUT_STRIP_TRAILING_WHITESPACE)
set(AFF3CT_PPA_DISTRIB ${DISTRI})
endif()

foreach(DISTRI ${AFF3CT_PPA_DISTRIB})
    string(REPLACE "\n" "\n " DEBIAN_LONG_DESCRIPTION " ${CPACK_PACKAGE_DESCRIPTION}")
    set(DEBIAN_PACKAGE_FILE_NAME "aff3ct-${AFF3CT_VERSION_FULL}-${DISTRI}.orig")
    set(DEBIAN_PACKAGE_VERSION "${AFF3CT_VERSION_FULL}-${DISTRI}")
    set(DEBIAN_SOURCE_DIR ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${CPACK_DEBIAN_PACKAGE_NAME}-${DEBIAN_PACKAGE_VERSION})
    set(DEBIAN_CONTROL ${DEBIAN_SOURCE_DIR}/debian/control)
    set(DEBIAN_COPYRIGHT ${DEBIAN_SOURCE_DIR}/debian/copyright)
    set(DEBIAN_RULES ${DEBIAN_SOURCE_DIR}/debian/rules)
    set(DEBIAN_CHANGELOG ${DEBIAN_SOURCE_DIR}/debian/changelog)
    set(DEBIAN_SOURCE_CHANGES ${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}-${DISTRI}_source.changes)
    set(ORIG_FILE "${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${DEBIAN_PACKAGE_FILE_NAME}.tar.gz")
    if(CHANGELOG_MESSAGE)
        set(output_changelog_msg ${CHANGELOG_MESSAGE})
    else()
        set(output_changelog_msg "* Package created with CMake")
    endif(CHANGELOG_MESSAGE)
    if(NOT CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG)
        set(CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG ${CMAKE_SOURCE_DIR}/debian/changelog)
    endif()

    # ##############################################################################
    # # debian/control
    list(APPEND CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS "cmake" "debhelper (>= 7.0.50)")
    list(REMOVE_DUPLICATES CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS)
    list(SORT CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS)
    string(REPLACE ";" ", " build_depends "${CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS}")
    string(REPLACE ";" ", " bin_depends "${CPACK_DEBIAN_PACKAGE_DEPENDS}")
    file(WRITE ${DEBIAN_CONTROL}
         "Source: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
         "Section: ${CPACK_DEBIAN_PACKAGE_SECTION}\n"
         "Priority: ${CPACK_DEBIAN_PACKAGE_PRIORITY}\n"
         "Maintainer: ${CPACK_DEBIAN_PACKAGE_MAINTAINER}\n"
         "Build-Depends: ${build_depends}\n"
         "Standards-Version: 3.9.7\n"
         "Homepage: ${CPACK_DEBIAN_PACKAGE_HOMEPAGE}\n"
         "\n"
         "Package: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
         "Architecture: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}\n"
         "Depends: ${bin_depends}, \${shlibs:Depends}, \${misc:Depends}\n"
         "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
         "${DEBIAN_LONG_DESCRIPTION}\n"
         "\n"
         "Package: ${CPACK_DEBIAN_PACKAGE_NAME}-dev\n"
         "Architecture: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}\n"
         "Depends: ${bin_depends}, \${shlibs:Depends}, \${misc:Depends}\n"
         "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
         "${DEBIAN_LONG_DESCRIPTION}\n"
         "\n"
         "Package: ${CPACK_DEBIAN_PACKAGE_NAME}-doc\n"
         "Architecture: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}\n"
         "Depends: ${bin_depends}, \${shlibs:Depends}, \${misc:Depends}\n"
         "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
         "${DEBIAN_LONG_DESCRIPTION}\n")

    file(WRITE "${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}.install"
         "usr/bin/*\n"
         "usr/share/aff3ct-${AFF3CT_VERSION_FULL}/conf/*\n"
         "usr/share/aff3ct-${AFF3CT_VERSION_FULL}/refs/*\n")

    file(WRITE "${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}-dev.install"
         "usr/lib/x86_64-linux-gnu/*\n"
         "usr/include/aff3ct-${AFF3CT_VERSION_FULL}/*\n")

    file(WRITE "${DEBIAN_SOURCE_DIR}/debian/${CPACK_DEBIAN_PACKAGE_NAME}-doc.install"
         "doc/sphinx/built/html/* usr/share/aff3ct-${AFF3CT_VERSION_FULL}/doc/html\n"
         "doc/sphinx/built/latex/AFF3CT.pdf usr/share/aff3ct-${AFF3CT_VERSION_FULL}/doc/pdf/\n")

    ##############################################################################
    # debian/copyright
    configure_file(${CPACK_RESOURCE_FILE_LICENSE} ${DEBIAN_COPYRIGHT} COPYONLY)

    ##############################################################################
    # debian/rules
    file(WRITE ${DEBIAN_RULES}
         "#!/usr/bin/make -f\n"
         "\nexport DH_VERBOSE=1"
         "\n\n%:\n"
         "\tdh  $@ --buildsystem=cmake\n"
         "\noverride_dh_auto_configure:\n"
         "\tDESTDIR=\"$(CURDIR)\" dh_auto_configure"
         " -- -DCMAKE_BUILD_TYPE=Release -DAFF3CT_COMPILE_SHARED_LIB=ON -DAFF3CT_EXT_STRINGS=OFF"
         "-DAFF3CT_OVERRIDE_VERSION=${GIT_VERSION} -DCMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'"
         "\n\noverride_dh_auto_install:\n"
         "\tdh_auto_install --destdir=\"$(CURDIR)\" --buildsystem=cmake"
         "\n\noverride_dh_strip:\n")

    execute_process(COMMAND chmod +x ${DEBIAN_RULES})

    ##############################################################################
    # debian/compat
    file(WRITE ${DEBIAN_SOURCE_DIR}/debian/compat "7")

    ##############################################################################
    # debian/source/format
    file(WRITE ${DEBIAN_SOURCE_DIR}/debian/source/format "3.0 (native)")

    ##############################################################################

    if(EXISTS ${CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG})
        configure_file(${CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG} ${DEBIAN_CHANGELOG} COPYONLY)
      
        if(CPACK_DEBIAN_UPDATE_CHANGELOG)
            file(READ ${DEBIAN_CHANGELOG} DEBIAN_CHANGELOG_content)
            execute_process(COMMAND date -R
                            OUTPUT_VARIABLE DATE_TIME
                            OUTPUT_STRIP_TRAILING_WHITESPACE)
            file(WRITE ${DEBIAN_CHANGELOG}
                 "${CPACK_DEBIAN_PACKAGE_NAME} (${AFF3CT_VERSION_FULL}-${DISTRI} ${DISTRI}; urgency=low\n\n"
                 "  ${output_changelog_msg}\n\n"
                 " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${DATE_TIME}\n\n")
            file(APPEND ${DEBIAN_CHANGELOG} ${DEBIAN_CHANGELOG_content})
    endif()

    else()
      execute_process(COMMAND date -R
                      OUTPUT_VARIABLE DATE_TIME
                      OUTPUT_STRIP_TRAILING_WHITESPACE)
      file(WRITE ${DEBIAN_CHANGELOG}
           "${CPACK_DEBIAN_PACKAGE_NAME} (${AFF3CT_VERSION_FULL}-${DISTRI}) ${DISTRI}; urgency=low\n\n"
           "  ${output_changelog_msg}\n\n"
           " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${DATE_TIME}\n")
    endif()

    ##########################################################################
    # .orig.tar.gz
    add_custom_command(OUTPUT ${ORIG_FILE}
                       COMMAND cpack -G TGZ --config ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake
                       -D CPACK_PACKAGE_FILE_NAME=${DEBIAN_PACKAGE_FILE_NAME}
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI})


    add_custom_command(OUTPUT ${DEBIAN_SOURCE_DIR}/CMakeLists.txt
                       COMMAND tar zxf ${ORIG_FILE}
                       WORKING_DIRECTORY ${DEBIAN_SOURCE_DIR}
                       DEPENDS ${ORIG_FILE})

    ##############################################################################
    # debuild -S (build log in )
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${DEBIAN_SOURCE_CHANGES}
                       COMMAND ${DEBUILD_EXECUTABLE} --no-tgz-check -S > /dev/null
                       DEPENDS ${DEBIAN_SOURCE_DIR}/CMakeLists.txt
                       WORKING_DIRECTORY ${DEBIAN_SOURCE_DIR})

    add_custom_target(debuild_${DISTRI} ALL
                      DEPENDS ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${DEBIAN_SOURCE_CHANGES}
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI})

    # ##############################################################################
    # dput ppa:your-lp-id/ppa <source.changes>
    # TODO : Add possibility to use .dc file
    add_custom_target(dput_${DISTRI} ALL
                      COMMAND ${DPUT_EXECUTABLE} ${AFF3CT_DPUT_HOST} ${DEBIAN_SOURCE_CHANGES}
                      DEPENDS  debuild_${DISTRI}
                      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI})


endforeach(DISTRI)
