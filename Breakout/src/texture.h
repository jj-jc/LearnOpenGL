#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture2D
{
public:
    unsigned int ID;
    // texture information
    unsigned int Width, Height;
    unsigned int Internal_Format;
    unsigned int Image_Format;
    // texture configuration
    unsigned int Wrap_S; 
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;
    // constructor
    Texture2D();
    // ~Texture2D();
    // utilities
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;
};


#endif