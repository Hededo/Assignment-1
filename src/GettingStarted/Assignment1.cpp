#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <sb7ktx.h>
#include <shader.h>
#include <assert.h>
#include <cmath>

#include <vector>

#define PI 3.14159265
#define SPHERE_DATA_ID 0
#define SPHERE_COLORS_ID 1
#define SPHERE_NORMALS_ID 2

class assignment1_app : public sb7::application
{

public:
	assignment1_app()
		: per_fragment_program(0),
		per_vertex_program(0),
		per_vertex(false)
	{
	}

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
	void triangle(vmath::vec4 a, vmath::vec4 b, vmath::vec4 c);
	void insertVec4IntoBuffer(vmath::vec4 toInsert, int bufferId);
	void divideTriangle(vmath::vec4 a, vmath::vec4 b, vmath::vec4 c, int count);
	void tetrahedron(vmath::vec4, vmath::vec4 b, vmath::vec4, vmath::vec4 d, int n);

	//Listener
    void onKey(int key, int action);
	void onMouseMove(int x, int y);
	void onMouseButton(int button, int action);
	//_________________________________________

	vmath::vec3 getArcballVector(int x, int y);

	void load_shaders();

	GLuint          per_fragment_program;
	GLuint          per_vertex_program;

	struct
	{
		GLuint      color;
		GLuint      normals;
	} textures;

	//Where uniforms are defined
    struct uniforms_block
    {
        vmath::mat4     mv_matrix;
        vmath::mat4     view_matrix;
        vmath::mat4     proj_matrix;
		vmath::vec4     uni_color;
		vmath::vec4     lightPos;
		vmath::vec4	    useUniformColor;
		vmath::vec4	    invertNormals;
    };

    GLuint          uniforms_buffer;

	bool            per_vertex;
	vmath::vec4     useUniformColor;


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
	const GLfloat skyBlue[4] = { 0.529f, 0.808f, 0.922f, 1.0f };
	const GLfloat ones[4] = { 1.0f, 1.0f, 1.0f, 1.0f};
	const vmath::vec4 white = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const vmath::vec4 orange = vmath::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	const vmath::vec4 purple = vmath::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	const vmath::vec4 gray = vmath::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	const vmath::vec4 falseVec = vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	const vmath::vec4 trueVec = vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    #pragma endregion

#pragma region Vertex Data
    static const int numberOfVertices = 36;
	static const int numberOfVerticeComponents = numberOfVertices * 4;
    const GLfloat cube_data[numberOfVerticeComponents] = {
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
	};

	const GLfloat color_data[numberOfVerticeComponents] = { 
		//B (Green)
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,

		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,

		//R (Red)
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		//F (Green)
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,

		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,
		0.0f,  1.0f,  0.0f, 1.0f,

		//L (Red)
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		//D (Blue)
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		//U
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		//End Cube
	};

	const GLfloat normals_data[numberOfVerticeComponents] = {
		//B (Green)
		0.0f,  0.0f,  -1.0f, 1.0f,
		0.0f,  0.0f,  -1.0f, 1.0f,
		0.0f,  0.0f,  -1.0f, 1.0f,

		0.0f,  0.0f,  -1.0f, 1.0f,
		0.0f,  0.0f,  -1.0f, 1.0f,
		0.0f,  0.0f,  -1.0f, 1.0f,

		//R (Red)
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  0.0f,  1.0f,

		//F (Green)
		0.0f,  0.0f,  1.0f, 1.0f,
		0.0f,  0.0f,  1.0f, 1.0f,
		0.0f,  0.0f,  1.0f, 1.0f,

		0.0f,  0.0f,  1.0f, 1.0f,
		0.0f,  0.0f,  1.0f, 1.0f,
		0.0f,  0.0f,  1.0f, 1.0f,

		//L (Red)
		-1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  0.0f,  0.0f,  1.0f,

		//D (Blue)
		0.0f, -1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f, 1.0f,

		0.0f, -1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		0.0f, -1.0f, 0.0f, 1.0f,

		//U
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		//End Cube
	};

#pragma endregion

	std::vector<GLfloat> sphereData = std::vector<GLfloat>();
	std::vector<GLfloat> sphereColors = std::vector<GLfloat>();
	std::vector<GLfloat> sphereNormals = std::vector<GLfloat>();
	GLuint sphereVertexCount = 0;

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

	// Initial light pos
	float iLightPosX = 1.0f;
	float iLightPosY = 0.5f;
	float iLightPosZ = 0.0f;
	vmath::vec4 lightPos = vmath::vec4(iLightPosX, iLightPosY, iLightPosZ, 1.0f);

	GLuint cubePosBuffer;
	GLuint cubeColorBuffer;
	GLuint cubeNormalsBuffer;
	GLuint cube_vao;

	GLuint sphereBufferSize = 0;
	GLuint spherePosBuffer;
	GLuint sphereColorBuffer;
	GLuint sphereNormalsBuffer;
	GLuint sphere_vao;
#pragma endregion
};

// ------------- Helper Functions -------------

vmath::vec4 randomColorVec4()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float a = 1.0f;
	return vmath::vec4(r, g, b, a);
}

// ---------------------------------------------

void assignment1_app::triangle(vmath::vec4 a, vmath::vec4 b, vmath::vec4 c) {

	//unable to cross vec4s. Solution is to make temp vec3 of a,b,c
	vmath::vec3 ta = vmath::vec3(a[0], a[1], a[2]);
	vmath::vec3 tb = vmath::vec3(b[0], b[1], b[2]);
	vmath::vec3 tc = vmath::vec3(c[0], c[1], c[2]);

	vmath::vec3 t1 = ta - tb;
	vmath::vec3 t2 = ta - tc;

	vmath::vec3 normalVec3 = vmath::normalize(vmath::cross(t1, t2));
	vmath::vec4 normal = vmath::vec4(normalVec3[0], normalVec3[1], normalVec3[2], 1.0f);
	vmath::vec4 color = randomColorVec4();

	insertVec4IntoBuffer(a, SPHERE_DATA_ID);
	insertVec4IntoBuffer(b, SPHERE_DATA_ID);
	insertVec4IntoBuffer(c, SPHERE_DATA_ID);

	insertVec4IntoBuffer(normal, SPHERE_NORMALS_ID);
	insertVec4IntoBuffer(normal, SPHERE_NORMALS_ID);
	insertVec4IntoBuffer(normal, SPHERE_NORMALS_ID);

	insertVec4IntoBuffer(color, SPHERE_COLORS_ID);
	insertVec4IntoBuffer(color, SPHERE_COLORS_ID);
	insertVec4IntoBuffer(color, SPHERE_COLORS_ID);

	sphereVertexCount += 3; // number of Vertices * x,y,z,w
}

void assignment1_app::insertVec4IntoBuffer(vmath::vec4 toInsert, int bufferId)
{

	if (toInsert == NULL)
	{
		return;
	}

	GLfloat x = toInsert[0];
	GLfloat y = toInsert[1];
	GLfloat z = toInsert[2];
	GLfloat a = 1.0f;

	switch (bufferId)
	{

	case SPHERE_DATA_ID:
		sphereData.push_back(x);
		sphereData.push_back(y);
		sphereData.push_back(z);
		sphereData.push_back(a);
		break;

	case SPHERE_COLORS_ID:
		sphereColors.push_back(x);
		sphereColors.push_back(y);
		sphereColors.push_back(z);
		sphereColors.push_back(a);
		break;

	case SPHERE_NORMALS_ID:
		sphereNormals.push_back(x);
		sphereNormals.push_back(y);
		sphereNormals.push_back(z);
		sphereNormals.push_back(a);
		break;

	default:
		break;
	}
}

/*
* divideTriangle
* recursively divides a triangle into
* smaller triangles
*
* Params:
*  a: a positon of vec4
*  b: a positon of vec4
*  c: a positon of vec4
*  count: number of subdivisons
*
* Returns:
*
*
* Modified from shadedSphere3.js
*/
void assignment1_app::divideTriangle(vmath::vec4 a, vmath::vec4 b, vmath::vec4 c, int count)
{
	if (count > 0) {

		vmath::vec4 ab = vmath::mix(a, b, 0.5);
		vmath::vec4 ac = vmath::mix(a, c, 0.5);
		vmath::vec4 bc = vmath::mix(b, c, 0.5);

		ab = vmath::normalize(ab);
		ac = vmath::normalize(ac);
		bc = vmath::normalize(bc);

		//reset the x coordinate
		ab[3] = 1.0f;
		ac[3] = 1.0f;
		bc[3] = 1.0f;

		divideTriangle(a, ab, ac, count - 1);
		divideTriangle(ab, b, bc, count - 1);
		divideTriangle(bc, c, ac, count - 1);
		divideTriangle(ab, bc, ac, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

/*
* tetrahedron
* constructs a tetrahedron from 4 points and
* a number of triangle subdivisons
*
* Params:
*  a: a positon of vec4
*  b: a positon of vec4
*  c: a positon of vec4
*  d: a positon of vec4
*  n: number of subdivisons
*
* Returns:
*
*
* Modified from shadedSphere3.js
*/
void assignment1_app::tetrahedron(vmath::vec4 a, vmath::vec4 b, vmath::vec4 c, vmath::vec4 d, int n)
{
	divideTriangle(a, b, c, n);
	divideTriangle(d, c, b, n);
	divideTriangle(a, d, b, n);
	divideTriangle(a, c, d, n);
}


void assignment1_app::startup()
{
	load_shaders();

    #pragma region Setup Cube
	glGenVertexArrays(1, &cube_vao);  //glGenVertexArrays(n, &array) returns n vertex array object names in arrays
	glBindVertexArray(cube_vao); //glBindVertexArray(array) binds the vertex array object with name array.

	glGenBuffers(1, &cubePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(cube_data),
		cube_data,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cubeColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeColorBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(color_data),
		color_data,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cubeNormalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormalsBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(normals_data),
		normals_data,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &uniforms_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);
#pragma endregion

    #pragma region Setup Sphere

	std::vector<vmath::vec4> points = {
		vmath::vec4(0.0f, 0.0f, -1.0f, 1.0f),
		vmath::vec4(0.0f, 0.942809f, 0.333333f, 1.0f),
		vmath::vec4(-0.816497f, -0.471405f, 0.333333f, 1.0f),
		vmath::vec4(0.816497f, -0.471405f, 0.333333f , 1.0f),
	};

	tetrahedron(points[0], points[1], points[2], points[3], 1);

	sphereBufferSize = sphereData.size() * 4 * sizeof(GLfloat);

	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);

	glGenBuffers(1, &spherePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, spherePosBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sphereBufferSize,
		&sphereData[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &sphereColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sphereBufferSize,
		&sphereColors[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &sphereNormalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphereNormalsBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sphereBufferSize,
		&sphereNormals[0],
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);
    #pragma endregion

	useUniformColor = falseVec;

#pragma region OPENGL Settings

	glEnable(GL_CULL_FACE); // Use face culling to see into the room.
	glFrontFace(GL_CW); //glFrontFace(GLenum mode) In a scene composed entirely of opaque closed surfaces, back-facing polygons are never visible.
	glEnable(GL_DEPTH_TEST); //glEnable(GLenum cap) glEnable and glDisable enable and disable various capabilities.
	glDepthFunc(GL_LEQUAL);	//glDepthFunc(GLenum func) specifies the function used to compare each incoming pixel depth value with the depth value present in the depth buffer. 
#pragma endregion

}

void assignment1_app::render(double currentTime)
{
    const float f = (float)currentTime;
	glUseProgram(per_vertex ? per_vertex_program : per_fragment_program);
	
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
		//Light position tracks with the camera
		lightPos = vmath::vec4(iLightPosX * translationMatrix[0][0] + iLightPosX * translationMatrix[0][1] + iLightPosX * translationMatrix[0][2],
			iLightPosY * translationMatrix[1][0] + iLightPosY * translationMatrix[1][1] + iLightPosY * translationMatrix[1][2],
			iLightPosZ * translationMatrix[2][0] + iLightPosZ * translationMatrix[2][1] + iLightPosZ * translationMatrix[2][2],
			1.0f
			);
		lightPos = vmath::vec4(lightPos[0] * rotationMatrix[0][0] + lightPos[0] * rotationMatrix[0][1] + lightPos[0] * rotationMatrix[0][2],
			lightPos[1] * rotationMatrix[1][0] + lightPos[1] * rotationMatrix[1][1] + lightPos[1] * rotationMatrix[1][2],
			lightPos[2] * rotationMatrix[2][0] + lightPos[2] * rotationMatrix[2][1] + lightPos[2] * rotationMatrix[2][2],
			1.0f
			);
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
	
	glUnmapBuffer(GL_UNIFORM_BUFFER); //release the mapping of a buffer object's data store into the client's address space
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);
	uniforms_block * block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    #pragma region Uniforms that remain constant for all geometery
	block->proj_matrix = perspective_matrix;
	block->lightPos = lightPos;
    #pragma endregion

	glBindBuffer(GL_ARRAY_BUFFER, spherePosBuffer);
	glEnableVertexAttribArray(0); //enable or disable a generic vertex attribute array
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)

	glBindBuffer(GL_ARRAY_BUFFER, sphereNormalsBuffer);
	glEnableVertexAttribArray(1); //enable or disable a generic vertex attribute array
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)

	glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer);
	glEnableVertexAttribArray(2); //enable or disable a generic vertex attribute array
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)

    #pragma region Draw Sphere
	glBindVertexArray(sphere_vao);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);

	block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT);
	vmath::mat4 model_matrix = 
		vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::scale(5.0f);
	block->uni_color = purple;
	block->mv_matrix = view_matrix * model_matrix;
	block->view_matrix = view_matrix;
	block->useUniformColor = useUniformColor;
	block->invertNormals = falseVec;

	glCullFace(GL_BACK);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertexCount);
#pragma endregion

//	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
//	glEnableVertexAttribArray(0); //enable or disable a generic vertex attribute array
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
//
//	glBindBuffer(GL_ARRAY_BUFFER, cubeNormalsBuffer);
//	glEnableVertexAttribArray(1); //enable or disable a generic vertex attribute array
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
//
//	glBindBuffer(GL_ARRAY_BUFFER, cubeColorBuffer);
//	glEnableVertexAttribArray(2); //enable or disable a generic vertex attribute array
//	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0); //define an array of generic vertex attribute data void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
//
//#pragma region Uniforms that remain constant for cubes
//	block->uni_color = orange;
//	block->useUniformColor = falseVec;
//#pragma endregion
//
//	//bind cube vertex data
//	glBindVertexArray(cube_vao);
//	glUnmapBuffer(GL_UNIFORM_BUFFER);
//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);
//	block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT);
//
//   #pragma region Draw Room
//
//	 model_matrix =
//		//vmath::rotate((float)currentTime * 14.5f, 0.0f, 1.0f, 0.0f) *
//		vmath::rotate(45.0f, 0.0f, 1.0f, 0.0f)*
//		vmath::scale(22.0f);
//
//	block->mv_matrix = view_matrix * model_matrix;
//	block->view_matrix = view_matrix;
//	block->invertNormals = falseVec;
//	
//	glCullFace(GL_FRONT);
//	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
//
//	#pragma endregion
//
//   #pragma region Draw Cube
//	glUnmapBuffer(GL_UNIFORM_BUFFER); //release the mapping of a buffer object's data store into the client's address space
//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);
//	block = (uniforms_block *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT);
//
//	model_matrix =
//		vmath::rotate(0.0f, 0.0f, 1.0f, 0.0f) *
//		vmath::translate(10.0f, -17.5f, -1.0f) *
//		vmath::scale(5.0f);
//
//	block->mv_matrix =  view_matrix * model_matrix;
//	block->view_matrix = view_matrix;
//	block->invertNormals = trueVec;
//
//	glCullFace(GL_BACK);
//	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
//    #pragma endregion
}

void assignment1_app::load_shaders()
{
	GLuint vs;
	GLuint fs;

	vs = sb7::shader::load("phong_perfragment.vs.txt", GL_VERTEX_SHADER);
	fs = sb7::shader::load("phong_perfragment.fs.txt", GL_FRAGMENT_SHADER);

	if (per_fragment_program)
	{
		glDeleteProgram(per_fragment_program);
	}
		

	per_fragment_program = glCreateProgram();
	glAttachShader(per_fragment_program, vs);
	glAttachShader(per_fragment_program, fs);
	glLinkProgram(per_fragment_program);

	vs = sb7::shader::load("phong_pervertex.vs.txt", GL_VERTEX_SHADER);
	fs = sb7::shader::load("phong_pervertex.fs.txt", GL_FRAGMENT_SHADER);

	if (per_vertex_program)
	{
		glDeleteProgram(per_vertex_program);
	}

	per_vertex_program = glCreateProgram();
	glAttachShader(per_vertex_program, vs);
	glAttachShader(per_vertex_program, fs);
	glLinkProgram(per_vertex_program);
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
				load_shaders();
				rotationMatrix = vmath::mat4::identity();
				translationMatrix = vmath::mat4::identity();
				fXpos = 0.0f;
				fYpos = 0.0f;
				fZpos = 75.0f;
                break;
			case 'V':
				per_vertex = !per_vertex;
				break;
			case 'C':
				// Provide a ‘C’ key to switch between colors in the vertex attribute to a constant color for shading the sphere.
				if (useUniformColor[0] == 1) 
				{
					useUniformColor = falseVec;
				}
				else
				{
					useUniformColor = trueVec;
				}
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
