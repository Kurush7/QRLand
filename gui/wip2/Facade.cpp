//
// Created by kurush on 29.05.2020.
//

#include "Facade.h"
using namespace std;

// TODO MORE DETAILS ON THE EDGES..... WTF?!!!!!!
// todo top-down inverts x and y... wtf?!
// todo camera: in self-rotate mode moves by X and Y are in camera's coords, but not aligned to camera's rotation

Facade::Facade(const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img)
: main_image(main_img), hmap_image(hmap_img) {
    manager = sptr<BaseCommandManager> (new CommandManager());

    // scene creation
    //auto lightPos = lenNorm(Vector3D(1,1,-3,0));
    auto lightPos = lenNorm(Vector3D(1,1,0.5,0));
    auto cr = PolySceneCreatorNoCamera(lightPos, -1*lightPos);
    scene = cr.create();
    //auto cam = sptr<QRCamera3D>(new Camera3D(50, 50, -5,50, 1,
    //                                         QRINF, Vector3D(0,0,-110), Vector3D(3*M_PI/5,0,0)));
    auto cam = sptr<QRCamera3D>(new Camera3D(100, 100, -5,50, 50,
                                             QRINF, Vector3D(0,-100,-150), Vector3D(M_PI,0,0)));
    scene->addCamera(cam, "observeCamera");

    cam = sptr<QRCamera3D>(new Camera3D(1, 1, -5, 20, 20, QRINF,
            Vector3D(0,40, 0), Vector3D(M_PI/2,0,0), true));
    scene->addCamera(cam, "walkCamera");
    scene->setActiveCamera("observeCamera");

    // renderer creation
    //renderer = sptr<QRenderer>(new FullThreadRenderer(main_image, scene));
    // builder creation
    builder = sptr<LandscapeBuilder>(new LandscapeBuilder(
            129, 129, 1, 1)); // another world step ruins all???
    topDown = sptr<TopDownVisualizer>(new TopDownVisualizer(builder, hmap_img));

    builder->setTools({
        {LayerTool, freqAVERAGE},
        {HillTool, freqRARE},
        //{PlateMountainsTool, freqUNIQUE}
    });
    builder->process(100);
    builder->useTool(PlateMountainsTool);

    sptr<QRPolyModel3D> land = builder->createLandscape();
    scene->addModel(land, Vector3D(0,0,0));

    builder->activateWaterManager();
    //builder->waterManager->setWaterLevel(35);
    builder->waterManager->setWaterLevel(30);

    for (auto f = builder->plateManager.getPlates(); f; ++f)
        topDown->addFigure(*f);

    topDown->drawHeightMap();

    auto r = sptr<QuickRenderer>(new QuickRenderer(main_image, scene));
    renderer = r;
    renderer->getColorManager()->setWorldStep(1);
    QuickShadowRenderer shadowRenderer(r, 0);
    shadowRenderer.generateShades();

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

void Facade::erosionIteration() {
    static int cnt = 0;
    cnt++;
    startMeasureTime;
    builder->waterManager->erosionIteration();
    cout << "erosion finished in " << endMeasureTime << '\n';
    if (cnt == 10)
        cnt = 0, draw();
}

void Facade::undo() {
    if (manager->isEmptyUndo()) return;
    manager->undo();
}