//------------------------------------------------------------------------------------------
//
//
// Created on: 1/21/2015
//     Author: Nghia Truong
//
//------------------------------------------------------------------------------------------

#include <sb7.h>
#include <vmath.h>
#include <object.h>
#include <sb7ktx.h>
#include <shader.h>
#include <assert.h>
#include <cmath>
#include <cmath>

#ifndef UVSPHERE_H
#define UVSPHERE_H


class UnitSphere
{
public:
    UnitSphere();
    ~UnitSphere();

    void generateSphere(int _numStacks, int _numSlices);

    int getNumVertices();
    int getNumIndices();
    int getVertexOffset();
    int getTexCoordOffset();
    int getIndexOffset();

    GLfloat* getVertices();
    GLfloat* getNormals();
    GLfloat* getNegativeNormals();
    GLfloat* getTexureCoordinates();
    GLushort* getIndices();

    int numStacks;
    int numSlices;

private:
    void clearData();

    list<vmath::vec3> verticesList;
    QList<vmath::vec3> texCoordList;
    QList<vmath::vec3> normalsList;
    QVector<GLushort> indicesList;
    GLfloat* vertices;
    GLfloat* texCoord;
    GLfloat* normals;

};

#endif // UVSPHERE_H
