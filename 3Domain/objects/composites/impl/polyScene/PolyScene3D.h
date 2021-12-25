//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_POLYSCENE3D_H
#define BIG3DFLUFFY_POLYSCENE3D_H

#include "../../interfaces/QRPolyScene3D.h"
#include "objects/lights/light.h"

class PolyScene3D: public QRPolyScene3D {
public:
    PolyScene3D() {activeCamera = "NO CAMERA IS SET!";}
    PolyScene3D(const sptr<QRCamera3D> &c, std::string name="defaultCamera") {
        cameras[name] = c;
        activeCamera = name;
    }
    virtual bool addModel(const sptr<QRPolyModel3D> &m, const Vector3D &v);
    virtual void clearModels() {models.clear();}

    virtual std::string getActiveCameraName() {return activeCamera;}
    virtual sptr<QRCamera3D> getActiveCamera() {
        if (cameras.find(activeCamera) == cameras.end())
            throw QRBadParamException(__FILE__,__LINE__, __TIME__, "camera is not set or not found");
        return cameras[activeCamera];}

    virtual bool addCamera(const sptr<QRCamera3D> &c, std::string name) {
        if (cameras.find(name) == cameras.end()) {
            cameras[name] = c;
            return true;
        }
        return false;
    }
    virtual bool setActiveCamera(std::string name) {
        if (cameras.find(name) != cameras.end()) {
            activeCamera = name;
            return true;
        }
        return false;
    }

    virtual RawModelIterator getModels() const {return models.begin();}
    virtual void setModels(RawModelIterator it) {models = it;}

    virtual void addLight(const sptr<QRLight> &light) {lights.push_back(light);}
    virtual LightIterator getLights() {return lights.begin();}

    // todo
    //virtual void deleteModel(const ModelIterator &it) {models.pop(it);}
    //virtual bool operator==(const QRPolyScene3D &b) const;
    //virtual QRPolyScene3D& operator=(const QRPolyScene3D &p);

private:
    QRVector<RawModel> models;
    QRVector<sptr<QRLight>> lights;

    std::string activeCamera;
    std::map<std::string, sptr<QRCamera3D>> cameras;

};

#endif //BIG3DFLUFFY_POLYSCENE3D_H
