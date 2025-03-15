file(GLOB_RECURSE CHAPTER5_FILES "*.cpp")
add_executable(compute_functions ${CHAPTER5_FILES})
target_link_libraries(compute_functions Arrow::arrow_shared Parquet::parquet_shared)

install(TARGETS compute_functions
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)