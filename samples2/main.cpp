/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 *
 * Edited from xerpi's libvita2d sample.
 *
 * Samples included:
 *   - Basic render target example
 *   - GLM example mixing ortho and perspective drawing
 *   - A simple tile map example
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <vector>
#include <string>

#include <vita2d.h>

#include "Sample.hpp"

vita2d_pvf *pvf = NULL;

int sampleIndex = 0;
SampleRenderTarget sampleRenderTarget;
SampleGLM sampleGLM;
SampleTileMap sampleTileMap;
std::vector<Sample*> samples;

void finiCallback()
{
    for(auto sample : samples)
    {
        sample->finish();
    }
}

int main()
{
    SceCtrlData pad;
    vita2d_pvf *pvf;

    vita2d_init();

    pvf = vita2d_load_default_pvf();

    samples = {
        &sampleRenderTarget,
        &sampleGLM,
        &sampleTileMap
    };

    for(auto sample : samples)
    {
        sample->init();
    }

    memset(&pad, 0, sizeof(pad));

    bool pressed = false;

    while(1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        if(pad.buttons & SCE_CTRL_RTRIGGER)
        {
            if(!pressed)
            {
                sampleIndex = (++sampleIndex) % (int)samples.size();
            }
            pressed = true;
        }
        else
        {
            pressed = false;
        }


        Sample* currentSample = samples[sampleIndex];
        currentSample->update(pad);
        currentSample->draw();

        vita2d_set_blend_mode(VITA2D_BLEND_DEFAULT);
        vita2d_reset_matrix();

        vita2d_pvf_draw_text(pvf, 20, 40, RGBA8(255, 255, 50, 255), 1.0f, "Change Sample with R");
        vita2d_pvf_draw_text(pvf, 20, 80, RGBA8(255, 255, 255, 255), 1.0f, currentSample->name.c_str());
        vita2d_pvf_draw_text(pvf, 20, 100, RGBA8(255, 255, 255, 255), 1.0f, currentSample->controls.c_str());

        vita2d_end_drawing();
        vita2d_wait_rendering_done();
        vita2d_swap_buffers();
    }

    /*
     * vita2d_fini() waits until the GPU has finished rendering,
     * then we can free the assets freely.
     */
    vita2d_fini_callback(&finiCallback);
    vita2d_free_pvf(pvf);

    sceKernelExitProcess(0);
    return 0;
}
