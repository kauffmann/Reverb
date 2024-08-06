 cmake_minimum_required(VERSION 3.22)


 project(Graphic-Assets)


set(Graphic_Lib "${CMAKE_CURRENT_SOURCE_DIR}/Graphic-Assets")




# Adding binary data from Graphic Assets directory . CONFIGURE_DEPEND , This will tell CMake to check the glob and rebuild if necessary.Glob == dir*
 file(GLOB_RECURSE ASSETS_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.png" "${CMAKE_CURRENT_SOURCE_DIR}/*.ttf")

# # Print the list of files to verify paths
# message(STATUS "Assets files: ${ASSETS_FILES}")

 


# # Checking if ASSETS_FILES is empty
 if(ASSETS_FILES)
     juce_add_binary_data(AudioPluginData SOURCES ${ASSETS_FILES})
     
 else()
     message(FATAL_ERROR "No asset files found in Graphic Assets directory." + ${CMAKE_CURRENT_SOURCE_DIR})
 endif()

