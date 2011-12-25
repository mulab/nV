new_shared_library(mulink)

add_common_to_target(mulink)
target_link_libraries(mulink nvkernel mukernel nvparser)

install(FILES
    EmbedmU.n
    EmbednV.m
    DESTINATION mULink
)
