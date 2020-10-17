//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/objects.h"
#include "managers/QRManagers.h"
#include "landscape/QRLandscape.h"
#include "renderStuff.h"

class Facade {
public:
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

    void scaleGrid() {
        builder->scaleGrid();
        //builder->useTool(HillTool);
        landscape = builder->createLandscape();
        scene->clearModels();
        scene->addModel(landscape, Vector3D(0,0,0));

        shadowRenderer->generateShades();

        renderer->getColorManager()->setWorldStep(builder->getWorldStep());
        //todo QuickShadowRenderer shadowRenderer(r, 0);
        //shadowRenderer.generateShades();

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
};


#endif //KG_LAB3FACADE_H
