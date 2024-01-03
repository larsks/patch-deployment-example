find_package(${pkgName} CONFIG REQUIRED COMPONENTS ${pkgName})

add_executable(patch-deployment main.c)
target_link_libraries(patch-deployment PRIVATE ${pkgName}::${pkgName})
