find_program(DEBUILD_EXECUTABLE debuild)
find_program(DPUT_EXECUTABLE dput)

if(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)
  message(WARNING "Debuild or dput not installed, please run sudo apt-get install devscripts")
  return()
endif(NOT DEBUILD_EXECUTABLE OR NOT DPUT_EXECUTABLE)

if(NOT AFF3CT_PPA_DISTRIB)
execute_process(
    COMMAND lsb_release -cs
    OUTPUT_VARIABLE DISTRI
    OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(AFF3CT_PPA_DISTRIB ${DISTRI})
endif()

foreach(DISTRI ${AFF3CT_PPA_DISTRIB})

set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}~${DISTRI}1")
set(DEBIAN_SOURCE_DIR ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${CPACK_DEBIAN_PACKAGE_NAME}_${DEBIAN_PACKAGE_VERSION})

# ##############################################################################
# # debian/control

set(debian_control ${DEBIAN_SOURCE_DIR}/debian/control)
list(APPEND CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS "cmake" "debhelper (>= 7.0.50)")
list(REMOVE_DUPLICATES CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS)
list(SORT CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS)
string(REPLACE ";" ", " build_depends "${CPACK_DEBIAN_PACKAGE_BUILD_DEPENDS}")
string(REPLACE ";" ", " bin_depends "${CPACK_DEBIAN_PACKAGE_DEPENDS}")
file(WRITE ${debian_control}
  "Source: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
  "Section: ${CPACK_DEBIAN_PACKAGE_SECTION}\n"
  "Priority: ${CPACK_DEBIAN_PACKAGE_PRIORITY}\n"
  "Maintainer: ${CPACK_DEBIAN_PACKAGE_MAINTAINER}\n"
  "Build-Depends: ${build_depends}\n"
  "Standards-Version: 3.9.7\n"
  "Homepage: ${CPACK_DEBIAN_PACKAGE_HOMEPAGE}\n"
  "\n"
  "Package: ${CPACK_DEBIAN_PACKAGE_NAME}\n"
  # "Version: ${CPACK_DEBIAN_PACKAGE_VERSION}\n"
  "Architecture: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}\n"
  "Depends: ${bin_depends}, \${shlibs:Depends}, \${misc:Depends}\n"
  "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
  "${deb_long_description}\n"
  )

# file(APPEND ${debian_control}
#   "\n\n"
#   "Package: ${CPACK_DEBIAN_PACKAGE_NAME}-dbg\n"
#   "Priority: extra\n"
#   "Section: debug\n"
#   "Architecture: any\n"
#   "Depends: ${CPACK_DEBIAN_PACKAGE_NAME} (= \${binary:Version}), \${shlibs:Depends}, \${misc:Depends}\n"
#   "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n"
#   "${deb_long_description}"
#   "\n .\n"
#   " This is the debugging symbols for the ${CPACK_DEBIAN_PACKAGE_NAME} library\n"
#  )

foreach(COMPONENT ${CPACK_COMPONENTS_ALL})
  string(TOUPPER ${COMPONENT} UPPER_COMPONENT)
  set(DEPENDS "${CPACK_DEBIAN_PACKAGE_NAME}")
  foreach(DEP ${CPACK_COMPONENT_${UPPER_COMPONENT}_DEPENDS})
    set(DEPENDS "${DEPENDS}, ${CPACK_DEBIAN_PACKAGE_NAME}-${DEP}")
  endforeach(DEP ${CPACK_COMPONENT_${UPPER_COMPONENT}_DEPENDS})
  set(DEPENDS "${DEPENDS}, \${misc:Depends}")
  file(APPEND ${debian_control} "\n"
    "Package: ${CPACK_DEBIAN_PACKAGE_NAME}-${COMPONENT}\n"
    "Architecture: ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}\n"
    "Depends: ${DEPENDS}\n"
    "Description: ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}"
    ": ${CPACK_COMPONENT_${UPPER_COMPONENT}_DISPLAY_NAME}\n"
    "${deb_long_description}"
    "\n .\n"
    " ${CPACK_COMPONENT_${UPPER_COMPONENT}_DESCRIPTION}\n"
	)
endforeach(COMPONENT ${CPACK_COMPONENTS_ALL})



##############################################################################
# debian/copyright
set(debian_copyright ${DEBIAN_SOURCE_DIR}/debian/copyright)
configure_file(${CPACK_RESOURCE_FILE_LICENSE} ${debian_copyright} COPYONLY)

##############################################################################
# debian/rules
set(debian_rules ${DEBIAN_SOURCE_DIR}/debian/rules)

file(WRITE ${debian_rules}
	"#!/usr/bin/make -f\n"
	"\nexport DH_VERBOSE=1"
	"\n\n%:\n"
	"\tdh  $@ --buildsystem=cmake\n"
	"\noverride_dh_auto_configure:\n"
	"\tDESTDIR=\"$(CURDIR)/debian/${CPACK_DEBIAN_PACKAGE_NAME}\" dh_auto_configure"
  " -- -DCMAKE_BUILD_TYPE=Release -DAFF3CT_COMPILE_SHARED_LIB=ON -DPACKAGE_TGZ=OFF "
  "-DAFF3CT_OVERRIDE_VERSION=${GIT_VERSION} -DCMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'"
	"\n\noverride_dh_auto_install:\n"
	"\tdh_auto_install --destdir=\"$(CURDIR)/debian/${CPACK_DEBIAN_PACKAGE_NAME}\" --buildsystem=cmake"
	"\n\noverride_dh_strip:\n"
	# "\tdh_strip --dbg-package=${CPACK_DEBIAN_PACKAGE_NAME}-dbg"
)

execute_process(COMMAND chmod +x ${debian_rules})

##############################################################################
# debian/compat
file(WRITE ${DEBIAN_SOURCE_DIR}/debian/compat "7")

##############################################################################
# debian/source/format
file(WRITE ${DEBIAN_SOURCE_DIR}/debian/source/format "3.0 (native)")

##############################################################################

# debian/changelog
set(debian_changelog ${DEBIAN_SOURCE_DIR}/debian/changelog)
if(NOT CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG)
  set(CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG ${CMAKE_SOURCE_DIR}/debian/changelog)
endif()

# TODO add support for git dch (git-buildpackage)
if(CHANGELOG_MESSAGE)
	set(output_changelog_msg ${CHANGELOG_MESSAGE})
else()
	set(output_changelog_msg "* Package created with CMake")
endif(CHANGELOG_MESSAGE)

if(EXISTS ${CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG})
  configure_file(${CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG} ${debian_changelog} COPYONLY)

  if(CPACK_DEBIAN_UPDATE_CHANGELOG)
    file(READ ${debian_changelog} debian_changelog_content)
    execute_process(
      COMMAND date -R
      OUTPUT_VARIABLE DATE_TIME
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(WRITE ${debian_changelog}
      "${CPACK_DEBIAN_PACKAGE_NAME} (${DEBIAN_PACKAGE_VERSION}) ${DISTRI}; urgency=low\n\n"
      "  ${output_changelog_msg}\n\n"
      " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${DATE_TIME}\n\n"
      )
    file(APPEND ${debian_changelog} ${debian_changelog_content})
  endif()

else()
  execute_process(
    COMMAND date -R
    OUTPUT_VARIABLE DATE_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  file(WRITE ${debian_changelog}
    "${CPACK_DEBIAN_PACKAGE_NAME} (${DEBIAN_PACKAGE_VERSION}) ${DISTRI}; urgency=low\n\n"
    "  ${output_changelog_msg}\n\n"
    " -- ${CPACK_DEBIAN_PACKAGE_MAINTAINER}  ${DATE_TIME}\n"
    )
   #configure_file(${debian_changelog} ${CPACK_DEBIAN_RESOURCE_FILE_CHANGELOG}  COPYONLY)
endif()


##########################################################################
# Templates

if (CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA)
	foreach(CF ${CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA})
	get_filename_component(CF_NAME ${CF} NAME)
	message("Writing debian/${CF_NAME}")
	configure_file(${CF} ${DEBIAN_SOURCE_DIR}/debian/${CF_NAME} @ONLY)
	endforeach()
endif(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA)


##########################################################################
# .orig.tar.gz
#execute_process(COMMAND date +%y%m%d
#  OUTPUT_VARIABLE day_suffix
#  OUTPUT_STRIP_TRAILING_WHITESPACE
#  )

#set(package_file_name "${CPACK_DEBIAN_PACKAGE_NAMEde}_${DEBIAN_PACKAGE_VERSION}")
set(package_file_name "${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}")

set(orig_file "${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${package_file_name}.orig.tar.gz")

add_custom_command(OUTPUT ${orig_file}
  COMMAND cpack -G TGZ --config ${CMAKE_BINARY_DIR}/CPackSourceConfig.cmake
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI}
  )


add_custom_command(OUTPUT ${DEBIAN_SOURCE_DIR}/CMakeLists.txt
  COMMAND tar zxf ${orig_file}
  WORKING_DIRECTORY ${DEBIAN_SOURCE_DIR}
  DEPENDS ${orig_file}
  )

##############################################################################
# debuild -S
set(DEB_SOURCE_CHANGES
  ${CPACK_DEBIAN_PACKAGE_NAME}_${DEBIAN_PACKAGE_VERSION}_source.changes
  )

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${DEB_SOURCE_CHANGES}
  COMMAND ${DEBUILD_EXECUTABLE} --no-tgz-check -S
  DEPENDS ${DEBIAN_SOURCE_DIR}/CMakeLists.txt
  WORKING_DIRECTORY ${DEBIAN_SOURCE_DIR}
  )

add_custom_target(debuild_${DISTRI} ALL
				DEPENDS ${DEBIAN_SOURCE_DIR}/CMakeLists.txt
        aff3ct-bin
				${CMAKE_BINARY_DIR}/Debian/${DISTRI}/${DEB_SOURCE_CHANGES}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI}
		)
# ##############################################################################
# # dput ppa:your-lp-id/ppa <source.changes>

set(DPUT_HOST "ppa:aff3ct/aff3ct")

add_custom_target(dput_${DISTRI} ALL
        COMMAND ${DPUT_EXECUTABLE} ${DPUT_HOST} ${DEB_SOURCE_CHANGES}
        DEPENDS  debuild_${DISTRI}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI}
    )
endforeach(DISTRI)



# if(UPLOAD_PPA)
#     if (EXISTS ${DPUT_CONFIG_IN})
#         set(DPUT_DIST ${DISTRI})
#         configure_file(
#             ${DPUT_CONFIG_IN}
#             ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/dput.cf
#             @ONLY
#         )
#         add_custom_target(dput_${DISTRI} ALL
#             COMMAND ${DPUT_EXECUTABLE} -c ${CMAKE_BINARY_DIR}/Debian/${DISTRI}/dput.cf ${DPUT_HOST} ${DEB_SOURCE_CHANGES}
#             DEPENDS debuild_${DISTRI}, aff3ct-bin
#             WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI}
#         )
#     else()
        # add_custom_target(dput_${DISTRI} ALL
        #     COMMAND ${DPUT_EXECUTABLE} ${DPUT_HOST} ${DEB_SOURCE_CHANGES}
        #     DEPENDS debuild_${DISTRI}, aff3ct-bin
        #     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Debian/${DISTRI}
        # )
#     endif()
# endif()