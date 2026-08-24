install(
    TARGETS ip-log-filtering
    EXPORT ip-log-filteringTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT ip-log-filteringTargets
    NAMESPACE ip-log-filtering::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ip_log_filtering
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ip_log_filteringConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ip_log_filteringConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ip_log_filteringConfig.cmake"
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/ip_log_filtering
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/ip_log_filteringConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/ip_log_filteringConfigVersion.cmake"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/ip_log_filtering
)
