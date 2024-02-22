#include "Model.h"

Model::Model(const char* filePath, std::vector<std::vector <Texture>>& textures) {
	Assimp::Importer importer;

	// Load the model
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Process each mesh in the scene
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		std::cout << "Mesh : " << meshIndex + 1 << std::endl;
		auto result = ProcessMesh(mesh);
		vertices = result.first;
		indices = result.second;
		std::cout << "Mesh Loaded"<< std::endl;
		meshes.push_back(Mesh(vertices, indices, textures[meshIndex]));
		std::cout << "\n";
	}
}

std::pair<std::vector<Vertex>, std::vector<GLuint>> Model::ProcessMesh(const aiMesh* mesh) {
	// Vector to store unique vertices
	std::vector<Vertex> uniqueVertices;
	// Vector to store indices
	std::vector<unsigned int> indices;

	// Iterate over each face in the mesh
	for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
		const aiFace& face = mesh->mFaces[faceIndex];

		// Add indices for each vertex in the face
		for (unsigned int index = 0; index < face.mNumIndices; ++index) {
			unsigned int vertexIndex = face.mIndices[index];
			Vertex currentVertex;
			currentVertex.position = glm::vec3(mesh->mVertices[vertexIndex].x, mesh->mVertices[vertexIndex].y, mesh->mVertices[vertexIndex].z);
			currentVertex.normal = glm::vec3(mesh->mNormals[vertexIndex].x, mesh->mNormals[vertexIndex].y, mesh->mNormals[vertexIndex].z);
			currentVertex.texUV = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);

			// Check if the vertex with the same position, normal, and texture UV is already in uniqueVertices
			auto key = std::make_tuple(currentVertex.position, currentVertex.normal, currentVertex.texUV);
			auto it = std::find_if(uniqueVertices.begin(), uniqueVertices.end(),
				[&key](const Vertex& v) {
					return std::tie(v.position, v.normal, v.texUV) == key;
				});

			// If not found, add it to uniqueVertices
			if (it == uniqueVertices.end()) {
				uniqueVertices.push_back(currentVertex);
				indices.push_back(static_cast<unsigned int>(uniqueVertices.size() - 1));
			}
			else {
				// If found, use the existing index
				indices.push_back(static_cast<unsigned int>(std::distance(uniqueVertices.begin(), it)));
			}
		}
	}

	// Print unique vertices and indices
	/*
	std::cout << "Unique Vertices:\n";
	for (const auto& vertex : uniqueVertices) {
		std::cout << "Position: (" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << "), ";
		std::cout << "Normal: (" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << "), ";
		std::cout << "Texture Coordinate: (" << vertex.texUV.x << ", " << vertex.texUV.y << ")\n";
	}
	*/

	std::cout << "Unique Vertices: " << uniqueVertices.size() << std::endl;
	std::cout << "Indices: " << indices.size() << std::endl;

	return std::make_pair(uniqueVertices, indices);
}

void Model::UpdateCamera(Shader& ShaderProgram, Camera& camera) {
	ShaderProgram.Activate();
	glUniform3f(glGetUniformLocation(ShaderProgram.ID, "CameraPosition"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	camera.Matrix(ShaderProgram, "CameraMatrix");
}

void Model::UpdateLight(Shader& ShaderProgram, glm::vec4 lightColor,glm::vec3 lightPosition) {
	ShaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(glGetUniformLocation(ShaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(ShaderProgram.ID, "lightPosition"), lightPosition.x, lightPosition.y, lightPosition.z);
}

void Model::Draw(Shader &ShaderProgram,Camera& camera) {
	for (auto it : meshes) {
		it.Draw(ShaderProgram, camera, false);
	}
}