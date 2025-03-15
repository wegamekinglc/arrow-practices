file(GLOB_RECURSE CHAPTER4_FILES "*.cpp")
add_library(export_cdata SHARED ${CHAPTER4_FILES})
target_link_libraries(export_cdata Arrow::arrow_shared)

install(TARGETS export_cdata
        RUNTIME DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)