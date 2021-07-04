file(GLOB SRC "*.cpp")
file(GLOB HEADERS "*.h")

add_library(Game SHARED ${SRC} ${HEADERS})

set(PHYSX "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysX_static_64.a")
set(PHYSX_COMMON "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXCommon_static_64.a")
set(PHYSX_COOKING "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXCooking_static_64.a")
set(PHYSX_FOUNDATION "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXFoundation_static_64.a")
set(PHYSX_PVD "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXPvdSDK_static_64.a")
set(PHYSX_EXTENSIONS "${CMAKE_SOURCE_DIR}/src/libs/PHYSX/linux/PhysX/bin/release/libPhysXExtensions_static_64.a")

target_include_directories(Game PUBLIC "../ExpansionMath" "../BulldozerFileManager" "../RaindropRenderer" "../ExpansionGameEngineCore" "../PhysicaSound" "../libs/GL/include")
target_link_libraries(Game PUBLIC ExpansionMath BulldozerFileManager RaindropRenderer PhysicaSound ExpansionGameEngineCore)
target_link_libraries(Game PUBLIC ${PHYSX_EXTENSIONS} ${PHYSX} ${PHYSX_PVD} ${PHYSX_COMMON} ${PHYSX_COOKING} ${PHYSX_FOUNDATION})
