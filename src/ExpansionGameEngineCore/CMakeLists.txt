file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(ExpansionGameEngineCore SHARED ${SRCS} ${HEADERS})

target_include_directories(ExpansionGameEngineCore PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../RaindropRenderer" "../PhysicaSound" "../libs/GL/include" "../libs/PHYSX/linux/PhysX/include" "../libs/PHYSX/linux/PxShared/include")
target_link_libraries(ExpansionGameEngineCore PUBLIC ExpansionMath BulldozerFileManager RaindropRenderer PhysicaSound)

set(PHYSX "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysX_static_64.a")
set(PHYSX_COMMON "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXCommon_static_64.a")
set(PHYSX_COOKING "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXCooking_static_64.a")
set(PHYSX_FOUNDATION "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXFoundation_static_64.a")
set(PHYSX_PVD "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXPvdSDK_static_64.a")
set(PHYSX_EXTENSIONS "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXExtensions_static_64.a")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
target_link_libraries(ExpansionGameEngineCore PUBLIC ${JSONCPP_LIBRARIES})
target_include_directories(ExpansionGameEngineCore PUBLIC ${JSONCPP_INCLUDE_DIRS})

target_link_libraries(ExpansionGameEngineCore PUBLIC ${PHYSX_EXTENSIONS} ${PHYSX} ${PHYSX_PVD} ${PHYSX_COMMON} ${PHYSX_COOKING} ${PHYSX_FOUNDATION})

