#pragma once

#include <unordered_map>
#include <string>
#include <memory>

class Mesh;
class Texture;

using MeshManagerType = std::unordered_map<std::string, std::shared_ptr<Mesh>>;
using TextureManagerType = std::unordered_map<std::string, std::shared_ptr<Texture>>;

struct ResourceManager
{
	static MeshManagerType meshManager;
	static TextureManagerType textureManager;
};

