set(src
    pmm_constants.c
    pmm_data.h
    pmm_header.h
    pmm_read_routines.h
    pmm_process_data.c
    pmm_process_header.c
    pmm_write_routines.c pmm_write_routines.h
)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
add_library(ParallelMatrixMarket STATIC ${src})
