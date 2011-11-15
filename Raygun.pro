######################################################################
# Automatically generated by qmake (2.01a) Sun Nov 13 21:01:22 2011
######################################################################

#CONFIG = qt
TEMPLATE = app
TARGET = 
DEPENDPATH += . Raygun
INCLUDEPATH += . Raygun
LIBS += -ljpeg

# Input
HEADERS += Raygun/AABB.h \
           Raygun/Camera.h \
           Raygun/Colour.h \
           Raygun/ImageBuffer.h \
           Raygun/ImageIO.h \
           Raygun/Light.h \
           Raygun/Material.h \
           Raygun/Matrix.h \
           Raygun/Mesh.h \
           Raygun/Metaballs.h \
           Raygun/Perlin.h \
           Raygun/PointLight.h \
           Raygun/Ray.h \
           Raygun/RayTracer.h \
           Raygun/Scene.h \
           Raygun/Shader.h \
           Raygun/Shape.h \
           Raygun/Sphere.h \
           Raygun/test_scenes.h \
           Raygun/Vector.h \
    Raygun/GUI/mainwindow.h \
    Raygun/GUI/rtthread.h
SOURCES += Raygun/ImageBuffer.cpp \
           Raygun/ImageIO.cpp \
           Raygun/main.cpp \
           Raygun/Mesh.cpp \
           Raygun/Metaballs.cpp \
           Raygun/Perlin.cpp \
           Raygun/PointLight.cpp \
           Raygun/RayTracer.cpp \
           Raygun/Scene.cpp \
           Raygun/Sphere.cpp \
    Raygun/GUI/mainwindow.cpp \
    Raygun/GUI/rtthread.cpp