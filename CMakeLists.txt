# CMake stuff for DisC by ~ironlake

# CMake set and package finding stuff
cmake_minimum_required(VERSION 3.6 )
project(DisC )
set(DISC_MAJ 0 )
set(DISC_MIN 1 )
set(DISC_VER ${DISC_MAJ}.${DISC_MIN} )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/find/" )
add_definitions( -DDISC_VER="${DISC_VER}" )
find_package(OpenSSL REQUIRED )
find_package(Jansson REQUIRED )
include_directories(AFTER ${JANSSON_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} )
include_directories(BEFORE include/DisC )

# library
ADD_LIBRARY( DisC STATIC src/DisC_errors.c src/DisC.c src/DisC_rest.c src/DisC_util.c src/DisC_gateway.c src/DisC_object.c )


install(TARGETS DisC DESTINATION lib)
install(DIRECTORY include/DisC DESTINATION include)
# test executable(s) (tbd)
