#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <sb7ktx.h>
#include <shader.h>
#include <assert.h>
#include <cmath>

#define PI 3.14159265

class assignment1_app : public sb7::application
{

#pragma region protected
protected:
    void init()
    {
        static const char title[] = "Assignment 1";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
		info.windowWidth = 512;
		info.windowHeight = 512;
    }

    void startup();
    void render(double currentTime);

	//Listener
    void onKey(int key, int action);
	void onMouseMove(int x, int y);
	void onMouseButton(int button, int action);
	//_________________________________________

	vmath::vec3 getArcballVector(int x, int y);

	GLuint			color_vertex_cube_program;

	//Where uniforms are defined
    struct uniforms_block
    {
        vmath::mat4     mv_matrix;
        vmath::mat4     view_matrix;
        vmath::mat4     proj_matrix;
    };

    GLuint          uniforms_buffer;


	// Variables for mouse interaction
    bool bPerVertex;
	bool bShiftPressed = false;
	bool bZoom = false;
	bool bRotate = false;
	bool bPan = false;

	int iWidth = info.windowWidth;
	int iHeight = info.windowHeight;

	// Rotation and Translation matricies for moving the camera by mouse interaction.
	vmath::mat4 rotationMatrix = vmath::mat4::identity();
	vmath::mat4 translationMatrix = vmath::mat4::identity();

    #pragma region Colors
    const GLfloat zeros[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat gray[4] = { 0.1f, 0.1f, 0.1f, 0.0f };
	const GLfloat green[4] = { 0.0f, 0.25f, 0.0f, 1.0f };
	const GLfloat skyBlue[4] = { 0.529f, 0.808f, 0.922f, 1.0f };
	const GLfloat ones[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
    #pragma endregion

#pragma region Vertex Data
    static const int numberOfVertices = 66;
	static const int numberOfVerticeComponents = numberOfVertices * 4;
    const GLfloat vertex_data[numberOfVerticeComponents] =
	{
		// Cube #1
		//B
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,

		//R
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,

		//F
		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		//L
		-1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,

		//D
		-1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f,

		//U
		-1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		//End Cube

		// Room
		//B
		-3.0f, 3.0f, -3.0f, 1.0f,
		-3.0f, -3.0f, -3.0f, 1.0f,
		3.0f, -3.0f, -3.0f, 1.0f,

		3.0f, -3.0f, -3.0f, 1.0f,
		3.0f, 3.0f, -3.0f, 1.0f,
		-3.0f, 3.0f, -3.0f, 1.0f,

		//R
		3.0f, -3.0f, -3.0f, 1.0f,
		3.0f, -3.0f, 3.0f, 1.0f,
		3.0f, 3.0f, -3.0f, 1.0f,

		3.0f, -3.0f, 3.0f, 1.0f,
		3.0f, 3.0f, 3.0f, 1.0f,
		3.0f, 3.0f, -3.0f, 1.0f,

		//L
		-3.0f, -3.0f, 3.0f, 1.0f,
		-3.0f, -3.0f, -3.0f, 1.0f,
		-3.0f, 3.0f, 3.0f, 1.0f,

		-3.0f, -3.0f, -3.0f, 1.0f,
		-3.0f, 3.0f, -3.0f, 1.0f,
		-3.0f, 3.0f, 3.0f, 1.0f,

		//D
		-3.0f, -3.0f, 3.0f, 1.0f,
		3.0f, -3.0f, 3.0f, 1.0f,
		3.0f, -3.0f, -3.0f, 1.0f,

		3.0f, -3.0f, -3.0f, 1.0f,
		-3.0f, -3.0f, -3.0f, 1.0f,
		-3.0f, -3.0f, 3.0f, 1.0f,

		//U
		-3.0f, 3.0f, -3.0f, 1.0f,
		3.0f, 3.0f, -3.0f, 1.0f,
		3.0f, 3.0f, 3.0f, 1.0f,

		3.0f, 3.0f, 3.0f, 1.0f,
		-3.0f, 3.0f, 3.0f, 1.0f,
		-3.0f, 3.0f, -3.0f, 1.0f,
		//End Room

	};
#pragma endregion

#pragma endregion

#pragma region private
private:
	// Variables for mouse position to solve the arcball vectors
	int iPrevMouseX = 0;
	int iPrevMouseY = 0;
	int iCurMouseX = 0;
	int iCurMouseY = 0;

	// Scale of the objects in the scene
	float fScale = 7.0f;

	// Initial position of the camera
	float fXpos = 0.0f;
	float fYpos = 0.0f;
	float fZpos = 75.0f;

	GLuint buffer;
	GLuint vao2;
#pragma endregion
};

void assignment1_app::startup()
{

	// Create program for the spinning cube
	color_vertex_cube_program = glCreateProgram(); //glCreateProgram creates an empty program object and returns a non-zero value by which it can be referenced. A program object is an object to which shader objects can be attached.

    #pragma region Vertex Shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	static const char * vs_source[] =
	{
		"#version 410 core                                                  \n"
		"                                                                   \n"
		"layout (location = 0) in vec4 position;                            \n"
		"																	\n"
		"layout(std140) uniform constants									\n"
		"{																	\n"
		"	mat4 mv_matrix;													\n"
		"	mat4 view_matrix;												\n"
		"	mat4 proj_matrix;												\n"
		"};																	\n"
		"                                                                   \n"
		"out VS_OUT                                                         \n"
		"{                                                                  \n"
		"    vec4 color;                                                    \n"
		"} vs_out;                                                          \n"
		"                                                                   \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    // Calculate view-space coordinate								\n"
		"    vec4 P = mv_matrix * position;									\n"
		"    vs_out.color = (position+1.5)/2.5;							    \n"
		"    gl_Position = proj_matrix * P;								    \n"
		"}                                                                  \n"
	};
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);
	GLint success = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	assert(success != GL_FALSE);
    #pragma endregion

    #pragma region Fragment Shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	static const char * fs_source[] =
	{
		"#version 410 core                                                  \n"
		"                                                                   \n"
		"out vec4 color;                                                    \n"
		"                                                                   \n"
		"in VS_OUT                                                          \n"
		"{                                                                  \n"
		"    vec4 color;                                                    \n"
		"} fs_in;                                                           \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    color = fs_in.color;                                           \n"
		"}                                                                  \n"
	};
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);
    success = 0;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	assert(success != GL_FALSE);
    #pragma endregion

    #pragma region Attach Shaders To Program
	glAttachShader(color_vertex_cube_program, vs);
	glAttachShader(color_vertex_cube_program, fs);
    #pragma endregion

    #pragma region Link And Use Program
	glLinkProgram(color_vertex_cube_program); //glLinkProgram links the program object specified by program.
	success = 0;
	glGetProgramiv(color_vertex_cube_program, GL_LINK_STATUS, &success); //glGetProgramiv returns in params the value of a parameter for a specific program object.
	assert(success != GL_FALSE);
	glUseProgram(color_vertex_cube_program); // installs the program object specified by program as part of current rendering state.
    #pragma endregion

	glGenVertexArrays(1, &vao2);  //glGenVertexArrays(n, &array) returns n vertex array object names in arrays
	glBindVertexArray(vao2); //glBindVertexArray(array) binds the vertex array object with name array.

    #pragma region Buffer
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertex_data),
		vertex_data,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
   #pragma endregion
	
    #pragma region Buffer For Uniform Block
	glGenBuffers(1, &uniforms_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);
    #pragma endregion

    #pragma region OPENGL Settings
	glFrontFace(GL_CW); //glFrontFace(GLenum mode) In a scene composed entirely of opaque closed surfaces, back-facing polygons are never visible.
    glEnable(GL_DEPTH_TEST); //glEnable(GLenum cap) glEnable and glDisable enable and disable various capabilities.
    glDepthFunc(GL_LEQUAL);	//glDepthFunc(GLenum func) specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer. 
    #pragma endregion
}

void assignment1_app::render(double currentTime)
{
    const float f = (float)currentTime;
	
    #pragma region Calculations for mouse interaction camera rotation and translation matrix
	float fAngle = 0.0f;
	vmath::vec3 axis_in_camera_coord = (0.0f, 1.0f, 0.0f);	
	if (iCurMouseX != iPrevMouseX || iCurMouseY != iPrevMouseY) {
		// Arcball Rotation
		if (bRotate){
			vmath::vec3 va = getArcballVector(iPrevMouseX, iPrevMouseY);
			vmath::vec3 vb = getArcballVector(iCurMouseX, iCurMouseY);
			fAngle = acos(fmin(1.0f, vmath::dot(va, vb)));
			axis_in_camera_coord = vmath::cross(va, vb);
			axis_in_camera_coord = vmath::normalize(axis_in_camera_coord);
			iPrevMouseX = iCurMouseX;
			iPrevMouseY = iCurMouseY;
			rotationMatrix *= vmath::rotate(vmath::degrees(fAngle), axis_in_camera_coord);
		}
		// Zoom in and out
		if (bZoom) {
			fZpos += (iCurMouseY - iPrevMouseY);
			if (fZpos > 500)
			{
				fZpos = 500;
			}
			else if (fZpos < 10)
			{
				fZpos = 10;
			}
			iPrevMouseY = iCurMouseY;
			iPrevMouseX = iCurMouseX;
		}
		// Pan camera left, right, up, and down
		if (bPan) {
			fXpos += (iCurMouseX - iPrevMouseX);
			fYpos += (iCurMouseY - iPrevMouseY);
			iPrevMouseY = iCurMouseY;
			iPrevMouseX = iCurMouseX;
			translationMatrix = vmath::translate(fXpos / (info.windowWidth / fZpos), -fYpos/(info.windowWidth / fZpos), 0.0f);
		}
	}
    #pragma endregion

    glViewport(0, 0, info.windowWidth, info.windowHeight);

	// Create sky blue background
    glClearBufferfv(GL_COLOR, 0, skyBlue);
    glClearBufferfv(GL_DEPTH, 0, ones);

	// Set up view and perspective matrix
	vmath::vec3 view_position = vmath::vec3(0.0f, 0.0f, fZpos);
	vmath::mat4 view_matrix = vmath::lookat(view_position,
		vmath::vec3(0.0f, 0.0f, 0.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f));
	view_matrix *= translationMatrix;
	view_matrix *= rotationMatrix;

	vmath::mat4 perspective_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);

	// Render cube
	
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);
	uniforms_block * block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	vmath::mat4 model_matrix =
		vmath::rotate((float)currentTime * 14.5f, 0.0f, 1.0f, 0.0f) *
		vmath::rotate(45.0f, 0.0f, 1.0f, 0.0f)*
		vmath::scale(fScale);

	block->mv_matrix = view_matrix * model_matrix;
	block->view_matrix = view_matrix;
	block->proj_matrix = perspective_matrix;

	glUnmapBuffer(GL_UNIFORM_BUFFER); //release the mapping of a buffer object's data store into the client's address space

	glBindBuffer(GL_ARRAY_BUFFER, buffer); 
	glEnableVertexAttribArray(0); //enable or disable a generic vertex attribute array
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices); //void glDrawArrays(GLenum mode, GLint first, GLsizei count); specifies multiple geometric primitives with very few subroutine calls.
}

GLfloat ReturnARandomColor() 
{
	return (GLfloat) 0.0f;
}

#pragma region Event Handlers
void assignment1_app::onKey(int key, int action)
{
	// Check to see if shift was pressed
	if (action == GLFW_PRESS && (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT))
	{
		bShiftPressed = true;
	}
	if (action)
	{
        switch (key)
        {
			
            case 'R': 
				rotationMatrix = vmath::mat4::identity();
				translationMatrix = vmath::mat4::identity();
				fXpos = 0.0f;
				fYpos = 0.0f;
				fZpos = 75.0f;
                break;
			case 'V':

				//TODO: The �v� key should change between these shading modes (switching shader programs).
				break;
		}
    }
	// Check to see if shift was released
	if (action == GLFW_RELEASE) {
		bShiftPressed = false;
	}
}

void assignment1_app::onMouseButton(int button, int action)
{
	int x, y;
	
	getMousePosition(x, y);
	// Check to see if left mouse button was pressed for rotation
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		bRotate = true;
		iPrevMouseX = iCurMouseX = x;
		iPrevMouseY = iCurMouseY = y;
	}
	// Check to see if right mouse button was pressed for zoom and pan
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		bZoom = false;
		bPan = false;
		if (bShiftPressed == true)
		{
			bZoom = true;
		}
		else if (bShiftPressed == false)
		{
			bPan = true;
		}
		iPrevMouseX = iCurMouseX = x;
		iPrevMouseY = iCurMouseY = y;
	}
	else {
		bRotate = false;
		bZoom = false;
		bPan = false;
	}
	
}

void assignment1_app::onMouseMove(int x, int y)
{
	// If rotating, zooming, or panning save mouse x and y
	if (bRotate || bZoom || bPan) 
	{
		iCurMouseX = x;
		iCurMouseY = y;
	}
}

// Modified from tutorial at the following website:
// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball

vmath::vec3 assignment1_app::getArcballVector(int x, int y) {
	// find vector from origin to point on sphere
	vmath::vec3 vecP = vmath::vec3(1.0f*x / info.windowWidth * 2 - 1.0f, 1.0f*y / info.windowHeight * 2 - 1.0f, 0.0f);
	// inverse y due to difference in origin location on the screen
	vecP[1] = -vecP[1];
	float vecPsquared = vecP[0] * vecP[0] + vecP[1] * vecP[1];
	// solve for vector z component
	if (vecPsquared <= 1)
		vecP[2] = sqrt(1-vecPsquared);		
	else
		vecP = vmath::normalize(vecP);
	return vecP;
}
#pragma endregion

DECLARE_MAIN(assignment1_app)
