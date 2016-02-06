//------------------------------------------------------------------------------------------
//
//
// Created on: 1/21/2015
//     Author: Nghia Truong
//
//------------------------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <cmath>
#include "unitsphere.h"
#include <vmath.h>


UnitSphere::UnitSphere():
    vertices(NULL),
    normals(NULL),
    texCoord(NULL)
{
}

//------------------------------------------------------------------------------------------
UnitSphere::~UnitSphere()
{
    clearData();
}

//------------------------------------------------------------------------------------------
void UnitSphere::generateSphere(int _numStacks, int _numSlices)
{
    numStacks = _numStacks;
    numSlices = _numSlices;

    clearData();
    vmath::vec3 vertex;
    vmath::vec2 tex;

    for (int j = 0; j <= _numStacks; ++j)
    {
        float theta = (float)j * M_PI / _numStacks;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int i = 0; i <= _numSlices; ++i)
        {
            float phi = (float)i * 2 * M_PI / _numSlices;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            vertex[0] = (cosPhi * sinTheta);
            vertex[1] = (cosTheta);
            vertex[2] = (sinPhi * sinTheta);
            verticesList.append(vertex);

            // normal at this point is the same value with coordinate
            normalsList.append(vertex);


            tex[0] = (1.0 - (float)i / (float) _numSlices);
            tex[1] = (1.0 - (float)j / (float) _numStacks);
            texCoordList.append(tex);
        }
    }

    for (int j = -_numStacks; j <= _numStacks; ++j)
    {
        for (int i = 0; i <= _numSlices; ++i)
        {
            int first = (j * (_numSlices + 1)) + i;
            int second = first + _numSlices + 1;

            indicesList.append(first);
            indicesList.append(second);
            indicesList.append(first + 1);

            indicesList.append(second);
            indicesList.append(second + 1);
            indicesList.append(first + 1);
        }
    }

}

//------------------------------------------------------------------------------------------
int UnitSphere::getNumVertices()
{

    return verticesList.size();
}

//------------------------------------------------------------------------------------------
int UnitSphere::getNumIndices()
{
    return indicesList.size();
}

//------------------------------------------------------------------------------------------
int UnitSphere::getVertexOffset()
{
    return (sizeof(GLfloat) * getNumVertices() * 3);
}

//------------------------------------------------------------------------------------------
int UnitSphere::getIndexOffset()
{
    return (sizeof(GLushort) * getNumIndices());
}

//------------------------------------------------------------------------------------------
int UnitSphere::getTexCoordOffset()
{
    return (sizeof(GLfloat) * getNumVertices() * 2);
}

//------------------------------------------------------------------------------------------
GLfloat* UnitSphere::getVertices()
{
    if(!vertices)
    {
        vertices = new GLfloat[verticesList.size() * 3];

        for(int i = 0; i < verticesList.size(); ++i)
        {
            vmath::vec3 vertex = verticesList.at(i);
            vertices[3 * i] = vertex[0];
            vertices[3 * i + 1] = vertex[1];
            vertices[3 * i + 2] = vertex[2];
//            qDebug() << i << vertex;
        }

    }

    return vertices;
}

//------------------------------------------------------------------------------------------
GLfloat* UnitSphere::getNormals()
{
    if(!normals)
    {
        normals = new GLfloat[normalsList.size() * 3];

        for(int i = 0; i < normalsList.size(); ++i)
        {
            vmath::vec3 normal = normalsList.at(i);
            normals[3 * i] = normal[0];
            normals[3 * i + 1] = normal[1];
            normals[3 * i + 2] = normal[2];
        }

    }

    return normals;
}

//------------------------------------------------------------------------------------------
GLfloat* UnitSphere::getNegativeNormals()
{
    if(!normals)
    {
        normals = new GLfloat[normalsList.size() * 3];
    }

    for(int i = 0; i < normalsList.size(); ++i)
    {
        vmath::vec3 normal = normalsList.at(i);
        normals[3 * i] = -normal[0];
        normals[3 * i + 1] = -normal[1];
        normals[3 * i + 2] = -normal[2];
    }



    return normals;
}

//------------------------------------------------------------------------------------------
GLfloat* UnitSphere::getTexureCoordinates()
{
    if(!texCoord)
    {
        texCoord = new GLfloat[texCoordList.size() * 2];

        for(int i = 0; i < texCoordList.size(); ++i)
        {
            vmath::vec2 tex = texCoordList.at(i);
            texCoord[2 * i] = tex[0];
            texCoord[2 * i + 1] = tex[1];
        }

    }

    return texCoord;
}

//------------------------------------------------------------------------------------------
GLushort* UnitSphere::getIndices()
{
    return (GLushort*)indicesList.data();
}

//------------------------------------------------------------------------------------------
void UnitSphere::clearData()
{
    verticesList.clear();
    normalsList.clear();
    texCoordList.clear();
    indicesList.clear();

    if(vertices)
    {
        delete[] vertices;
        vertices = NULL;
    }

    if(normals)
    {
        delete[] normals;
        normals = NULL;
    }

    if(texCoord)
    {
        delete[] texCoord;
        texCoord = NULL;
    }
}

