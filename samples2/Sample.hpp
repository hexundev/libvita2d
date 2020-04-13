#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>

#include <psp2/ctrl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vita2d.h>

class Sample
{
public:
    std::string name;
    std::string controls;

    virtual void init() = 0;
    virtual void finish() = 0;
    virtual void update(SceCtrlData &pad) = 0;
    virtual void draw() = 0;
};

class SampleRenderTarget: public Sample
{
public:
    void init();
    void finish();
    void update(SceCtrlData &pad);
    void draw();
};

class SampleGLM: public Sample
{
public:
    void init();
    void finish();
    void update(SceCtrlData &pad);
    void draw();
};

class SampleTileMap: public Sample
{
public:
    void init();
    void finish();
    void update(SceCtrlData &pad);
    void draw();
};

#endif // SAMPLE_H
