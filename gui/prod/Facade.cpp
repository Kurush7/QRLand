//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"
using namespace std;

// MORE DETAILS ON THE EDGES.....?!!!!!!
//  negative scale for camera when in 1st view
// camera 1st view: bugged rotations
// beyound the right border - draws column on the left... filler solved the problem

// water source at 0,0: not flowing anywhere: hardcoded solution
// erosion: water accumulating at the borders: check now

// todo guide
// todo input validation

// dynamic tense for mountains..... almost
// shadows: bigger image size, some bugs
// todo thread_cnt=8: label
// single pixels problem: almost solved
// todo not growing down
// todo too long shadowing

Facade::Facade(const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img)
: main_image(main_img), hmap_image(hmap_img) {

}

Facade::Facade(ModelInitData data, const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img)
: main_image(main_img), hmap_image(hmap_img) {
    manager = sptr<BaseCommandManager> (new CommandManager());

    // scene creation
    auto lightPos = lenNorm(Vector3D(-1,-1,0.5,0));
    auto cr = PolySceneCreatorNoCamera(lightPos, -1*lightPos);
    scene = cr.create();
    auto cam = sptr<QRCamera3D>(new Camera3D(data.w*data.step/16., data.h*data.step/16., data.step,
            QRINF, Vector3D(0,-data.w*data.step,-data.h*data.step), Vector3D(6*M_PI/7,0,0),
            false, -data.step*5));
    scene->addCamera(cam, "observeCamera");

    cam = sptr<QRCamera3D>(new Camera3D(1, 1, data.step, QRINF,
            Vector3D(0,0,-1), Vector3D(M_PI/2,0,0), true));
    scene->addCamera(cam, "walkCamera");
    scene->setActiveCamera("observeCamera");

    // builder creation
    builder = sptr<LandscapeBuilder>(new LandscapeBuilder(
            data.w, data.h, data.step));
    builder->setPlateManager(data.plateManager);
    topDown = sptr<TopDownVisualizer>(new TopDownVisualizer(builder, hmap_img));

    builder->setTools({
        //{LayerTool, freqAVERAGE},
        //{HillTool, freqRARE},
        {PlateMountainsTool, freqUNIQUE}
    });

    landscape = builder->createLandscape();
    scene->addModel(landscape, Vector3D(0,0,0));

    builder->waterManager->enableWater();
    //builder->waterManager->setWaterLevel(0);
    //builder->waterManager->addRiverSource(40, 40);
    //builder->waterManager->addRiverSource(100, 100);

    for (auto f = builder->plateManager->getPlates(); f; ++f)
        topDown->addFigure(*f);
    topDown->drawMiniMap();

    auto r = sptr<QuickRenderer>(new QuickRenderer(main_image, scene));
    renderer = r;
    renderer->getColorManager()->setWorldStep(builder->getWorldStep());
    shadowRenderer = sptr<QuickShadowRenderer>(new QuickShadowRenderer(r, 0));
    shadowRenderer->generateShades(builder->getWidth(),
                                   builder->getHeight());

    renderer->render();
}

void Facade::draw() {
    auto command = sptr<QRCommand>(new RenderCmd(renderer));
    manager->push(command);
    manager->execAll();
    //topDown->drawHeightMap();
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

void Facade::setWaterVisible(bool x) {
    builder->waterManager->setWaterStatus(x);
    renderer->render();
}
void Facade::setShadesVisible(bool x) {
    renderer->setUseShades(x);
    renderer->render();
}

void Facade::erosionIteration(bool draw_flag, bool useTools) {
    builder->waterManager->erosionIteration(defaultErosionDT, useTools);

    if (draw_flag) renderer->render();

}

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}