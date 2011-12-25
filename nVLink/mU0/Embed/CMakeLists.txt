new_shared_library(mu0embed)

target_link_libraries(mu0embed mu0link)

install(FILES mU0.u mU0Test.u DESTINATION Link/mU0/Embed)
