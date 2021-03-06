file(GLOB RD_SOURCES *.cpp)
list(REMOVE_ITEM RD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(RD_SOURCES ${RD_SOURCES} PARENT_SCOPE)

add_executable(rd
    main.cpp
    ${RD_SOURCES}
)

set_target_properties(rd PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
)

# We duplicate the libararies here to solve a dumb link time error

if(STATIC_GSL)
set(LINK_LIBS ${PLLMODULES_LIBRARIES} ${PLL_LIBRARIES} ${PLLMODULES_LIBRARIES}
    ${GSL_LIBS} lbfgs_lib -static)
else()
set(LINK_LIBS ${PLLMODULES_LIBRARIES} ${PLL_LIBRARIES} ${PLLMODULES_LIBRARIES}
    ${GSL_LIBS} lbfgs_lib)
endif()

target_link_libraries(rd ${LINK_LIBS})
target_compile_options(rd PRIVATE -Wall -Wextra -Wshadow
    -Wdouble-promotion -Wmissing-include-dirs -Wtrampolines
    -pedantic -Wsign-conversion -Wnarrowing)
target_compile_definitions(rd PRIVATE
    GIT_REV=${RD_BUILD_VERSION}
    GIT_COMMIT=${RD_BUILD_COMMIT}
    BUILD_DATE=${RD_BUILD_DATE})

