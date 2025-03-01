file(GLOB_RECURSE CHAPTER1_FILES "*.hpp" "*.cpp")
add_executable(chapter1 ${CHAPTER1_FILES})
target_link_libraries(chapter1 Arrow::arrow_static)

install(TARGETS chapter1
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)