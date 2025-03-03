cmake_minimum_required(VERSION 3.5)

project(engine)

############################################################
# Set compiler flags
############################################################
# If supported by your compiler, you can add the -Wall, -Wextra, –fstack-protector-all and -g3 flags here.
set(OWN_GXX_FLAGS "-std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OWN_GXX_FLAGS}")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OWN_GXX_FLAGS}")

############################################################
# List all sources
############################################################
set(engine_sources
	easy_image.cc
	ini_configuration.cc
		engine.cc
         Color.cpp Color.h Point2D.cpp Point2D.h Line2D.cpp Line2D.h l_parser.cc l_parser.h teken2D.cpp teken2D.h teken3D.cpp teken3D.h vector3d.cc vector3d.h Face.cpp Face.h Figure.cpp Figure.h PlatBody.cpp PlatBody.h ZBuffer.cpp ZBuffer.h Light.cpp Light.h InfLight.cpp InfLight.h PointLight.cpp PointLight.h)

############################################################
# Create an executable
############################################################
set(exe_name "engine")
add_executable( ${exe_name} ${engine_sources} )
install( TARGETS ${exe_name} DESTINATION ${PROJECT_SOURCE_DIR}/ )
