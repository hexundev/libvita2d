#include "Sample.hpp"

#include <stdlib.h>
#include <vector>

static const int tileMap[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

static const int mapWidth = 16;
static const int mapHeight = 16;


/* Edited draw_texture_scale_rotate_hotspot_generic from vita2d_texture */
static void setVertices(vita2d_texture_vertex* vertices,
                        const vita2d_texture *texture,
                        float x, float y,
                        float tex_x, float tex_y,
                        float tex_w, float tex_h,
                        float x_scale, float y_scale,
                        float rad)
{
    const float w_full = vita2d_texture_get_width(texture);
    const float h_full = vita2d_texture_get_height(texture);

    const float w_half = (tex_w * x_scale) / 2.0f;
    const float h_half = (tex_h * y_scale) / 2.0f;

    const float u0 = tex_x / w_full;
    const float v0 = tex_y / h_full;
    const float u1 = (tex_x + tex_w) / w_full;
    const float v1 = (tex_y + tex_h) / h_full;

    vertices[0].x = -w_half;
    vertices[0].y = -h_half;
    vertices[0].z = +0.5f;
    vertices[0].u = u0;
    vertices[0].v = v0;

    vertices[1].x = w_half;
    vertices[1].y = -h_half;
    vertices[1].z = +0.5f;
    vertices[1].u = u1;
    vertices[1].v = v0;

    vertices[2].x = -w_half;
    vertices[2].y = h_half;
    vertices[2].z = +0.5f;
    vertices[2].u = u0;
    vertices[2].v = v1;

    vertices[3].x = -w_half;
    vertices[3].y = h_half;
    vertices[3].z = +0.5f;
    vertices[3].u = u0;
    vertices[3].v = v1;

    vertices[4].x = w_half;
    vertices[4].y = -h_half;
    vertices[4].z = +0.5f;
    vertices[4].u = u1;
    vertices[4].v = v0;

    vertices[5].x = w_half;
    vertices[5].y = h_half;
    vertices[5].z = +0.5f;
    vertices[5].u = u1;
    vertices[5].v = v1;

    const float c = cosf(rad);
    const float s = sinf(rad);
    int i;
    for(i = 0; i < 6; ++i) {  // Rotate and translate
        float _x = vertices[i].x;
        float _y = vertices[i].y;
        vertices[i].x = _x * c - _y * s + x;
        vertices[i].y = _x * s + _y * c + y;
    }
}


std::vector<int> tiles;
vita2d_texture *tileTexture;
vita2d_vertex_buffer vertices;
glm::vec3 scrollPos(200.0f, 50.0f, 0.0f);

static int getArrayIndex(int x, int y)
{
    return x + mapWidth * y;
}
static int getTile(int x, int y)
{
    if(tiles.empty())
    {
        return 0;
    }
    if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
    {
        return 0;
    }
    return tiles[getArrayIndex(x, y)];
}

static void createTileMap()
{
    if(tiles.empty())
    {
        return;
    }

    int numSolid = 0;
    for(std::size_t i = 0; i < tiles.size(); ++i)
    {
        if(tiles[i] > 0)
        {
            numSolid++;
        }
    }

    int numTileMapVerts = numSolid * 6;
    vertices = vita2d_vertex_buffer_create(numTileMapVerts, VITA2D_VERTEX_TYPE_TEXTURE);

    if(vertices.size == 0)
    {
        return;
    }

    int vertexIndex = 0;
    vita2d_texture_vertex *mapVertices = (vita2d_texture_vertex*)vertices.vertices;
    vita2d_texture_vertex quadVertices[6];
    glm::vec2 tileSize(32.0f, 32.0f);

    for(int i = 0; i < mapWidth; ++i)
    {
        for(int j = 0; j < mapHeight; ++j)
        {
            if(getTile(i, j) <= 0)
            {
                continue;
            }

            float tileImage = (float)(rand() % 4);

            glm::vec2 pos((float)i * tileSize.x, (float)j * tileSize.y);
            pos.x += tileSize.x * 0.5f;
            pos.y += tileSize.y * 0.5f;

            setVertices(quadVertices,
                        tileTexture,
                        pos.x, pos.y,
                        tileImage * 32.0f, 0.0f,
                        32.0f, 32.0f,
                        1.0f, 1.0f,
                        0.0f);

            memcpy(&mapVertices[vertexIndex], &quadVertices[0], 6 * sizeof(vita2d_texture_vertex));
            vertexIndex += 6;
        }
    }
}


void SampleTileMap::init()
{
    name = "Tile Map Sample";
    controls = "Controls:\n\n"
               "D-Pad: Move Camera\n\n"
               "Triangle: Reset";


    tileTexture = vita2d_load_PNG_file("app0:/tiles.png");

    memset(&vertices, 0, sizeof(vita2d_vertex_buffer));

    tiles = std::vector<int>(std::begin(tileMap), std::end(tileMap));
    createTileMap();
}

void SampleTileMap::update(SceCtrlData & pad)
{
    if(pad.buttons & SCE_CTRL_UP)
    {
        scrollPos.y += 2.0f;
    }
    else if(pad.buttons & SCE_CTRL_DOWN)
    {
        scrollPos.y -=  2.0f;
    }
    else if(pad.buttons & SCE_CTRL_RIGHT)
    {
        scrollPos.x -=  2.0f;
    }
    else if(pad.buttons & SCE_CTRL_LEFT)
    {
        scrollPos.x +=  2.0f;
    }
    else if(pad.buttons & SCE_CTRL_TRIANGLE)
    {
        scrollPos = glm::vec3(200.0f, 50.0f, 0.0f);
    }
}

void SampleTileMap::draw()
{
    vita2d_start_drawing();
    vita2d_clear(RGBA8(60, 60, 60, 255));

    vita2d_set_ortho_view_matrix((int)scrollPos.x, (int)scrollPos.y, 960, 544);

    float w = 16.0f * 32.0f;
    vita2d_draw_rectangle(0.0f, 0.0f, w, w, RGBA8(94, 143, 204, 255));
    vita2d_draw_vertex_buffer(tileTexture, &vertices, RGBA8(255, 255, 255, 255));
}

void SampleTileMap::finish()
{
    vita2d_free_texture(tileTexture);
    vita2d_vertex_buffer_free(&vertices);
}
