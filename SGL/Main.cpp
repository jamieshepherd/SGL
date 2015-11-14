#include <iostream>
#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/soil.h>
#include <GLM/glm.hpp>
#include <GLM/GTC/matrix_transform.hpp>
#include <GLM/GTC/type_ptr.hpp>

#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Make the context of our window the main context on the current thread
	glfwSetKeyCallback(window, key_callback); // Set the required callback functions

	glewExperimental = GL_TRUE; // Setting glewExperimental to true ensures GLEW uses more modern techniques for managing OpenGL functionality

	if (glewInit() != GLEW_OK) // Try to initialise glew
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Create the viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	SGL::Shader ourShader("VS_Shader.glsl", "FS_Shader.glsl");

	// Set up vertex data (and buffer(s)) and attribute pointers
	//GLfloat vertices[] = {
	//  // First triangle
	//   0.5f,  0.5f,  // Top Right
	//   0.5f, -0.5f,  // Bottom Right
	//  -0.5f,  0.5f,  // Top Left 
	//  // Second triangle
	//   0.5f, -0.5f,  // Bottom Right
	//  -0.5f, -0.5f,  // Bottom Left
	//  -0.5f,  0.5f   // Top Left
	//}; 
	GLfloat vertices[] = {
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

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	GLuint VBO, VAO, EBO; // Will store the buffer's unique ID
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // Create a VERTEX BUFFER OBJECT
	glGenBuffers(1, &EBO); // Basically the index buffer (ELEMENT BUFFER OBJECT)
	
	glBindVertexArray(VAO); // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy the VERTICES data to our VBO

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	/////////////////////////////////////////

	GLuint texture;
	glGenTextures(1, &texture); // Generate texture, assign it ID
	glBindTexture(GL_TEXTURE_2D, texture); //Bind the texture to GL_TEXTURE_2D type
	
	int width, height;
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image); // Delete
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind

	/////////////////////////////////////////

	// The game loop
	while (!glfwWindowShouldClose(window)) // For as long as window should NOT close
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		ourShader.Use();

		// Create transformations
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw from EBO
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window); // Essentially the swapchain
	}

	// Properly de-allocate all resources once they've outlived their purpose - SHUTDOWN
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

// Input callback, if a user presses a key this callback is called
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << "Key pressed: " << key << std::endl;
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}