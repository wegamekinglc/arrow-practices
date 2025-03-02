file(GLOB_RECURSE EXAMPLE_FILES "*.hpp" "*.cpp")
add_executable(example_101 ${EXAMPLE_FILES})
target_link_libraries(example_101 Arrow::arrow_shared)

install(TARGETS example_101
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)