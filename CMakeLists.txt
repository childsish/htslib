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
check_library_exists(deflate libdeflate_deflate_compress "" HAVE_LIBDEFLATE)
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

if(ENABLE_GCS)
    if (NOT ENABLE_LIBCURL)
        message(WARNING "GCS support not enabled

Support for Google Cloud Storage URLs requires libcurl support to be enabled
in HTSlib.  Configure with --enable-libcurl in order to use GCS URLs.")
    endif()
endif()

configure_file(config.in.h ${PROJECT_BINARY_DIR}/config.h)


# Create version.h

execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/htslib
    OUTPUT_VARIABLE PACKAGE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
file(WRITE ${PROJECT_BINARY_DIR}/version.h "#define HTS_VERSION_TEXT \"${PACKAGE_VERSION}-emscripten_compatible\"")


# Include directories

# hts

set(HTSLIB_PUBLIC_HEADERS
    htslib/bgzf.h
    htslib/cram.h
    htslib/faidx.h
    htslib/hfile.h
    htslib/hts.h
    htslib/hts_defs.h
    htslib/hts_endian.h
    htslib/hts_log.h
    htslib/hts_os.h
    htslib/kbitset.h
    htslib/kfunc.h
    htslib/khash.h
    htslib/khash_str2int.h
    htslib/klist.h
    htslib/knetfile.h
    htslib/kseq.h
    htslib/ksort.h
    htslib/kstring.h
    htslib/regidx.h
    htslib/sam.h
    htslib/synced_bcf_reader.h
    htslib/tbx.h
    htslib/thread_pool.h
    htslib/vcf.h
    htslib/vcf_sweep.h
    htslib/vcfutils.h)

set(HTSLIB_SOURCES
    ${HTSLIB_PUBLIC_HEADERS}
    bcf_sr_sort.c
    bcf_sr_sort.h
    bgzf.c
    config.h
    errmod.c
    faidx.c
    header.c
    header.h
    hfile_internal.h
    hfile.c
    hfile_gcs.c
    hfile_libcurl.c
    hfile_net.c
    hts.c
    hts_internal.h
    hts_os.c
    kfunc.c
    knetfile.c
    kstring.c
    md5.c
    multipart.c
    plugin.c
    probaln.c
    realn.c
    regidx.c
    region.c
    sam.c
    sam_internal.h
    synced_bcf_reader.c
    tbx.c
    textutils.c
    textutils_internal.h
    thread_pool.c
    thread_pool_internal.h
    vcf.c
    vcf_sweep.c
    vcfutils.c
    cram/cram.h
    cram/cram_codecs.c
    cram/cram_codecs.h
    cram/cram_decode.c
    cram/cram_decode.h
    cram/cram_encode.c
    cram/cram_encode.h
    cram/cram_external.c
    cram/cram_index.c
    cram/cram_index.h
    cram/cram_io.c
    cram/cram_io.h
    cram/cram_samtools.c
    cram/cram_samtools.h
    cram/cram_stats.c
    cram/cram_stats.h
    cram/cram_structs.h
    cram/mFILE.c
    cram/mFILE.h
    cram/misc.h
    cram/open_trace_file.c
    cram/open_trace_file.h
    cram/os.h
    cram/pooled_alloc.c
    cram/pooled_alloc.h
    cram/rANS_byte.h
    cram/rANS_static.c
    cram/rANS_static.h
    cram/string_alloc.c
    cram/string_alloc.h
    os/lzma_stub.h
    os/rand.c)

if (${HAVE_HMAC})
    list(APPEND HTSLIB_SOURCES
        hfile_s3.c
        hfile_s3_write.c)
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
