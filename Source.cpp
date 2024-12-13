#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
#include <iostream>

using namespace std;
using namespace glm;

enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};
vec3 cameraPosition(0, 0, 3);
vec3 cameraDirection(0, 0, -1);
vec3 cameraUp(0, 1, 0);
struct vertex
{
	vec3 position;
	vec3 color;
	vertex() {}
	vertex(vec3 _position, vec3 _color) :position{ _position }, color{ _color }
	{
	}
	vertex(vec3 _position) :position{ _position }, color{ _position }
	{
	}
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO_Triangle, VBO_Cube, IBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::FilledTriangle;

// transformation
GLuint modelMatLoc, viewMatLoc, projMatLoc, colorLoc, boolcolorLoc;

void CreateTriangle()
{
	vertex TriangleVertices[] =
	{
		vec3(-1,-1,0),vec3(0,1,0),
		vec3(1,-1,0),vec3(0,0,1),
		vec3(0,1,0),vec3(1,0,0)
	};

	// create buffer object
	glGenBuffers(1, &VBO_Triangle);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//GLuint Vertex_Position_Location = glGetAttribLocation(BasiceprogramId, "vertex_position");
}

void BindTriangle()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
}

void CreateRibbon()
{
	// Define vertices for a small triangle (the ribbon)
	vertex RibbonVertices[] =
	{
		vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // Top point (Red)
		vec3(0.0f, 0.1f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // Bottom-left (Red)
		vec3(0.1f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)  // Bottom-right (Red)
	};

	// Create buffer object
	glGenBuffers(1, &VBO_Triangle);

	// Bind buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RibbonVertices), RibbonVertices, GL_STATIC_DRAW);

	// Define vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);
}

vertex cube_core_vertices[] = {
	{vec3(-0.5, 0.5, 0.5), vec3(1, 1, 1)},
	{vec3(-0.5, -0.5, 0.5), vec3(1, 1, 1)},
	{vec3(0.5, -0.5, 0.5), vec3(1, 1, 1)},
	{vec3(0.5, 0.5, 0.5), vec3(1, 1, 1)},
	{vec3(0.5, 0.5, -0.5), vec3(0, 0, 0)},
	{vec3(0.5, -0.5, -0.5), vec3(0, 0, 0)},
	{vec3(-0.5, -0.5, -0.5), vec3(0, 0, 0)},
	{vec3(-0.5, 0.5, -0.5), vec3(0, 0, 0)}
};

void CreateCube()
{
	int vertices_Indeces[] = {
		//front
		0,
		1,
		2,

		0,
		2,
		3,
		//Right
		3,
		2,
		5,

		3,
		5,
		4,
		//Back
		4,
		5,
		6,

		4,
		6,
		7,
		//Left
		7,
		6,
		1,

		7,
		1,
		0,
		//Top
		7,
		0,
		3,

		7,
		3,
		4,
		//Bottom
		2,
		1,
		6,

		2,
		6,
		5
	};

	// create VBO
	glGenBuffers(1, &VBO_Cube);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_core_vertices), cube_core_vertices, GL_STATIC_DRAW);

	// Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices_Indeces), vertices_Indeces, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);

}

void BindCube()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (char*)(2 * sizeof(vec3)));
	glEnableVertexAttribArray(2);
}

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	CreateTriangle();
	CreateCube();
	CreateRibbon();
	modelMatLoc = glGetUniformLocation(BasiceprogramId, "modelMat");
	viewMatLoc = glGetUniformLocation(BasiceprogramId, "viewMat");
	projMatLoc = glGetUniformLocation(BasiceprogramId, "projMat");
	colorLoc = glGetUniformLocation(BasiceprogramId, "baccolor");
	boolcolorLoc = glGetUniformLocation(BasiceprogramId, "useCustomColor");



	glm::mat4 projMat = glm::perspectiveFov(60.0f, (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));


	glClearColor(0, 0.5, 0.5, 1);
	glEnable(GL_DEPTH_TEST);

	return 0;
}

float theta = 0;
void Update()
{
	theta += 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (Current_DrawingMode)
	{
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	BindCube();

	mat4 penguinTransform = glm::rotate(theta * 180 / 3.14f, glm::vec3(0, 1, 0));

	// Body
	mat4 bodyTransform = penguinTransform *
		glm::translate(glm::vec3(0, -0.5f, 0)) *
		glm::scale(glm::vec3(0.5f, 1.0f, 0.5f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(bodyTransform));
	glUniform1f(boolcolorLoc, 0.f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);


	// Head
	mat4 headTransform = penguinTransform *
		glm::translate(glm::vec3(0.0f, 0.15f, 0)) *
		glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(headTransform));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Ribbon
	//mat4 ribbonTransform = penguinTransform *
	//	glm::translate(glm::vec3(0.0f, 0.3f, 0.0f)) *  // Position slightly above the head
	//	glm::scale(glm::vec3(0.5f, 0.09f, 0.2f));
	//glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ribbonTransform));
	//glUniform4f(colorLoc, 1, 0.208, 0.471  , 1.0f); 
	//glUniform1f(boolcolorLoc, 1.f);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	mat4 ribbonTransform1 = penguinTransform *
		glm::translate(glm::vec3(-0.08f, 0.3f, 0.11f)) *
		glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ribbonTransform1));
	glUniform4f(colorLoc, 1, 0.208, 0.471, 1.0f);
	glUniform1f(boolcolorLoc, 1.f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	mat4 ribbonTransform2 = penguinTransform *
		glm::translate(glm::vec3(-0.18f, 0.2, 0.11f)) *
		glm::rotate(180.0f, glm::vec3(0, 0, 1)) *
		glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(ribbonTransform2));
	glUniform4f(colorLoc, 1, 0.208, 0.471, 1.0f);
	glUniform1f(boolcolorLoc, 1.f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	mat4 RightwingAngle = glm::rotate((sin(theta * 3.0f) + 1) * 15.0f, glm::vec3(0, 0, 1));
	mat4 LeftwingAngle = glm::rotate(-(sin(theta * 3.0f) + 1) * 15.0f, glm::vec3(0, 0, 1));
	//std::cout << "wingAngle: " << wingAngle << std::endl;

	// Left Wing
	mat4 leftWingTransform =
		penguinTransform *
		glm::translate(glm::vec3(-.3f, -0.1f, 0)) *
		LeftwingAngle *
		glm::scale(glm::vec3(0.1f, 0.6f, 0.4f)) *
		glm::translate(glm::vec3(0, -0.5f, 0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(leftWingTransform));
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.f); // Black
	glUniform1f(boolcolorLoc, 1.f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Right Wing
	mat4 rightWingTransform =
		penguinTransform *
		glm::translate(glm::vec3(.3f, -0.1f, 0)) *
		RightwingAngle *
		glm::scale(glm::vec3(0.1f, 0.6f, 0.4f)) *
		glm::translate(glm::vec3(0, -0.5f, 0));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(rightWingTransform));
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);


	// Left Leg
	mat4 leftLegTransform = penguinTransform *
		glm::translate(glm::vec3(-0.2f, -1.1f, 0)) *
		glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(leftLegTransform));
	glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.f); // Orange
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Right Leg
	mat4 rightLegTransform = penguinTransform *
		glm::translate(glm::vec3(0.2f, -1.1f, 0)) *
		glm::scale(glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(rightLegTransform));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Beak
	mat4 beakTransform = penguinTransform *
		glm::translate(glm::vec3(0.0f, 0.1f, 0.2f)) *
		glm::scale(glm::vec3(0.1f, 0.05f, 0.2f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(beakTransform));
	glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Left Eye
	mat4 leftEyeTransform = penguinTransform *
		glm::translate(glm::vec3(-0.1f, 0.15f, 0.15f)) *
		glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(leftEyeTransform));
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.f);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// Right Eye
	mat4 rightEyeTransform = penguinTransform *
		glm::translate(glm::vec3(0.1f, 0.15f, 0.15f)) *
		glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(rightEyeTransform));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);



	glm::mat4 viewMat = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Patrokaaaa!", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				if (event.key.code == sf::Keyboard::W)
				{
					//y
					cameraPosition -= 0.5f * cameraUp;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					//y
					cameraPosition += 0.5f * cameraUp;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					//x
					cameraPosition += 0.5f * (glm::cross(cameraDirection, cameraUp));
				}
				if (event.key.code == sf::Keyboard::A)
				{
					//x
					cameraPosition -= 0.5f * (glm::cross(cameraDirection, cameraUp));
				}
				if (event.key.code == sf::Keyboard::Q)
				{
					//z
					cameraPosition += 0.3f * cameraDirection;
				}
				if (event.key.code == sf::Keyboard::E)
				{
					//z
					cameraPosition -= 0.3f * cameraDirection;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					vec4 cdirection = glm::rotate(-10.0f, cameraUp) * vec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
					cameraDirection = vec3(cdirection.x, cdirection.y, cdirection.z);
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					vec4 cdirection = glm::rotate(10.0f, cameraUp) * vec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
					cameraDirection = vec3(cdirection.x, cdirection.y, cdirection.z);
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					vec4 cdirection = glm::rotate(10.0f, glm::cross(cameraDirection, cameraUp)) * vec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
					cameraDirection = vec3(cdirection.x, cdirection.y, cdirection.z);
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					vec4 cdirection = glm::rotate(-10.0f, glm::cross(cameraDirection, cameraUp)) * vec4(cameraDirection.x, cameraDirection.y, cameraDirection.z, 1);
					cameraDirection = vec3(cdirection.x, cdirection.y, cdirection.z);
				}
				break;
			}
			}
			cameraUp = (glm::cross(cameraDirection, glm::cross(cameraUp, cameraDirection)));
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}