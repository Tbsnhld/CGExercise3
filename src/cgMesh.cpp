/** \file cgMesh.cpp
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#include <iostream>

#include <glm/glm.hpp>

#include "utils.h"
#include "cgMesh.h"

cgMesh create_plane()
{
	// create four vertices
	std::vector<glm::vec3> vertices = {
		    { -0.5, 0.0, -0.5 }, { 0.5, 0.0, -0.5 }, { 0.5,  0.0, 0.5 }, { -0.5,  0.0, 0.5 }};
	// create normals of the four vertices
	std::vector<glm::vec3> normals = {
			{ 0.0, 1.0, 0.0}, { 0.0, 1.0, 0.0}, { 0.0, 1.0, 0.0}, { 0.0, 1.0, 0.0}
		};
	// create indices for two triangles
	std::vector<unsigned int> indices = {0, 1, 2,
		     	 	 	 	 	 	 	 2, 3, 0};
	// assign arrays to mesh struct
	cgMesh plane_output;
	plane_output.vertices = vertices;
	plane_output.normals = normals;
	plane_output.indices = indices;

	return plane_output;
}

cgMesh create_cube()
{
	// define an array with vertices of the cube
	std::vector<glm::vec3> vertices = {
		    {-0.5f, -0.5f, -0.5f},    { 0.5f, -0.5f, -0.5f},    { 0.5f,  0.5f, -0.5f},    {-0.5f,  0.5f, -0.5f},  // back side
		    {-0.5f, -0.5f,  0.5f},    { 0.5f, -0.5f,  0.5f},    { 0.5f,  0.5f,  0.5f},    {-0.5f,  0.5f,  0.5f},  // front side
		    {-0.5f, -0.5f, -0.5f},    {-0.5f, -0.5f,  0.5f},    {-0.5f,  0.5f,  0.5f},    {-0.5f,  0.5f, -0.5f},  // left side
		    { 0.5f, -0.5f,  0.5f},    { 0.5f, -0.5f, -0.5f},    { 0.5f,  0.5f, -0.5f},    { 0.5f,  0.5f,  0.5f},  // right side
		    {-0.5f, -0.5f,  0.5f},    { 0.5f, -0.5f,  0.5f},    { 0.5f, -0.5f, -0.5f},    {-0.5f, -0.5f, -0.5f},  // bottom side
		    {-0.5f,  0.5f, -0.5f},    { 0.5f,  0.5f, -0.5f},    { 0.5f,  0.5f,  0.5f},    {-0.5f,  0.5f,  0.5f}  // top side
		};
	// define an array with normals (for each vertex) of the cube
	std::vector<glm::vec3> normals = {
		    { 0.0f,  0.0f, -1.0f},  { 0.0f,  0.0f, -1.0f},   { 0.0f,  0.0f, -1.0f},   { 0.0f,  0.0f, -1.0f},  // back side
		    { 0.0f,  0.0f,  1.0f},  { 0.0f,  0.0f,  1.0f},   { 0.0f,  0.0f,  1.0f},   { 0.0f,  0.0f,  1.0f},  // front side
		    {-1.0f,  0.0f,  0.0f},  {-1.0f,  0.0f,  0.0f},   {-1.0f,  0.0f,  0.0f},   {-1.0f,  0.0f,  0.0f},  // left side
			{ 1.0f,  0.0f,  0.0f},  { 1.0f,  0.0f,  0.0f},   { 1.0f,  0.0f,  0.0f},   { 1.0f,  0.0f,  0.0f},  // right side
			{ 0.0f, -1.0f,  0.0f},  { 0.0f, -1.0f,  0.0f},   { 0.0f, -1.0f,  0.0f},   { 0.0f, -1.0f,  0.0f},  // bottom side
			{ 0.0f,  1.0f,  0.0f},  { 0.0f,  1.0f,  0.0f},   { 0.0f,  1.0f,  0.0f},   { 0.0f,  1.0f,  0.0f}   // top side
		};
	// create indices for two triangles per side
	std::vector<unsigned int> indices = {
	        0, 1, 2,        2, 3, 0,    // back side
	        4, 5, 6,        6, 7, 4,    // front side
	        8, 9, 10,       10, 11, 8,  // left side
	        12, 13, 14,     14, 15, 12, // right side
	        16, 17, 18,     18, 19, 16, // bottom side
	        20, 21, 22,     22, 23, 20  // top side
	};
	// assign arrays to mesh struct
	cgMesh mesh_output;
	mesh_output.vertices = vertices;
	mesh_output.normals = normals;
	mesh_output.indices = indices;

	return mesh_output;
};

cgMesh create_teapot(std::string filename)
{
	std::vector<glm::vec3> vertices;
	// define an array with normals (for each vertex) of the cube
	std::vector<glm::vec3> normals;
	// create indices for two triangles per side
	std::vector<unsigned int> indices;

	// load the teapot from the obj-file
	if(!loadTeapotOBJ(filename, vertices, normals, indices))
	{
		std::cerr<<"ERROR: Can not Load teapot object from file: "<<filename<<"!\n";
		return cgMesh(); // returns empty geometry
	}

	// assign arrays to mesh struct
	cgMesh plane_output;
	plane_output.vertices = vertices;
	plane_output.normals = normals;
	plane_output.indices = indices;

	return plane_output;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation of Class cgMesh
//
///////////////////////////////////////////////////////////////////////////////////////////////

cgMesh::~cgMesh()
{
	// free the OpenGL buffers
	deleteOpenGLBuffers();
	// note that STL std::vector arrays are deleted automatically if lifetime
	// of the class ends
}

bool cgMesh::initOpenGLBuffers()
{
	if(vertices.empty())
	{
		std::cerr<<"ERROR: No mesh to initialize!\n";
		return false;
	}
	if(vao_id > 0)
	{
		std::cerr<<"ERROR: Geometry is already initialized!\n";
		return false;
	}

	// Tell OpenGL to allocate us some space for the VAO
	glGenVertexArrays(1, &vao_id);
	// Now allocate some space for all the buffer objects
	buffer_object_ids.resize(4, 0); // buffer for vertices, normals, indices, texture_coords with 0 initialized
	glGenBuffers(4, &buffer_object_ids[0]); // return head of array

	// now bind the VAO all following operations are related to this VAO
	glBindVertexArray(vao_id);

	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object_ids[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	if(! normals.empty())
	{
		assert(normals.size() == vertices.size());
		// bind normal data to OpenGL buffer
		glBindBuffer(GL_ARRAY_BUFFER, buffer_object_ids[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
		// Bind normals to layout location 1
		glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	}
	if(! indices.empty())
	{
		// The indices array tells OpenGL what order to iterate through the buffers in when the shaders execute
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_object_ids[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	}
	if(! texture_coords.empty())
	{
		assert(texture_coords.size() == vertices.size());
		// bind texture coords data to OpenGL buffer
		glBindBuffer(GL_ARRAY_BUFFER, buffer_object_ids[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texture_coords.size(), &texture_coords[0],GL_STATIC_DRAW);
		// Use layout location 2 for texcoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	}

	// unbind last buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind VAO so that it is not accidenty changed outside
	glBindVertexArray(0);

	return true;
}

bool cgMesh::drawSolid()
{
	if(vao_id == 0)
	{
		std::cerr<<"ERROR: Mesh is not initialized!"<<std::endl;
		return false;
	}

	// bind the VAO of this mesh
	glBindVertexArray(vao_id);

	if(indices.empty())
	{
		// alse draw triangle vertices directly
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
	else
	{
		// if an index buffer is used in this mesh, draw this
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	// unbind VAO so that it is not accidently changed outside
	glBindVertexArray(0);

	return true;
}

void cgMesh::deleteOpenGLBuffers()
{
	if(buffer_object_ids.size() > 0)
	{
		glDeleteBuffers(buffer_object_ids.size(), &buffer_object_ids[0]);
		buffer_object_ids.clear();
	}
	if(vao_id>0)
	{
		glDeleteVertexArrays(1, &vao_id);
		vao_id = 0;
	}
}

