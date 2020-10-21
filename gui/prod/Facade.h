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
        sptr<LandscapeBuilder> b = man.load(file);
        if (!b) {
            cerr << "FAILED TO LOAD LANDSCAPE";
            return;
        }

        builder = b;
        landscape = builder->createLandscape();
        scene->clearModels();
        scene->addModel(landscape, Vector3D(0,0,0));
        builder->waterManager->enableWater();
        builder->waterManager->setWaterMatrix(man.water);

        builder->climateManager->on_the_7th_day();
        builder->waterManager->updateWater();
        landscape->interpolateColors();

        shadowRenderer->generateShades();
        renderer->getColorManager()->setWorldStep(builder->getWorldStep());

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

private:
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
