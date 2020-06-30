//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"

using namespace std;

Facade::Facade(sptr<QRImage> img): image(img) {
    manager = sptr<BaseCommandManager> (new CommandManager());
    auto cr = PolySceneCreator();
    auto scene = cr.create();
}

void Facade::addCube(double a, double x, double y, double z, QRColor c) {
    auto txt = sptr<QRTexture>(new ColorTexture(c));
    auto cr = sptr<QRPolyModelCreator>(new CubeModelCreator(a, txt));
    auto command = sptr<QRCommand>(new AddModelCmd(cr, Vector3D(x,y,z), scene));
    manager->push(command);
}

void Facade::draw() {
    // todo auto command = sptr<QRCommand>(new DrawCommand(painterCreator->getPainter(), scene));
    //manager->push(command);
   // manager->execAll();
}

void Facade::moveCamera(double dx, double dy, double dz) {
    auto command = sptr<QRCommand>(new MoveCameraCmd(Vector3D(dx,dy,dz), scene));
    manager->push(command);
}
void Facade::scaleCamera(double kx, double ky, double kz) {
    auto command = sptr<QRCommand>(new ScaleCameraCmd(Vector3D(kx,ky,kz), scene));
    manager->push(command);
}
void Facade::rotateCamera(double dx, double dy, double dz) {
    auto command = sptr<QRCommand>(new RotateCameraCmd(Vector3D(dx,dy,dz), scene));
    manager->push(command);
}

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}