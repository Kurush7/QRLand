//
// Created by kurush on 29.05.2020.
//

#include "Lab3Facade.h"

using namespace std;

Lab3Facade::Lab3Facade(sptr<BasePainterCreator> cr): painterCreator(cr) {
    manager = sptr<BaseCommandManager> (new CommandManager());
    factory = sptr<QRObject3DFactory> (new SimpleBasicObject3DFactory());

    auto director = SceneBuildDirector();
    auto builder = sptr<BaseScene3DBuilder>(new Scene3DBuilder(factory, 40, 40));
    scene = director.build(builder);
}

void Lab3Facade::addModel(string filename) {
    auto data = sptr<LoadData>(new FileLoadData(filename));
    auto source = sptr<LoadSource>(new FileSource(data));
    auto loader = sptr<BaseFrame3DLoader> (new Frame3DLoader(source, factory));
    auto director = FrameLoadDirector();
    auto command = sptr<QRCommand>(new AddModelCommand(director, loader, scene));
    manager->push(command);
}

void Lab3Facade::draw() {
    auto command = sptr<QRCommand>(new DrawCommand(painterCreator->getPainter(), scene));
    manager->push(command);
    manager->execAll();
}


void Lab3Facade::move(double dx, double dy, double dz) {
    auto creator = sptr<BaseTransformer3DCreator> (new MoveTransformer3DCreator(dx,dy,dz));
    auto trans = sptr<BaseTransformer3D>(creator->create());

    sptr<QRCommand> command;
    if(scene->isEmptySelection())
        command = sptr<QRCommand>(new TransformCameraCommand(trans, scene));
    else
        command = sptr<QRCommand>(new MoveTransformSelectionCommand(trans, scene));
    //cout << "move by: " << dx << ' ' << dy << ' ' << dz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}
void Lab3Facade::scale(double kx, double ky, double kz) {
    auto creator = sptr<BaseTransformer3DCreator> (new ScaleTransformer3DCreator(kx,ky,kz));
    auto trans = sptr<BaseTransformer3D>(creator->create());

    sptr<QRCommand> command;
    if(scene->isEmptySelection())
        command = sptr<QRCommand>(new ScaleCameraCommand(trans, scene));
    else
        command = sptr<QRCommand>(new TransformSelectionCommand(trans, scene));

    //cout << "scale by: " << kx << ' ' << ky << ' ' << kz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}
void Lab3Facade::rotate(double dx, double dy, double dz) {
    auto creator = sptr<BaseTransformer3DCreator> (new RotateTransformer3DCreator(dx,dy,dz));
    auto trans = sptr<BaseTransformer3D>(creator->create());

    sptr<QRCommand> command;
    if(scene->isEmptySelection())
        command = sptr<QRCommand>(new TransformCameraCommand(trans, scene));
    else
        command = sptr<QRCommand>(new TransformSelectionCommand(trans, scene));

    //cout << "rotate by: " << dx << ' ' << dy << ' ' << dz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}

void Lab3Facade::select(double x, double y) {
    auto command = sptr<QRCommand>(new SelectCommand(x, y, scene));
    //cout << "select by: " << x << ' ' << y <<"\n";
    //cout << "select by: " << x << ' ' << y <<"\n";
    manager->push(command);
}

void Lab3Facade::setColor(QRColor pointcolor, QRColor edgeColor) {
    auto keeper = ColorKeeper(QRPointStyle(pointcolor), QREdgeStyle(edgeColor));
    auto command = sptr<QRCommand>(new SetColorSelectionCommand(keeper, scene));
    manager->push(command);
}

void Lab3Facade::deleteSelection() {
    auto command = sptr<QRCommand>(new DeleteSelectionCommand(scene));
    manager->push(command);
}

void Lab3Facade::undo() {
    if (manager->isEmptyUndo())
        return;
    manager->undo();
}