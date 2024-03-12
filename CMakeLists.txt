cmake_minimum_required( VERSION 3.1 )

project( GLFW-CMake-starter )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

add_subdirectory( glfw )
add_subdirectory( assimp )

set_target_properties(assimp PROPERTIES FOLDER "assimp")
set_target_properties(unit PROPERTIES FOLDER "assimp")
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "assimp")
set_target_properties(uninstall PROPERTIES FOLDER "assimp")
set_target_properties(zlibstatic PROPERTIES FOLDER "assimp")

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

function(add_example ARG)

	set( EXAMPLE_SOURCES "" )
	set( GLFW-CMAKE-STARTER-SRC "" )
	get_filename_component( PROJECT_NAME ${ARG} NAME )
	
	file(GLOB_RECURSE EXAMPLE_SOURCES ${ARG}/*.cpp ${ARG}/*.hpp ${ARG}/*.cc ${ARG}/*.h)

	set( GLFW-CMAKE-STARTER-SRC ${EXAMPLE_SOURCES} )

	add_executable( ${PROJECT_NAME} WIN32 ${GLFW-CMAKE-STARTER-SRC} )
	target_link_libraries( ${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw )
	target_include_directories ( ${PROJECT_NAME} PRIVATE "glad/include" "assimp/include" )
	
endfunction()

set ( EXAMPLES 
	"example/triangle"
	"example/triangle2"
	"example/mesh"
)

foreach(EXAMPLE ${EXAMPLES})
add_example( ${EXAMPLE} )
endforeach()


if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "triangle" )
    endif()
endif()
