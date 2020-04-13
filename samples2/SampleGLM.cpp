#include "Sample.hpp"

vita2d_texture *image;

glm::mat4 matRotation;
glm::mat4 matScale;
glm::mat4 matPerspective;
glm::mat4 matView;

glm::mat4 matTranslateOrtho;
glm::mat4 matViewOrtho;
glm::mat4 matOrtho;

glm::mat4 matMVP;

float rotation = 0.0f;
glm::vec3 camEye(0.0f, 0.0f, 4.0f);

float rad = 0.0f;

void SampleGLM::init()
{
    name = "GLM Sample - Mixing orthographic and perspective";
    controls = "Controls:\n\n"
               "D-Pad: Move Camera\n\n"
               "Square: Forward\n\n"
               "Cross: Backward\n\n"
               "Circle: Rotate\n\n"
               "Triangle: Reset";

    image = vita2d_load_PNG_file("app0:/image.png");

    matScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, -0.01f, 0.01f));
    matPerspective = glm::perspective(45.0f, (960.0f / 544.0f), 0.01f, 100.f);

    matOrtho = glm::ortho(0.0f, 960.0f, 544.0f, 0.0f, 0.01f, 100.f);
    matTranslateOrtho = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    matViewOrtho = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 0.0f, -1.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f));
}

void SampleGLM::update(SceCtrlData &pad)
{
    rad += 0.1f;

    if(pad.buttons & SCE_CTRL_UP)
    {
        camEye.y += 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_DOWN)
    {
        camEye.y -= 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_RIGHT)
    {
        camEye.x += 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_LEFT)
    {
        camEye.x -= 0.01f;
    }
    else if(pad.buttons & SCE_CTRL_SQUARE)
    {
        camEye.z -= 0.1f;
    }
    else if(pad.buttons & SCE_CTRL_CROSS)
    {
        camEye.z += 0.1f;
    }
    else if(pad.buttons & SCE_CTRL_CIRCLE)
    {
        rotation += 0.1f;
    }
    else if(pad.buttons & SCE_CTRL_TRIANGLE)
    {
        rotation = 0.0f;
        camEye = glm::vec3(0.0f, 0.0f, 4.0f);
    }
}

void SampleGLM::draw()
{
    vita2d_start_drawing();
    vita2d_reset_matrix();
    vita2d_clear(RGBA8(60, 60, 60, 255));

    /* Perspective */

    // Model rotation
    matRotation = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0));

    // View matrix
    glm::vec3 camTarget = camEye + glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
    matView = glm::lookAt(camEye, camTarget, upVec);

    matMVP = matPerspective * matView * matRotation * matScale;
    vita2d_set_matrix((const float*)glm::value_ptr(matMVP));

    vita2d_draw_texture_rotate(image, 0.0f, 0.0f, 0.0f);


    /* Ortho */
    matMVP = matOrtho * matViewOrtho * matTranslateOrtho;
    vita2d_set_matrix((const float*)glm::value_ptr(matMVP));

    for(int i = 0; i < 10; ++i)
    {
        float x = (float)(i % 9) * 100.0f;
        float y = 540.0f * 0.5f;
        vita2d_draw_texture_tint_scale_rotate(image, x, y, 0.4f, 0.4f, rad, RGBA8(100, 100, 255, 100));
    }

}

void SampleGLM::finish()
{
    vita2d_free_texture(image);
}
