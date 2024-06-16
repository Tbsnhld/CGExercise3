/** \file utils.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>

// this file implements some simple helper functions that are
// used in this project

// loads the teapot.obj file
// NOTE: this is not a fully functional OBJ-reader
bool loadTeapotOBJ(std::string filename,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec3> & out_normals,
		std::vector<unsigned int> & out_indices);

// Save the current window as screenshot in file
void saveScreenshot(std::string filename);
// looks for current OpenGL version
void checkOpenGLVersion();

//
// some functions helpful for debugging, e.g. prints matrices and vectors
//
void printMatrix(glm::mat3 matrix);
void printMatrix(glm::mat4 matrix);
void printVector(glm::vec2 vector);
void printVector(glm::vec3 vector);
void printVector(glm::vec4 vector);

