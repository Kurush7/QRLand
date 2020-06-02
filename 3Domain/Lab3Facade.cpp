//
// Created by kurush on 29.05.2020.
//

#include "Lab3Facade.h"

using namespace std;

Lab3Facade::Lab3Facade(std::shared_ptr<BasePainterCreator> cr): painterCreator(cr) {
    manager = shared_ptr<BaseCommandManager> (new CommandManager());
    factory = shared_ptr<AbstractObject3DFactory> (new BasicObject3DFactory());

    auto director = SceneBuildDirector();
    auto builder = shared_ptr<BaseScene3DBuilder>(new Scene3DBuilder(factory, 40, 40));
    scene = director.build(builder);
}

void Lab3Facade::addModel(string filename) {
    auto data = shared_ptr<LoadData>(new FileLoadData(filename));
    auto source = shared_ptr<LoadSource>(new FileSource(data));
    auto loader = shared_ptr<BaseFrame3DLoader> (new Frame3DLoader(source, factory));
    auto director = FrameLoadDirector();
    auto command = shared_ptr<BaseCommand>(new AddModelCommand(director, loader, scene));
    manager->push(command);
}

void Lab3Facade::draw() {
    auto command = shared_ptr<BaseCommand>(new DrawCommand(painterCreator->getPainter(), scene));
    manager->push(command);
    manager->execAll();
}


void Lab3Facade::move(double dx, double dy, double dz) {
    auto creator = shared_ptr<BaseTransformer3DCreator> (new MoveTransformer3DCreator(dx,dy,dz));
    auto trans = shared_ptr<BaseTransformer3D>(creator->create());

    shared_ptr<BaseCommand> command;
    if(scene->isEmptySelection())
        command = shared_ptr<BaseCommand>(new TransformCameraCommand(trans, scene));
    else
        command = shared_ptr<BaseCommand>(new MoveTransformSelectionCommand(trans, scene));
    //cout << "move by: " << dx << ' ' << dy << ' ' << dz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}
void Lab3Facade::scale(double kx, double ky, double kz) {
    auto creator = shared_ptr<BaseTransformer3DCreator> (new ScaleTransformer3DCreator(kx,ky,kz));
    auto trans = shared_ptr<BaseTransformer3D>(creator->create());

    shared_ptr<BaseCommand> command;
    if(scene->isEmptySelection())
        command = shared_ptr<BaseCommand>(new ScaleCameraCommand(trans, scene));
    else
        command = shared_ptr<BaseCommand>(new TransformSelectionCommand(trans, scene));

    //cout << "scale by: " << kx << ' ' << ky << ' ' << kz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}
void Lab3Facade::rotate(double dx, double dy, double dz) {
    auto creator = shared_ptr<BaseTransformer3DCreator> (new RotateTransformer3DCreator(dx,dy,dz));
    auto trans = shared_ptr<BaseTransformer3D>(creator->create());

    shared_ptr<BaseCommand> command;
    if(scene->isEmptySelection())
        command = shared_ptr<BaseCommand>(new TransformCameraCommand(trans, scene));
    else
        command = shared_ptr<BaseCommand>(new TransformSelectionCommand(trans, scene));

    //cout << "rotate by: " << dx << ' ' << dy << ' ' << dz <<"\nmatrix:" << trans->getMatrix();
    manager->push(command);
}

void Lab3Facade::select(double x, double y) {
    auto command = shared_ptr<BaseCommand>(new SelectCommand(x, y, scene));
    //cout << "select by: " << x << ' ' << y <<"\n";
    //cout << "select by: " << x << ' ' << y <<"\n";
    manager->push(command);
}

void Lab3Facade::setColor(QRColor pointcolor, QRColor edgeColor) {
    auto keeper = ColorKeeper(QRPointStyle(pointcolor), QREdgeStyle(edgeColor));
    auto command = shared_ptr<BaseCommand>(new SetColorSelectionCommand(keeper, scene));
    manager->push(command);
}

void Lab3Facade::deleteSelection() {
    auto command = shared_ptr<BaseCommand>(new DeleteSelectionCommand(scene));
    manager->push(command);
}

void Lab3Facade::undo() {
    if (manager->isEmptyUndo())
        return;
    manager->undo();
}