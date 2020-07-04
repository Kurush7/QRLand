//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"

using namespace std;

Facade::Facade(sptr<QRImage> img): image(img) {
    manager = sptr<BaseCommandManager> (new CommandManager());
    auto cr = PolySceneCreator();
    scene = cr.create();
    scene->getActiveCamera()->setBind(Vector3D(0,0,0)); // todo
    renderer = sptr<QRenderer>(new QRenderer(image, scene));
}

void Facade::addCube(float a, float x, float y, float z, QRColor c) {
    auto txt = sptr<QRTexture>(new ColorTexture(c));
    auto cr = sptr<QRPolyModelCreator>(new CubeModelCreator(a, txt));
    auto command = sptr<QRCommand>(new AddModelCmd(cr, Vector3D(x,y,z), scene));
    manager->push(command);
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

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}