#include "Model.h"
#include "Skybox.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;


float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

std::string CubemapFolder = "lake";
std::string CubeMapTextureFaces[6] =
{
	"./textures/skybox/" + CubemapFolder + "/right.jpg",
	"./textures/skybox/" + CubemapFolder + "/left.jpg",
	"./textures/skybox/" + CubemapFolder + "/top.jpg",
	"./textures/skybox/" + CubemapFolder + "/bottom.jpg",
	"./textures/skybox/" + CubemapFolder + "/front.jpg",
	"./textures/skybox/" + CubemapFolder + "/back.jpg"
};

int main() {

	if (!glfwInit()) {
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Project", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		return -1;
	}


	glEnable(GL_DEPTH_TEST);

	Shader lightShader("./shaders/light-vertex.glsl", "./shaders/light-fragment.glsl");
	Shader BasicProgram("./shaders/vertex.glsl", "./shaders/fragment.glsl","./shaders/explode-geometry.glsl");
	Shader SkyboxShaderProgram("./shaders/skybox-vertex.glsl", "./shaders/skybox-fragment.glsl");


	// Loading In Textures with Different Filtering Options
	std::vector<std::vector<Texture>>StandardTextures
	{
		{
			Texture("./textures/checks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE,"standard"),
		},
	};

	std::vector<std::vector<Texture>>BiLinearTextures
	{
		{
			Texture("./textures/checks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE,"bilinear"),
		},
	};
	std::vector<std::vector<Texture>>TriLinearTextures
	{
		{
			Texture("./textures/checks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE,"trilinear"),
		},
	};


	//Main Light
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPosition = glm::vec3(0.0f, 3.5f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPosition);
	lightModel = glm::scale(lightModel, glm::vec3(1.0f, 1.0f, 1.0f)*0.1f);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	SkyboxShaderProgram.Activate();
	glUniform1i(glGetUniformLocation(SkyboxShaderProgram.ID, "skybox"), 0);

	BasicProgram.Activate();
	glUniform1i(glGetUniformLocation(BasicProgram.ID, "skybox"), 0);

	Model Light("./models/ball.obj", StandardTextures);
	Light.model = lightModel;

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT, glm::vec3(0.0f, 1.0f, 2.0f));
	
	const char modelName[] = "./models/flatBox.obj";

	std::cout << "\nBasic: " << std::endl;
	Model Basic(modelName, StandardTextures);
	glm::vec3 BasicPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 Basic_model = glm::mat4(1.0f);
	Basic_model = glm::translate(Basic_model, BasicPosition);
	Basic_model = glm::scale(Basic_model, glm::vec3(1.0f, 1.0f, 1.0f) * 10.7f);
	Basic.model = Basic_model;
	Basic.UpdateLight(BasicProgram, lightColor, lightPosition);
	
	std::cout << "\nBiLinear: " << std::endl;
	Model BiLinear(modelName, BiLinearTextures);
	glm::vec3 BiLinearPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 BiLinear_model = glm::mat4(1.0f);
	BiLinear_model = glm::translate(BiLinear_model, BiLinearPosition);
	BiLinear_model = glm::scale(BiLinear_model, glm::vec3(1.0f, 1.0f, 1.0f) * 10.7f);
	BiLinear.model = BiLinear_model;
	BiLinear.UpdateLight(BasicProgram, lightColor, lightPosition);

	std::cout << "\nTriLinear: " << std::endl;
	Model TriLinear(modelName, TriLinearTextures);
	glm::vec3 TriLinearPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 TriLinear_model = glm::mat4(1.0f);
	TriLinear_model = glm::translate(TriLinear_model, TriLinearPosition);
	TriLinear_model = glm::scale(TriLinear_model, glm::vec3(1.0f, 1.0f, 1.0f) * 10.7f);
	TriLinear.model = TriLinear_model;
	TriLinear.UpdateLight(BasicProgram, lightColor, lightPosition);

	// Skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int CubeMapTexture;
	glGenTextures(1, &CubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//Completed Skybox

	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, numberOfChannels;
		unsigned char* data = stbi_load(CubeMapTextureFaces[i].c_str(), &width, &height, &numberOfChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
			std::cout << " Loaded Texture For Skybox : " << CubeMapTextureFaces[i].c_str() << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture: " << CubeMapTextureFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}

    while (!glfwWindowShouldClose(window)) {
		
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.UpdateMatrix(45.0f, 0.1f, 1000.0f);
		
		Basic.UpdateCamera(BasicProgram, camera);
		BiLinear.UpdateCamera(BasicProgram, camera);
		BiLinear.UpdateCamera(BasicProgram, camera);
		
		Basic.UpdateLight(BasicProgram, lightColor, lightPosition);
		BiLinear.UpdateLight(BasicProgram, lightColor, lightPosition);
		TriLinear.UpdateLight(BasicProgram, lightColor, lightPosition);

		lightShader.Activate();
		camera.Matrix(lightShader, "CameraMatrix");
		

		switch (camera.filteringMode) {
		case 1:
			Basic.Draw(BasicProgram, camera);
			break;
		case 2:
			BiLinear.Draw(BasicProgram, camera);
			break;
		case 3:
			TriLinear.Draw(BasicProgram, camera);
			break;
		}

		Light.Draw(lightShader, camera);
		

		//Skybox
		glDepthFunc(GL_LEQUAL);
		SkyboxShaderProgram.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyboxShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		//Skybox End

		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
