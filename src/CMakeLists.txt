set(SOURCES
    b64.c
)

set(HEADERS
    b64.h
)

set(MAINSRC
    main.cpp
)

add_library(b64 SHARED STATIC ${SOURCES} ${HEADERS})
add_executable(b64e ${SOURCES} ${HEADERS} ${MAINSRC})

target_link_libraries(b64e b64)
