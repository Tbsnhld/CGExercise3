/** \file cgMesh.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef _CGMESH_H_
#define _CGMESH_H_

#include <vector>
#include <string>

// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>      // GLEW provids OpenGL headers in
						  // a platform independent way
#endif

#include <glm/glm.hpp>

// forward declaration of the class
class cgMesh;

// Functions to create different meshes
cgMesh create_cube();
cgMesh create_plane();
cgMesh create_teapot(std::string filename);


/*! \brief This base class describes a geometric object composed by triangles.
 *
 * The base class assembles the triangle vertices, normals and textures in an
 * VAO and transfer it to OpenGL in the initOpenGLBuffers() routine.
 * The drawSolid() routine renders the VAO in OpenGL.
 *
 * Define vertices, normals etc. in the constructor of a derived class and call
 * init() and drawSolid() of the base class.
 */
class cgMesh {
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texture_coords;
	std::vector<unsigned int> indices;

	cgMesh() = default; // constructor (use default values)
	~cgMesh();          // destructor

	bool initOpenGLBuffers();  // send arrays to VRAM
	bool drawSolid();          // draw mesh with OpenGL

	void deleteOpenGLBuffers(); // delete arrays in VRAM

protected:
	// OpenGL buffer IDs
	GLuint vao_id = 0;  // ID of the VAO for this object
	std::vector<GLuint> buffer_object_ids;  // array with IDs
											// of OpenGL buffers
};

#endif // CGMESH_H_
