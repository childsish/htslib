cmake_minimum_required(VERSION 3.0)
project(htslib_js)

include(CheckLibraryExists)
include(CheckIncludeFiles)
include(CheckFunctionExists)


# Set environment variables

set(CMAKE_CXX_STANDARD 11)
if (UNIX)
    set(PLUGIN_EXT .so)
else()
    set(PLUGIN_EXT .dll)
endif()

# Create config.h

check_function_exists(fdatasync HAVE_FDATASYNC)
check_function_exists(getpagesize HAVE_GETPAGESIZE)
check_function_exists(gmtime_r HAVE_GMTIME_R)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_function_exists(mmap HAVE_MMAP)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/param.h HAVE_SYS_PARAM_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
set(HAVE_LIBZ on)

if(ENABLE_LIBCURL)
    check_function_exists(CCHmac HAVE_COMMONCRYPTO)
    check_library_exists(crypto HMAC "" HAVE_HMAC)
    if(HAVE_COMMONCRYPTO OR HAVE_HMAC)
        check_library_exists(curl curl_easy_init "" HAVE_LIBCURL)
    else()
        message(FATAL_ERROR "You need SSL development package to compile htslib with ENABLE_LIBCURL.")
    endif()

    if(NOT HAVE_LIBCURL)
        message(FATAL_ERROR "You need curl development package to compile htslib with ENABLE_LIBCURL.")
    endif()
else()
    set(HAVE_COMMONCRYPTO no)
    set(HAVE_HMAC no)
    set(HAVE_LIBCURL no)
endif()

configure_file(config.in.h ${PROJECT_BINARY_DIR}/config.h)


# Create version.h

execute_process(
    COMMAND git describe --always --dirty
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/htslib
    OUTPUT_VARIABLE PACKAGE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(WRITE ${PROJECT_BINARY_DIR}/version.h "#define HTS_VERSION \"${PACKAGE_VERSION}\"")


# Include directories

# hts

if(!ENABLE_PLUGINS)
    list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/htslib/plugin.c")
endif()

set(HTSLIB_SOURCES
    bcf_sr_sort.c
    bgzf.c htslib/bgzf.h
    errmod.c
    faidx.c htslib/faidx.h
    hfile.c htslib/hfile.h
    hfile_gcs.c
    hfile_net.c
    hts.c htslib/hts.h
    hts_os.c
    kfunc.c htslib/kfunc.h
    knetfile.c htslib/knetfile.h
    kstring.c htslib/kstring.h
    md5.c
    multipart.c
    plugin.c
    probaln.c
    realn.c
    regidx.c
    sam.c htslib/sam.h
    synced_bcf_reader.c htslib/synced_bcf_reader.h
    tbx.c htslib/tbx.h
    textutils.c
    thread_pool.c htslib/thread_pool.h
    vcf.c htslib/vcf.h
    vcf_sweep.c htslib/vcf_sweep.h
    vcfutils.c htslib/vcfutils.h
    cram/cram_codecs.c cram/cram_codecs.h
    cram/cram_decode.c cram/cram_decode.h
    cram/cram_encode.c cram/cram_encode.h
    cram/cram_external.c
    cram/cram.h
    cram/cram_index.c cram/cram_index.h
    cram/cram_io.c cram/cram_io.h
    cram/cram_samtools.c cram/cram_samtools.h
    cram/cram_stats.c cram/cram_stats.h
    cram/cram_structs.h
    cram/files.c
    cram/mFILE.c cram/mFILE.h
    cram/misc.h
    cram/open_trace_file.c cram/open_trace_file.h
    cram/os.h
    cram/pooled_alloc.c cram/pooled_alloc.h
    cram/rANS_byte.h
    cram/rANS_static.c cram/rANS_static.h
    cram/sam_header.c cram/sam_header.h
    cram/string_alloc.c cram/string_alloc.h)
if(ENABLE_LIBCURL)
    list(APPEND HTSLIB_SOURCES hfile_libcurl.c)
endif()

add_library(hts ${HTSLIB_SOURCES})
# try setting EMSCRIPTEN_INCLUDE_DIR to emscripten/system/include
target_include_directories(hts PRIVATE
    ${EMSCRIPTEN_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR})
target_link_libraries(hts z pthread)
if(ENABLE_LIBCURL)
    target_link_libraries(curl crypto)
    list(APPEND HTSLIB_SOURCES
        hfile_libcurl.c
        hfile_s3.c)
endif()
if (EMSCRIPTEN)
    target_compile_options(hts PRIVATE "-s" "USE_ZLIB=1")
    target_link_libraries(hts "-s USE_ZLIB=1")
endif()

# Executables

add_executable(bgzip bgzip.c)
target_include_directories(bgzip PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(bgzip hts)

add_executable(htsfile htsfile.c)
target_include_directories(htsfile PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(htsfile hts)

add_executable(tabix tabix.c)
target_include_directories(tabix PRIVATE ${PROJECT_BINARY_DIR})
target_link_libraries(tabix hts)

if (EMSCRIPTEN)
    target_compile_options(bgzip PRIVATE "-s" "USE_ZLIB=1")
    target_compile_options(htsfile PRIVATE "-s" "USE_ZLIB=1")
    target_compile_options(tabix PRIVATE "-s" "USE_ZLIB=1")
endif ()
