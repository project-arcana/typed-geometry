# Check if a CPU Extension Instruction Set is present on your current machine
# example usage:
# is_cpu_extension_supported(BMI2_SUPPORTED "bmi2")
function(is_cpu_extension_supported EXTENSION_SUPPORTED EXTENSION_NAME)
    try_compile(COMPILE_SUCCESS
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/cmake/checks/cpu_${EXTENSION_NAME}.cc"
        COMPILE_DEFINITIONS "-march=native"
        OUTPUT_VARIABLE TRY_OUT
    )
    set(${EXTENSION_SUPPORTED} ${COMPILE_SUCCESS} PARENT_SCOPE)
endfunction()
