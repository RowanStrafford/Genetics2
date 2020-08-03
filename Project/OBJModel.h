/*#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class OBJModel
{
public:
	OBJModel(std::string path);
	~OBJModel();

	void Draw(Shader &shader);
private:
	std::vector<Mesh> m_vMeshes;
	std::string m_directory;
	std::vector<Texture> m_textures_loaded;

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, const std::string &directory);
};
*/
