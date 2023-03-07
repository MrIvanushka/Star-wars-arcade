#ifndef SWTOR_FIELDSCENE_H
#define SWTOR_FIELDSCENE_H

#include "Engine/Scene.h"

class FieldScene : public Scene
{
private:
    CollisionDetector _collisionDetector;

    void initShaders(int GL_VERSION_MAJOR, int GL_VERSION_MINOR) override;
    void initTextures() override;
    void initMaterials() override;
    void initObjects() override;
public:
    FieldScene(int GL_VERSION_MAJOR, int GL_VERSION_MINOR, int framebufferWidth, int framebufferHeight);
};

#endif //SWTOR_FIELDSCENE_H