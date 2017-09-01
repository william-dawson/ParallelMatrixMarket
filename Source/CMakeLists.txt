set(src
    mmheader.h
    process_header.c process_header.h
)

add_library(ParallelMatrixMarket STATIC ${src})
