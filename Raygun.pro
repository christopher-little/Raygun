TEMPLATE = app
TARGET = 
DEPENDPATH += . gui rt rt\lights rt\shapes
INCLUDEPATH += . gui rt rt\lights rt\shapes
win32 {
    INCLUDEPATH += C:\MinGW-nuwen\include
    LIBS += C:\MinGW-nuwen\lib\libjpeg.a
}
unix {
	LIBS += -ljpeg
}

# Input
HEADERS += gui/mainwindow.h \
           gui/rtthread.h \
           rt/AABB.h \
           rt/Camera.h \
           rt/Colour.h \
           rt/ImageBuffer.h \
           rt/ImageIO.h \
           rt/Material.h \
           rt/Matrix.h \
           rt/Perlin.h \
           rt/Ray.h \
           rt/RayTracer.h \
           rt/Scene.h \
           rt/Shader.h \
           rt/test_scenes.h \
           rt/Vector.h \
           rt/lights/Light.h \
           rt/lights/PointLight.h \
           rt/shapes/Mesh.h \
           rt/shapes/Metaballs.h \
           rt/shapes/Shape.h \
           rt/shapes/Sphere.h
SOURCES += gui/mainwindow.cpp \
           gui/rtthread.cpp \
           rt/ImageBuffer.cpp \
           rt/ImageIO.cpp \
           rt/main.cpp \
           rt/Perlin.cpp \
           rt/RayTracer.cpp \
           rt/Scene.cpp \
           rt/lights/PointLight.cpp \
           rt/shapes/Mesh.cpp \
           rt/shapes/Metaballs.cpp \
           rt/shapes/Sphere.cpp








