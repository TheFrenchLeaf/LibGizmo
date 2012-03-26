cmake_minimum_required(VERSION 2.6)
project(helloGl CXX C)

#########################
# Qt
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
SET(QT_USE_QTOPENGL ON)
INCLUDE(${QT_USE_FILE})
#########################

INCLUDE_DIRECTORIES(CedricGuillemet-LibGizmo-4b097bd/inc)
ADD_SUBDIRECTORY(CedricGuillemet-LibGizmo-4b097bd)

SET(helloGl_SOURCES main.cpp glwidget.cpp window.cpp qtlogo.cpp)
QT_WRAP_CPP(HELLOGL helloGl_SOURCES glwidget.h window.h qtlogo.h)
ADD_EXECUTABLE(helloGl ${helloGl_SOURCES})
TARGET_LINK_LIBRARIES(helloGl
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTOPENGL_LIBRARY}
                      ${OPENGL_gl_LIBRARY}
                      ${OPENGL_glu_LIBRARY}
					  libGizmo)