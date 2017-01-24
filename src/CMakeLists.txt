set(SOURCES
    b64.c
)

set(HEADERS
    b64.h
)

add_library(b64 SHARED ${SOURCES} ${HEADERS})
