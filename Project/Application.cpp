#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"
#include "GA.h"
#include "MyCylinder.h"
#include "SubComponent.h"
#include "Component.h"
#include "OBJ.h"

// GLM - MATHS/VECTORS/MATRICES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <Windows.h>
#include <iostream>
#include <time.h>  

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window);

void Setup(GLFWwindow* window);
void LoadFile();
void DrawGA(GA* ga, Shader* shader, GLFWwindow* window);
void DrawGAInputModel(GA* ga, Shader* shader, GLFWwindow* window);
void DrawOBJModels(std::vector<OBJ*> models, Shader* shader, GLFWwindow* window);

Camera* cam = new Camera(glm::vec3(0.0f, 3.0f, 15.0f));
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

GA* ga = new GA();
Cube* cube1;
int displayIndex = 0;

bool mousePressed = false;
bool simulationBegin = false;

int displayNum = 0;

int main()
{
	// Initialisation of GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader* shader1 = new Shader("Shaders/Vertex.vert", "Shaders/Fragment.frag");
	Shader* shader2 = new Shader("Shaders/Cylinder.vert", "Shaders/Cylinder.frag");
	Shader* shaderMix = new Shader("Shaders/Mix.vert", "Shaders/Mix.frag");
	Shader* colorShader = new Shader("Shaders/Color.vert", "Shaders/Color.frag");
	
	srand(time(NULL));

	OBJ* base = new OBJ();
	base->LoadOBJ("Models/Lamp2/Base.obj");
	base->GenerateBuffers();
	base->SetTransforms(glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1, 0, 0));

	OBJ* pole = new OBJ();
	pole->LoadOBJ("Models/Lamp2/Pole.obj");
	pole->GenerateBuffers();
	pole->SetTransforms(glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1, 0, 0));

	OBJ* shade = new OBJ();
	shade->LoadOBJ("Models/Lamp2/Shade.obj");
	shade->GenerateBuffers();
	shade->SetTransforms(glm::vec3(0, 10, 0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1, 0, 0));	
	shade->CalculateBoundingBox();
	//base->Update();
	//pole->Update();
	//shade->Update();	

	SubComponent* sc1 = new SubComponent();
	sc1->SetOBJ(base);
	sc1->SetColour();

	SubComponent* sc2 = new SubComponent();
	sc2->SetOBJ(pole);
	sc2->SetColour();

	SubComponent* sc3 = new SubComponent();
	sc3->SetOBJ(shade);
	sc3->SetColour();

	sc2->Connect(sc1);
	sc2->m_OBJ->CalculateBoundingBox();
	sc3->Connect(sc2);

	Component* inputComponent = new Component(0);
	inputComponent->AddComponent(sc1);
	inputComponent->AddComponent(sc2);
	inputComponent->AddComponent(sc3);
	inputComponent->Update();

	ga->SetInputModel(inputComponent);
	DrawGAInputModel(ga, shader1, window);
	ga->GenerateInitialPopulation(5);	

	DrawGA(ga, shader1, window);

	glfwTerminate();
	return 0;
}

void DrawGAInputModel(GA* ga, Shader* shader, GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window)) 
	{
		if (simulationBegin) return;

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		shader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(cam->m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader->SetMatrix("projection", projection);

		glm::mat4 view = cam->GetViewMatrix();
		shader->SetMatrix("view", view);

		ga->DisplayInputModel(shader, cam);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DrawGA(GA* ga, Shader* shader, GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.78f, 0.78f, 0.78f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		shader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(cam->m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader->SetMatrix("projection", projection);

		glm::mat4 view = cam->GetViewMatrix();
		shader->SetMatrix("view", view);

		ga->DisplayPopulation(shader, cam, displayNum);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

std::string ReadFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);


	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		//content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Setup(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);

	Shader* shader1 = new Shader("Vertex.vert", "Fragment.frag");

	float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Texture stuff
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Me.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	shader1->Use();

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		shader1->Use();

		glm::mat4 projection = glm::perspective(glm::radians(cam->m_zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader1->SetMatrix("projection", projection);

		glm::mat4 view = cam->GetViewMatrix();
		shader1->SetMatrix("view", view);

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader1->SetMatrix("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void LoadFile()
{
	/*char filename[MAX_PATH];

	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Text Files\0*.txt\0\Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select an OBJ file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		std::cout << "File chosen :" << filename;
	}*/
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		displayNum = 0;
	} 

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		displayNum = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		displayNum = 2;
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		displayNum = 3;
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		displayNum = 4;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->ProcessKeyboard(RIGHT, deltaTime);

		//ga->RotateCurrentModel(glm::vec3(0, 1, 0), deltaTime);

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(1, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(-1, 0, 0));
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(0, 1, 0));
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(0, -1, 0));
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(0, 0, 1));
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		cube1->SetRotation(glm::vec3(0, 0, -1));

	// Start Simulation
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		simulationBegin = true;

	if (glfwGetMouseButton(window, 0) && (!mousePressed))
	{
		mousePressed = true;
		ga->IncrimentDisplay(cam);
	}

	if (!glfwGetMouseButton(window, 0)) mousePressed = false;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	cam->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{	
	ga->AddScore(yoffset);
}