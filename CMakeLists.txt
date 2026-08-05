cmake_minimum_required(VERSION 3.8)

project(MediaLoader)

set (CMAKE_CXX_STANDARD 11)


if(MSVC)
	set(CMAKE_SUPPRESS_REGENERATION TRUE)
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MP")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MP")
	if(MEGA_X86)
		# This prevents usage of SSE2.
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:SSE")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /arch:SSE")
	endif()
	message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
	message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
endif()

include_directories(include image-libs/)

file(GLOB image_src ${CMAKE_CURRENT_SOURCE_DIR}/image/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/image-libs/**/*.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/image-libs/**/*.cpp )

add_library(medialoader_image ${image_src})
target_link_libraries(medialoader_image jpeg png tiff webp)

# The following code will build the sound libraries.

add_subdirectory(sound-libs/libmodplug)
add_subdirectory(sound-libs/libogg)

set(OGG_LIBRARY ogg-static)
add_subdirectory(sound-libs/libvorbis)
add_subdirectory(sound-libs/mpg123/ports/cmake)

include_directories(sound-libs/ 	
	${CMAKE_CURRENT_BINARY_DIR}/sound-libs/mpg123/ports/cmake/src/
	sound-libs/mpg123/src/
	sound-libs/mpg123/src/libmpg123/)

file(GLOB sound_src ${CMAKE_CURRENT_SOURCE_DIR}/sound/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/sound-libs/**/*.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/sound-libs/Wuff/*.c)

add_library(medialoader_sound ${sound_src})
target_link_libraries(medialoader_sound modplug-static ogg-static vorbis-static vorbisfile-static libmpg123)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ COMPONENT Development)

install(TARGETS medialoader_image medialoader_sound DESTINATION ${CMAKE_INSTALL_LIBDIR}/ COMPONENT Development)

# --- model (assimp) ---
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
# Keep medialoader's shared-libs policy; force static assimp for this subdir.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(model-libs/assimp)

set(model_src
	${CMAKE_CURRENT_SOURCE_DIR}/model/StdFileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model/ModelScene.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model/AssimpIO.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model/ModelLoader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/image/Exception.cpp
)
add_library(medialoader_model ${model_src})
target_include_directories(medialoader_model
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(medialoader_model PUBLIC assimp)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/medialoader/model
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/medialoader/ COMPONENT Development)
install(TARGETS medialoader_model DESTINATION ${CMAKE_INSTALL_LIBDIR}/ COMPONENT Development)

add_executable(medialoader_model_test model/tests/model_loader_test.cpp)
target_link_libraries(medialoader_model_test PRIVATE medialoader_model)