/*
--------------------------------------------------------------------
 Main.cpp
--------------------------------------------------------------------
 The main entry point for the program
--------------------------------------------------------------------
*/

#include "Common.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Skybox.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void input_process();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Mouse
GLfloat yaw = -90.0f; // Due to Eular angles
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat fov   = 45.0f;

// Input
bool keys[1024]; // 1024 key array whether a key is pressed

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current and last frame
GLfloat lastFrame = 0.0f; // Time of last frame

int main()
{
#ifndef _DEBUG // Hide our console window if we're in release mode
	FreeConsole(); 
#endif
	std::cout << "Starting GLFW context, OpenGL 4.5" << std::endl;

	glfwInit(); // In the main function we first initialize GLFW with glfwInit
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL major version 4 (OpenGL 4.5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // OpenGL minor version 5 (OpenGL 4.5)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // The window should not be resizable
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // If we're on Mac OS X we need this too

	// Create a new window object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ShepherdGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Make the context of our window the main context on the current thread

	glfwSwapInterval(0); // Disable vysnc

	glfwSetKeyCallback(window, key_callback); // Set the required callback functions
	glfwSetCursorPosCallback(window, mouse_callback); // Callback for when mouse position changes
	glfwSetScrollCallback(window, scroll_callback); // Callback for when scrollwheel scrolls

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide the cursor

	glewExperimental = GL_TRUE; // Setting glewExperimental to true ensures GLEW uses more modern techniques for managing OpenGL functionality

	if (glewInit() != GLEW_OK) // Try to initialise glew
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Create the viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	// Depth test
	glEnable(GL_DEPTH_TEST);

	//Shader lightingShader("Shaders/VS_Lighting.glsl", "Shaders/FS_Lighting.glsl");
	//Shader lampShader("Shaders/VS_Lamp.glsl", "Shaders/FS_Lamp.glsl");

	Shader modelShader("Shaders/VS_Model.glsl", "Shaders/FS_Model.glsl");
	Model ourModel("Models/nanosuit/nanosuit.obj");
	Skybox skybox();

	// Draw in wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// The game loop
	while (!glfwWindowShouldClose(window)) // For as long as window should NOT close
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		input_process();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		// Change light position values over time
		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		// Lighting stuff
		modelShader.Use();

		// View
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Projection 
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(modelShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(modelShader);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}






// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}





// Input callback, if a user presses a key this callback is called
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 	closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void input_process()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	std::cout << deltaTime << std::endl;

	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Polygon mode
	if (keys[GLFW_KEY_F1])
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (keys[GLFW_KEY_F2])
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (float)xpos - lastX;
	GLfloat yoffset = lastY - (float)ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (float)xpos;
	lastY = (float)ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}