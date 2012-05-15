file(GLOB_RECURSE mu_scripts RELATIVE ${PROJECT_SOURCE_DIR}/mUScript "*.m")

foreach (script ${mu_scripts})
    configure_file(${script} ${MUSCRIPT_OUTPUT_PATH}/${script})
endforeach ()

install(DIRECTORY Integrate DESTINATION ${MUSCRIPT_RELATIVE_DIR}
         FILES_MATCHING PATTERN "*.m")
install(DIRECTORY System DESTINATION ${MUSCRIPT_RELATIVE_DIR}
         FILES_MATCHING PATTERN "*.m")