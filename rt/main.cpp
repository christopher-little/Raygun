#include <iostream>
#include <string>

using namespace std;

/*
#include <QApplication>
#include <QPoint>

#include "GUI/mainwindow.h"
*/

#include "rt/RayTracer.h"
#include "rt/Scene.h"
#include "rt/SceneParser.h"
#include "rt/test_scenes.h"
#include "rt/ImageBuffer.h"
#include "rt/ImageIO.h"


int main(int argc, char **argv)
{
    // Check command-line flags
    bool testModeFlag = false;	// Render and display image immediately for testing purposes
    for(int i=0; i<argc; i++)
    {
        if(string(argv[i]) == string("-testmode"))
            testModeFlag = true;
    }

    /*
    // Create the application and main window
    QApplication app(argc, argv);

    // Start up the main window
    MainWindow *mainWindow = new MainWindow(testModeFlag);
    mainWindow->move(QPoint(100,150));
    mainWindow->show();

    return app.exec();
    */

    /*
    cout << "Starting" << endl;
    Scene *sceneYAML = SceneParser::parse_yaml("scenes/test1.yml");
    RayTracer *rt = new RayTracer(sceneYAML);
    ImageBuffer *image = new ImageBuffer(1280,720);
    rt->render(image);
    cout << "Writing image" << endl;
    writeJPG("image.jpg", image);
    delete rt;


    Scene *scene = testscene::transformScene();
    rt = new RayTracer(scene);
    rt->render(image);
    writeJPG("image2.jpg", image);

    cout << "And finally..." << endl;
    rt->rtPixelSampleCount = 8;
    rt->render(image);
    writeJPG("image3.jpg", image);
    */


    cout << "Starting" << endl;
    Scene *scene = testscene::DOFtest();
    RayTracer *rt = new RayTracer(scene);
    ImageBuffer *image = new ImageBuffer(960,540);

    rt->rtPixelSampleCount = 1;
    rt->render(image);

    cout << "Writing image" << endl;
    writeJPG("image.jpg", image);
    delete rt;

    return 0;
}
