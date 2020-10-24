//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/objects.h"
#include "managers/QRManagers.h"
#include "landscape/QRLandscape.h"
#include "renderStuff.h"
#include "managers/SaveLoadManager.h"

struct ModelInitData {
    int w, h, seed;
    float step;
    ModelInitData(int w, int h, int seed, float step): w(w), h(h), seed(seed), step(step) {}
    ModelInitData(): w(17), h(17), seed(17), step(1) {} // todo hardcode
    sptr<PlateManager> plateManager = nullptr;
};


class Facade {
public:
    Facade(ModelInitData dt, const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img);
    Facade(const sptr<QRImage> &main_img, const sptr<QRImage> &hmap_img);
    void draw();

    void moveCamera(float dx, float dy, float dz);
    void scaleCamera(float kx, float ky, float kz);
    void rotateCamera(float dx, float dy, float dz);

    void changeCamera();
    void setWaterVisible(bool x);
    void setShadesVisible(bool x);
    void erosionIteration();
    void process() {
        builder->process();
        renderer->render();
    }

    void save() {
        if (!landscape) return;
        man.saveSTL(landscape);
        man.save(builder);
    }
    void load(string file) {
        manager = sptr<BaseCommandManager> (new CommandManager());

        sptr<LandscapeBuilder> b = man.load(file);
        if (!b) {
            cerr << "FAILED TO LOAD LANDSCAPE";
            return;
        }

        // scene creation
        auto lightPos = lenNorm(Vector3D(1,1,0.5,0));
        auto cr = PolySceneCreatorNoCamera(lightPos, -1*lightPos);
        scene = cr.create();
        double step = b->getWorldStep();
        double w = b->getHeightMap().width(), h = b->getHeightMap().height();
        auto cam = sptr<QRCamera3D>(new Camera3D(w*step/16.,
                h*step/16., step,
                QRINF, Vector3D(0,-w*step,-h*step),
                Vector3D(6*M_PI/7,0,0),
                false, -step*5));
        scene->addCamera(cam, "observeCamera");

        cam = sptr<QRCamera3D>(new Camera3D(1, 1, step, QRINF,
                                            Vector3D(0,0,-1), Vector3D(M_PI/2,0,0), true));
        scene->addCamera(cam, "walkCamera");
        scene->setActiveCamera("observeCamera");

        builder = b;
        builder->setTools({
            {PlateMountainsTool, freqUNIQUE}
                          });
        landscape = builder->createLandscape();
        builder->waterManager->enableWater();
        builder->waterManager->setWaterMatrix(man.water);
        builder->waterManager->addRainSource();

        builder->climateManager->on_the_7th_day();
        builder->waterManager->updateWater();
        landscape->interpolateColors();

        topDown = sptr<TopDownVisualizer>(new TopDownVisualizer(builder, hmap_image));

        for (auto f = builder->plateManager->getPlates(); f; ++f)
            topDown->addFigure(*f);

        topDown->drawMiniMap();

        scene->clearModels();
        scene->addModel(landscape, Vector3D(0,0,0));
        auto r = sptr<QuickRenderer>(new QuickRenderer(main_image, scene));
        renderer = r;
        renderer->getColorManager()->setWorldStep(builder->getWorldStep());
        shadowRenderer = sptr<QuickShadowRenderer>(new QuickShadowRenderer(r, 0));
        shadowRenderer->generateShades();

        renderer->render();
    }

    void scaleGrid() {
        builder->scaleGrid();
        //builder->useTool(HillTool);
        landscape = builder->createLandscape();
        scene->clearModels();
        scene->addModel(landscape, Vector3D(0,0,0));

        shadowRenderer->generateShades();

        renderer->getColorManager()->setWorldStep(builder->getWorldStep());

        renderer->render();
    }

    void undo();

public:
    sptr<QRImage> main_image, hmap_image;
    sptr<BaseCommandManager> manager;
    sptr<QRPolyScene3D> scene;
    sptr<QuickRenderer> renderer;
    sptr<QuickShadowRenderer> shadowRenderer;
    sptr<TopDownVisualizer> topDown = nullptr;
    sptr<LandscapeBuilder> builder;
    sptr<RoamLandscape> landscape;

    SaveLoadManager man;
};


#endif //KG_LAB3FACADE_H
