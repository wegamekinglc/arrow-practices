file(GLOB_RECURSE FLIGHT_CLIENT_FILES "*.hpp" "*.cpp")
add_executable(flight_client ${FLIGHT_CLIENT_FILES})
target_link_libraries(flight_client Arrow::arrow_static)

install(TARGETS flight_client
        RUNTIME DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)