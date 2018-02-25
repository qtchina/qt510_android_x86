
if (CMAKE_VERSION VERSION_LESS 2.8.3)
    message(FATAL_ERROR "Qt 5 requires at least CMake version 2.8.3")
endif()

get_filename_component(_qt53DExtras_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

# For backwards compatibility only. Use Qt53DExtras_VERSION instead.
set(Qt53DExtras_VERSION_STRING 5.10.0)

set(Qt53DExtras_LIBRARIES Qt5::3DExtras)

macro(_qt5_3DExtras_check_file_exists file)
    if(NOT EXISTS "${file}" )
        message(FATAL_ERROR "The imported target \"Qt5::3DExtras\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
endmacro()

macro(_populate_3DExtras_target_properties Configuration LIB_LOCATION IMPLIB_LOCATION)
    set_property(TARGET Qt5::3DExtras APPEND PROPERTY IMPORTED_CONFIGURATIONS ${Configuration})

    set(imported_location "${_qt53DExtras_install_prefix}/lib/${LIB_LOCATION}")
    _qt5_3DExtras_check_file_exists(${imported_location})
    set_target_properties(Qt5::3DExtras PROPERTIES
        "INTERFACE_LINK_LIBRARIES" "${_Qt53DExtras_LIB_DEPENDENCIES}"
        "IMPORTED_LOCATION_${Configuration}" ${imported_location}
        "IMPORTED_SONAME_${Configuration}" "libQt53DExtras.so"
        # For backward compatibility with CMake < 2.8.12
        "IMPORTED_LINK_INTERFACE_LIBRARIES_${Configuration}" "${_Qt53DExtras_LIB_DEPENDENCIES}"
    )

endmacro()

if (NOT TARGET Qt5::3DExtras)

    set(_Qt53DExtras_OWN_INCLUDE_DIRS "${_qt53DExtras_install_prefix}/include/" "${_qt53DExtras_install_prefix}/include/Qt3DExtras")
    set(Qt53DExtras_PRIVATE_INCLUDE_DIRS
        "${_qt53DExtras_install_prefix}/include/Qt3DExtras/5.10.0"
        "${_qt53DExtras_install_prefix}/include/Qt3DExtras/5.10.0/Qt3DExtras"
    )

    foreach(_dir ${_Qt53DExtras_OWN_INCLUDE_DIRS})
        _qt5_3DExtras_check_file_exists(${_dir})
    endforeach()

    # Only check existence of private includes if the Private component is
    # specified.
    list(FIND Qt53DExtras_FIND_COMPONENTS Private _check_private)
    if (NOT _check_private STREQUAL -1)
        foreach(_dir ${Qt53DExtras_PRIVATE_INCLUDE_DIRS})
            _qt5_3DExtras_check_file_exists(${_dir})
        endforeach()
    endif()

    set(Qt53DExtras_INCLUDE_DIRS ${_Qt53DExtras_OWN_INCLUDE_DIRS})

    set(Qt53DExtras_DEFINITIONS -DQT_3DEXTRAS_LIB)
    set(Qt53DExtras_COMPILE_DEFINITIONS QT_3DEXTRAS_LIB)
    set(_Qt53DExtras_MODULE_DEPENDENCIES "3DRender;3DInput;3DLogic;3DCore;Gui;Core")


    set(Qt53DExtras_OWN_PRIVATE_INCLUDE_DIRS ${Qt53DExtras_PRIVATE_INCLUDE_DIRS})

    set(_Qt53DExtras_FIND_DEPENDENCIES_REQUIRED)
    if (Qt53DExtras_FIND_REQUIRED)
        set(_Qt53DExtras_FIND_DEPENDENCIES_REQUIRED REQUIRED)
    endif()
    set(_Qt53DExtras_FIND_DEPENDENCIES_QUIET)
    if (Qt53DExtras_FIND_QUIETLY)
        set(_Qt53DExtras_DEPENDENCIES_FIND_QUIET QUIET)
    endif()
    set(_Qt53DExtras_FIND_VERSION_EXACT)
    if (Qt53DExtras_FIND_VERSION_EXACT)
        set(_Qt53DExtras_FIND_VERSION_EXACT EXACT)
    endif()

    set(Qt53DExtras_EXECUTABLE_COMPILE_FLAGS "")

    foreach(_module_dep ${_Qt53DExtras_MODULE_DEPENDENCIES})
        if (NOT Qt5${_module_dep}_FOUND)
            find_package(Qt5${_module_dep}
                5.10.0 ${_Qt53DExtras_FIND_VERSION_EXACT}
                ${_Qt53DExtras_DEPENDENCIES_FIND_QUIET}
                ${_Qt53DExtras_FIND_DEPENDENCIES_REQUIRED}
                PATHS "${CMAKE_CURRENT_LIST_DIR}/.." NO_DEFAULT_PATH
            )
        endif()

        if (NOT Qt5${_module_dep}_FOUND)
            set(Qt53DExtras_FOUND False)
            return()
        endif()

        list(APPEND Qt53DExtras_INCLUDE_DIRS "${Qt5${_module_dep}_INCLUDE_DIRS}")
        list(APPEND Qt53DExtras_PRIVATE_INCLUDE_DIRS "${Qt5${_module_dep}_PRIVATE_INCLUDE_DIRS}")
        list(APPEND Qt53DExtras_DEFINITIONS ${Qt5${_module_dep}_DEFINITIONS})
        list(APPEND Qt53DExtras_COMPILE_DEFINITIONS ${Qt5${_module_dep}_COMPILE_DEFINITIONS})
        list(APPEND Qt53DExtras_EXECUTABLE_COMPILE_FLAGS ${Qt5${_module_dep}_EXECUTABLE_COMPILE_FLAGS})
    endforeach()
    list(REMOVE_DUPLICATES Qt53DExtras_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES Qt53DExtras_PRIVATE_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES Qt53DExtras_DEFINITIONS)
    list(REMOVE_DUPLICATES Qt53DExtras_COMPILE_DEFINITIONS)
    list(REMOVE_DUPLICATES Qt53DExtras_EXECUTABLE_COMPILE_FLAGS)

    set(_Qt53DExtras_LIB_DEPENDENCIES "Qt5::3DRender;Qt5::3DInput;Qt5::3DLogic;Qt5::3DCore;Qt5::Gui;Qt5::Core")


    add_library(Qt5::3DExtras SHARED IMPORTED)

    set_property(TARGET Qt5::3DExtras PROPERTY
      INTERFACE_INCLUDE_DIRECTORIES ${_Qt53DExtras_OWN_INCLUDE_DIRS})
    set_property(TARGET Qt5::3DExtras PROPERTY
      INTERFACE_COMPILE_DEFINITIONS QT_3DEXTRAS_LIB)

    set(_Qt53DExtras_PRIVATE_DIRS_EXIST TRUE)
    foreach (_Qt53DExtras_PRIVATE_DIR ${Qt53DExtras_OWN_PRIVATE_INCLUDE_DIRS})
        if (NOT EXISTS ${_Qt53DExtras_PRIVATE_DIR})
            set(_Qt53DExtras_PRIVATE_DIRS_EXIST FALSE)
        endif()
    endforeach()

    if (_Qt53DExtras_PRIVATE_DIRS_EXIST
        AND NOT CMAKE_VERSION VERSION_LESS 3.0.0 )
        add_library(Qt5::3DExtrasPrivate INTERFACE IMPORTED)
        set_property(TARGET Qt5::3DExtrasPrivate PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES ${Qt53DExtras_OWN_PRIVATE_INCLUDE_DIRS}
        )
        set(_Qt53DExtras_PRIVATEDEPS)
        foreach(dep ${_Qt53DExtras_LIB_DEPENDENCIES})
            if (TARGET ${dep}Private)
                list(APPEND _Qt53DExtras_PRIVATEDEPS ${dep}Private)
            endif()
        endforeach()
        set_property(TARGET Qt5::3DExtrasPrivate PROPERTY
            INTERFACE_LINK_LIBRARIES Qt5::3DExtras ${_Qt53DExtras_PRIVATEDEPS}
        )
    endif()

    _populate_3DExtras_target_properties(RELEASE "libQt53DExtras.so" "" )




    file(GLOB pluginTargets "${CMAKE_CURRENT_LIST_DIR}/Qt53DExtras_*Plugin.cmake")

    macro(_populate_3DExtras_plugin_properties Plugin Configuration PLUGIN_LOCATION)
        set_property(TARGET Qt5::${Plugin} APPEND PROPERTY IMPORTED_CONFIGURATIONS ${Configuration})

        set(imported_location "${_qt53DExtras_install_prefix}/plugins/${PLUGIN_LOCATION}")
        _qt5_3DExtras_check_file_exists(${imported_location})
        set_target_properties(Qt5::${Plugin} PROPERTIES
            "IMPORTED_LOCATION_${Configuration}" ${imported_location}
        )
    endmacro()

    if (pluginTargets)
        foreach(pluginTarget ${pluginTargets})
            include(${pluginTarget})
        endforeach()
    endif()




_qt5_3DExtras_check_file_exists("${CMAKE_CURRENT_LIST_DIR}/Qt53DExtrasConfigVersion.cmake")

endif()
