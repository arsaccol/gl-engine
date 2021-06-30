// Yeah, this class is entirely copied from JoeyDeVries' tutorial 
// at LearnOpenGL.com

#pragma once
#include <vector>
#include "Vertex.hpp"
#include "Shader.hpp"

#include <iostream>

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh
{

	// attribute indices related to shader layout
	enum Attributes
	{
		Position = 0,
		TextureCoordinates = 1,
		Color = 2,
		//Normal = 3
	};
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: vertices{ vertices }
		, indices{ indices }
		//, VAO{ 0 }, VBO{ 0 }, EBO{ 0 }
	{
		setupMesh();
	}

	// Model loading constructor
	// only loads a single mesh, so I guess it's better suited for .obj files?
	Mesh(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
		const aiNode* rootNode = scene->mRootNode;

		aiMesh* mesh = scene->mMeshes[0];

		// copy vertices
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			// do processing here
			auto [x, y, z] = mesh->mVertices[i];
			vertex.Position = glm::vec3{ x, y, z };
			vertex.Color = glm::vec3{ 1 };

			vertices.push_back(vertex);
		}


		// copy indices
		for(size_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t i = 0; i < face.mNumIndices; ++i)
			{
				indices.push_back(face.mIndices[i]);
			}
		}

		setupMesh();
	}


	void draw(const ShaderProgram& shaderProgram)
	{
		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}



	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// prepare vertex data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


		// prepare index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


		// setup Position attribute
		glVertexAttribPointer(Attributes::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(Attributes::Position);

		// setup TextureCoordinates attribute
		glVertexAttribPointer(Attributes::TextureCoordinates, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinates));
		glEnableVertexAttribArray(Attributes::TextureCoordinates);

		// setup Color attribute
		glVertexAttribPointer(Attributes::Color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		glEnableVertexAttribArray(Attributes::Color);

		// setup Normal attribute
		//glVertexAttribPointer(Attributes::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//glEnableVertexAttribArray(Attributes::Normal);


		glBindVertexArray(0);
	}


private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO, VBO, EBO;
};
