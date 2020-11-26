file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(ExpansionGameEngineCore SHARED ${SRCS} ${HEADERS})

target_include_directories(ExpansionGameEngineCore PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../RaindropRenderer" "../PhysicaSound" "../libs/GL/include")
target_link_libraries(ExpansionGameEngineCore PUBLIC ExpansionMath BulldozerFileManager RaindropRenderer PhysicaSound)

find_package(Bullet REQUIRED)
if(BULLET_FOUND)
	target_include_directories(ExpansionGameEngineCore PUBLIC ${BULLET_INCLUDE_DIRS})
	target_link_libraries(ExpansionGameEngineCore PUBLIC ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY})
endif(BULLET_FOUND)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
target_link_libraries(ExpansionGameEngineCore PUBLIC ${JSONCPP_LIBRARIES})
target_include_directories(ExpansionGameEngineCore PUBLIC ${JSONCPP_INCLUDE_DIRS})

