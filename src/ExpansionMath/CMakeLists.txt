
file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_library(ExpansionMath SHARED ${SRCS} ${HEADERS})