file(GLOB_RECURSE CHAPTER5_V3_FILES "*.cpp")
add_executable(simple_acero ${CHAPTER5_V3_FILES})
target_link_libraries(simple_acero Arrow::arrow_shared)

install(TARGETS simple_acero
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)