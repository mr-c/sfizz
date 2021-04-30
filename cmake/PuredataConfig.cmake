find_path(PUREDATA_INCLUDE_DIR "m_pd.h" PATH_SUFFIXES "pd")

if(PUREDATA_INCLUDE_DIR)
    message(STATUS "Puredata headers: ${PUREDATA_INCLUDE_DIR}")
else()
    message(STATUS "Puredata headers not found, using our own")
    set(PUREDATA_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/plugins/puredata/external/pd/include")
endif()

if(WIN32)
    set(PUREDATA_SUFFIX ".dll")
elseif(APPLE)
    set(PUREDATA_SUFFIX ".pd_darwin")
else()
    set(PUREDATA_SUFFIX ".pd_linux")
endif()
