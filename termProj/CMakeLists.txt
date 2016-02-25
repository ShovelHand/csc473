# In CMake, # are comments.

# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.0)

# Now set the project name. You can call this whatever you
# like.
project(assignment)

# If you're using an IDE, then you might want to use folders to
# organize everything. So tell CMake to use them.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Now we're going to define some variables. This is to prevent us
# from having to type full paths later on.
# I usually name them like PROJECTNAME_VAR_NAME
set(ASSIGNMENT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(ASSIGNMENT_SOURCE_ROOT "${ASSIGNMENT_SOURCE_DIR}/termProj/source")
set(ASSIGNMENT_INCLUDE_ROOT "${ASSIGNMENT_SOURCE_DIR}/termProj/include")
set(ASSIGNMENT_SHADER_ROOT "${ASSIGNMENT_SOURCE_DIR}/termProj/shaders")

# You'll see why we need this in a second.
set(ASSIGNMENT_GENERATED_ROOT "${ASSIGNMENT_SOURCE_DIR}/generated")

# Set this one to wherever you saved atlas. In this case, I
# saved it to lib/atlas
set(ASSIGNMENT_ATLAS_ROOT "${ASSIGNMENT_SOURCE_DIR}/lib/atlas")

# This is where Atlas stores its configuration files for compilers and
# whatnot (since v1.1). We will use this to setup the compiler for us.
set(ASSIGNMENT_CONFIG_ROOT "${ASSIGNMENT_ATLAS_ROOT}/config")

# This next block sets up the compiler. Instead of worrying about
# different configurations, we can just use the ones that Atlas
# already provides.
include("${ASSIGNMENT_CONFIG_ROOT}/Compiler.cmake")

# Now, we add the atlas subdirectory.
add_subdirectory(${ASSIGNMENT_ATLAS_ROOT})

# Now let's add our source folders.
add_subdirectory(${ASSIGNMENT_INCLUDE_ROOT})
add_subdirectory(${ASSIGNMENT_SOURCE_ROOT})
add_subdirectory(${ASSIGNMENT_SHADER_ROOT})

# Tell the compiler where to find include files.
include_directories(
    ${ATLAS_INCLUDE_DIRS}
    ${ASSIGNMENT_GENERATED_ROOT}
    ${ASSIGNMENT_INCLUDE_ROOT})

# This next step is optional and is purely for aesthetic
# purposes within the IDE. Essentially, it tells the IDE
# under what folders to place each file.
source_group("include" FILES ${INCLUDE_LIST})
source_group("source" FILES ${SOURCE_LIST})
source_group("shaders" FILES ${SHADER_LIST})

# Now let's make our executable and link the libraries.
add_executable(assignment ${SOURCE_LIST} ${INCLUDE_LIST} ${SHADER_LIST})
target_link_libraries(assignment ${ATLAS_LIBRARIES})