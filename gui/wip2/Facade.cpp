//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"
using namespace std;

// TODO MORE DETAILS ON THE EDGES..... WTF?!!!!!!

Facade::Facade(sptr<QRImage> img): image(img) {
    manager = sptr<BaseCommandManager> (new CommandManager());
    auto cr = PolySceneCreatorNoCamera();
    scene = cr.create();

    auto cam = sptr<QRCamera3D>(new Camera3D(3, 3, -5,2, 0.01,
                                             QRINF, Vector3D(0,0,-5), Vector3D(M_PI/2,0,0)));
    scene->addCamera(cam, "observeCamera");

    cam = sptr<QRCamera3D>(new Camera3D(3, 3, -5, 2, 0.01, QRINF,
            Vector3D(0,0,0), Vector3D(M_PI/2,0,0), true));
    scene->addCamera(cam, "walkCamera");

    scene->setActiveCamera("observeCamera");

    renderer = sptr<QRenderer>(new FullThreadRenderer(image, scene));


    //scene->addModel(sptr<QRPolyModel3D>(new QRLandscapeSurface(2,2, 10)), Vector3D(0,0,0));

    //scene->addModel(RandomHMapLandscapeSurfaceCreator(50, 50, 0.2).create(),
    //       Vector3D(0,0,0));

    scene->addModel(RoamLandscapeCreator(129, 129, 0.1).create(),
           Vector3D(0,0,0));

    //scene->addModel(CubeModelCreator(10,
    //        sptr<QRTexture>(new ColorTexture(127,127,127))).create(),Vector3D(0,0,0));
}

void Facade::draw() {
    auto command = sptr<QRCommand>(new RenderCmd(renderer));
    manager->push(command);
    manager->execAll();
}

void Facade::moveCamera(float dx, float dy, float dz) {
    //cout << "move by: " << Vector3D(dx,dy,dz) << '\n';
    auto command = sptr<QRCommand>(new MoveCameraCmd(Vector3D(dx,dy,dz), scene));
    manager->push(command);
}
void Facade::scaleCamera(float kx, float ky, float kz) {
    //cout << "scale by: " << Vector3D(kx,ky,kz) << '\n';
    auto command = sptr<QRCommand>(new ScaleCameraCmd(Vector3D(kx,ky,kz), scene));
    manager->push(command);
}
void Facade::rotateCamera(float dx, float dy, float dz) {
    //cout << "rotate by: " << Vector3D(dx,dy,dz) << '\n';
    auto command = sptr<QRCommand>(new RotateCameraCmd(Vector3D(dx,dy,dz), scene));
    manager->push(command);
}

void Facade::changeCamera() {
    if (scene->getActiveCameraName() == "walkCamera")
        scene->setActiveCamera("observeCamera");
    else scene->setActiveCamera("walkCamera");

    renderer->render();
}

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}