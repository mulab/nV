AUX_SOURCE_DIRECTORY(. PARSER_SRCS)
add_library(nvparser SHARED ${PARSER_SRCS})
target_link_libraries(nvparser nvkernel nvpolynomial)
