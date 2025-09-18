#pragma once
#include <glad/gl.h>
#include <SDL3/SDL.h>

uint32_t CreateGLTexture(uint16_t width, uint16_t height, void* pixelData = nullptr, uint32_t mode = GL_RGB,
                         GLint textureFilter = GL_LINEAR, GLint storageFormat = GL_UNSIGNED_BYTE);

uint32_t CreateGLTextureFromSurface(SDL_Surface* surface, GLint textureFilter = GL_LINEAR);
