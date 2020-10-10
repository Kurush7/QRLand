//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"
using namespace std;

// TODO MORE DETAILS ON THE EDGES..... WTF?!!!!!!
// todo top-down inverts x and y... wtf?!
// todo camera: in self-rotate mode moves by X and Y are in camera's coords, but not aligned to camera's rotation
// todo negative scale for camera when in 1st view
// todo frame visibility: hides fuck up?
// todo water manager fucked up
// todo LOD: ignores small details
// todo erosion: water accumulating at the borders

Facade::Facade(const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img)
: main_image(main_img), hmap_image(hmap_img) {
    manager = sptr<BaseCommandManager> (new CommandManager());

    // scene creation
    //auto lightPos = lenNorm(Vector3D(1,1,-3,0));
    auto lightPos = lenNorm(Vector3D(1,1,0.5,0));
    auto cr = PolySceneCreatorNoCamera(lightPos, -1*lightPos);
    scene = cr.create();
    auto cam = sptr<QRCamera3D>(new Camera3D(100, 100, -5,50, 50,
                                             QRINF, Vector3D(0,-100,-150), Vector3D(M_PI,0,0)));
    scene->addCamera(cam, "observeCamera");

    cam = sptr<QRCamera3D>(new Camera3D(1, 1, -5, 20, 20, QRINF,
            Vector3D(0,40, 0), Vector3D(M_PI/2,0,0), true));
    scene->addCamera(cam, "walkCamera");
    scene->setActiveCamera("observeCamera");

    // builder creation
    // DO NOT SET LESSER THAN 64: OR CHANGE FRAME SIZE
    builder = sptr<LandscapeBuilder>(new LandscapeBuilder(
            17, 17, 5));
    topDown = sptr<TopDownVisualizer>(new TopDownVisualizer(builder, hmap_img));

    builder->setTools({
        //{LayerTool, freqAVERAGE},
        /*{HillTool, freqRARE},
        {HillTool, freqRARE},
        {HillTool, freqRARE},
        {HillTool, freqRARE},
        {HillTool, freqRARE},
        {HillTool, freqRARE},
        {HillTool, freqRARE},*/
        {PlateMountainsTool, freqUNIQUE}
    });
    builder->process(0);

    landscape = builder->createLandscape();
    scene->addModel(landscape, Vector3D(0,0,0));

    builder->activateWaterManager();
    builder->waterManager->setWaterLevel(10);
    builder->waterManager->setWaterLevel(0);
    builder->waterManager->addRiverSource(40, 40);
    builder->waterManager->addRiverSource(100, 100);
    builder->waterManager->addRainSource();

    for (auto f = builder->plateManager.getPlates(); f; ++f)
        topDown->addFigure(*f);

    topDown->drawHeightMap();

    auto r = sptr<QuickRenderer>(new QuickRenderer(main_image, scene));
    renderer = r;
    renderer->getColorManager()->setWorldStep(builder->getWorldStep());
    // todo move to renderer itself
    shadowRenderer = sptr<QuickShadowRenderer>(new QuickShadowRenderer(r, 0));
    shadowRenderer->generateShades();


    renderer->render();
    //scene->addModel(sptr<QRPolyModel3D>(new QRLandscapeSurface(2,2, 10)), Vector3D(0,0,0));
    //scene->addModel(RandomHMapLandscapeSurfaceCreator(50, 50, 0.2).create(),
    //       Vector3D(0,0,0));
    //scene->addModel(RoamLandscapeCreator(129, 129, 0.1).create(),
    //       Vector3D(0,0,0));
    //scene->addModel(CubeModelCreator(10, sptr<QRTexture>(new ColorTexture(127,127,127))).create(),Vector3D(0,0,0));
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

void Facade::erosionIteration() {
    static int cnt = 0;
    cnt++;
    builder->waterManager->erosionIteration();
    if (cnt == 20) {
        startMeasureTime;
        builder->climateManager->on_the_7th_day();
        builder->waterManager->updateWater();
        landscape->interpolateColors(); // todo optimize: not each iteration
        cout << "\nwater update: " << endMeasureTime << "\n\n";
        cnt = 0,
        draw();
    }
}

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}