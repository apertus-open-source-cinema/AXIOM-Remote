#include "HelpersGL.h"

uint32_t CreateGLTexture(uint16_t width, uint16_t height, void* pixelData, uint32_t mode, GLint textureFilter,
                         GLint storageFormat)
{
    uint32_t textureID = 0;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, storageFormat, pixelData);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

uint32_t CreateGLTextureFromSurface(SDL_Surface* surface, GLint textureFilter)
{
    uint32_t mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4)
    {
        mode = GL_RGBA;
    }

    return CreateGLTexture(surface->w, surface->h, surface->pixels, mode);
}