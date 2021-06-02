set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
add_custom_target( 
    doc ALL
    COMMAND doxygen ${DOXYFILE}
)