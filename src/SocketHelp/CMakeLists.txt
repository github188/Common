SET(WINPR_PATH    ${CMAKE_SOURCE_DIR}/3rdparty/winpr)
SET(WINPR_INCLUDE ${WINPR_PATH}/include)

INCLUDE_DIRECTORIES(
    ../../inc/linux
    ${WINPR_INCLUDE}
    )

LIST(APPEND REF_FILES
    SocketHelp.cpp stdafx.cpp
	)

ADD_DEFINITIONS(-g -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-result -fPIC -DPIC)


ADD_LIBRARY(SocketHelp STATIC ${REF_FILES})
