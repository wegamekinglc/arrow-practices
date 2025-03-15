file(GLOB_RECURSE CHAPTER5_V2_FILES "*.cpp")
add_executable(compute_or_not ${CHAPTER5_V2_FILES})
target_link_libraries(compute_or_not Arrow::arrow_shared)

install(TARGETS compute_or_not
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)