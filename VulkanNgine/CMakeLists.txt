#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME VKViewer)

set(VKNGINE_SRC
        main.cpp)

set(VKNGINE_HEADER)

add_executable(${VKNGINE_APP_NAME} ${VKNGINE_SRC} ${VKNGINE_HEADER})