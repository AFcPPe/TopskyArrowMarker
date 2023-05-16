cmake_minimum_required(VERSION 3.25)
project(TopskyArrowMarker)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/lib/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
find_library(EuroScopePlugIn EuroScopePlugInDll.lib ${CMAKE_SOURCE_DIR}/lib/)
add_library(TopskyArrowMarker SHARED library.cpp library.h Plugin.cpp Plugin.h CTmRadar.cpp CTmRadar.h)
target_link_libraries(TopskyArrowMarker ${EuroScopePlugIn})