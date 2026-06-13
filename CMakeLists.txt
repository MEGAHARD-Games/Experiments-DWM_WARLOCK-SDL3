cmake_minimum_required(VERSION 3.31.6)
project(TryingFoxHunting)

# Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# By configuring CMake with -DDOWNLOAD_DEPENDENCIES=ON/OFF,
# users can choose between downloading dependencies or using system libraries
option(DOWNLOAD_DEPENDENCIES "Download dependencies" TRUE)

if (DOWNLOAD_DEPENDENCIES)
	# FetchContent downloads and configures dependencies
	include(FetchContent)

	FetchContent_Declare(
			SDL3
			GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
			GIT_TAG "main"
			EXCLUDE_FROM_ALL
	)
	FetchContent_MakeAvailable(SDL3)

	FetchContent_Declare(
			glm
			GIT_REPOSITORY "https://github.com/g-truc/glm.git"
			GIT_TAG "master"
			EXCLUDE_FROM_ALL
	)
	FetchContent_MakeAvailable(glm)
else ()
	# find_package looks for already-installed system packages.
	# Configure with `-DCMAKE_PREFIX_PATH="/path/to/package1;/path/to/package2"`
	# to add search paths.
	find_package(SDL3 CONFIG REQUIRED)
	find_package(glm CONFIG REQUIRED)
endif ()

# Create your game executable target as usual
add_executable(${CMAKE_PROJECT_NAME}
		src/main.cpp
)

# Link to the libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3 glm::glm)

# Copy C++ DLLs to the build directory for Windows
if (WIN32)
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>
			$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
			COMMAND_EXPAND_LISTS
	)

	if (MINGW)
		get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)

		add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${MINGW_BIN_DIR}/libstdc++-6.dll"
				"${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
				"${MINGW_BIN_DIR}/libwinpthread-1.dll"
				$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
		)
	endif ()
endif ()

# This is safe to set on all platforms. Otherwise your SDL app will
#  have a terminal window pop up with it on Windows.
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
