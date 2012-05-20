TEMPLATE = app
TARGET =

DEPENDPATH += . gui rt rt/lights rt/shapes
INCLUDEPATH += . gui rt rt/lights rt/shapes

# Ensure build will trigger recompile/relink when only a header file is changed (this is a bug or at least a severe annoyance in QT Creator as far as I'm concerned...)
CONFIG += depend_includepath

LIBS += -ljpeg -lyaml-cpp

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
	   rt/shapes/Sphere.h \
    rt/lights/all_lights.h \
    rt/shapes/all_shapes.h \
    rt/SceneParser.h
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
	   rt/shapes/Sphere.cpp \
    rt/SceneParser.cpp








