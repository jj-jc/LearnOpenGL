#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "shader.h"
#include "texture.h"


class ResourceManager
{
public:
    static std::map<std::string, Shader> Shaders; // store Shaders created
    static std::map<std::string, Texture2D> Textures; // store textures created

    ResourceManager(const ResourceManager&) = delete; // avoids the copy constructor
    static ResourceManager& Get() { return s_resourceManager;} // return the reference to the singleton object
    
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShader, std::string name);
    static Shader GetShader(std::string name);
    static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
    static Texture2D GetTexture(std::string name);
    static void Clear();

private:
    static ResourceManager s_resourceManager;
    ResourceManager() {};
    static Shader loadShaderFromFile(const char* vShader, const char* fShaderFile, const char* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};


#endif