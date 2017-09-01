set(src
    mmdata.h
    mmheader.c mmheader.h
    process_data.c process_data.h
    process_header.c process_header.h
)
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
add_library(ParallelMatrixMarket STATIC ${src})
