set(src
    mmdata.h
    mmheader.c mmheader.h
    process_data.c process_data.h
    process_header.c process_header.h
)

add_library(ParallelMatrixMarket STATIC ${src})
