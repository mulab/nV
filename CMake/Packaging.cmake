# TODO: try include InstallRequiredSystemLibraries

set (CPACK_PACKAGE_NAME maTHmU)
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "maTHmU computer algebraic engine")
set (CPACK_PACKAGE_VENDOR "maTHmU Research")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
set (CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set (CPACK_PACKAGE_CONTACT "SONG Hao <baritono.tux@gmail.com>")
set (CPACK_STRIP_FILES TRUE)
set (CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PFREFIX})

set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.mathmu.cn/")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-date-time1.46.1, libboost-regex1.46.1, libboost-system1.46.1, libboost-thread1.46.1, libc6 (>= 2.11), libgcc1 (>= 1:4.1.1), libgmp10, libmpfi0 (>=1.5), libmpfr4, libstdc++6 (>= 4.6)")
set (CPACK_DEBIAN_PACKAGE_SECTION "math")

include (CPack)