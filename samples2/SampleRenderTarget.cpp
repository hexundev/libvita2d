#include "Sample.hpp"


static int frame = 0;
static vita2d_texture *target;
float w = 400.0f;
float h = 240.0f;
float rot = 0.0f;
float scale = 2.0f;

void SampleRenderTarget::init()
{
    name = "Render Target Sample - 400x240 render target";
    controls = "Controls:\n\n"
               "Up/Down: Change Scale\n\n"
               "Left/Right: Rotate\n\n"
               "Triangle: Reset";

    target = vita2d_create_empty_texture_rendertarget((unsigned int)w, (unsigned int)h, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8);
}

void SampleRenderTarget::update(SceCtrlData &pad)
{
    frame++;

    if(pad.buttons & SCE_CTRL_UP)
    {
        scale += 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_DOWN)
    {
        scale -= 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_RIGHT)
    {
        rot += 0.05f;
    }
    else if(pad.buttons & SCE_CTRL_LEFT)
    {
        rot -= 0.05f;
    }
    else if(pad.buttons & SCE_CTRL_TRIANGLE)
    {
        rot = 0.0f;
        scale = 2.0f;
    }
}

void SampleRenderTarget::draw()
{
    vita2d_start_drawing_advanced(target, 0);
    vita2d_set_ortho_matrix((int)w, (int)h);
    vita2d_clear(RGBA8(0, 0, 0, 255));

    for(float i = 0.0f; i < 8.0f; i += 1.0f)
    {
        int r = (frame * 10 + (int)(i * 40.0f)) % 255;
        float y = sinf((float)frame * 0.2f + i * 2.0f) * 10.0f;
        vita2d_draw_rectangle(10.0f + i * 40.0f, 40.0f + y, 32.0f, 32.0f, RGBA8(r, 255, r, 255));
    }

    vita2d_end_drawing();

    vita2d_start_drawing();
    vita2d_reset_matrix();
    vita2d_clear(RGBA8(60, 60, 60, 255));

    vita2d_draw_texture_scale_rotate(target,
                                     960.0f * 0.5f,
                                     544.0f * 0.5f,
                                     scale, scale,
                                     rot);
}

void SampleRenderTarget::finish()
{
    vita2d_free_texture(target);
}
