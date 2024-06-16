/** \file utils.cpp
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
#include <iomanip>
#include <cstring>
#include <vector>

// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>      // GLEW provids OpenGL headers in
						  // a platform independent way
#endif

#include <glm/glm.hpp>
#include "glm/gtc/constants.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include <stb_image_write.h>  // this is a header library that is used
							  // to save (and load) screenshots as images

#include "utils.h"

// loads the teapot.obj file
// NOTE: this is not a fully functional OBJ-reader
bool loadTeapotOBJ(std::string filename,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec3> & out_normals,
		std::vector<unsigned int> & out_indices)
{
	std::cout<<"Loading OBJ file "<<filename<<"...\n";

	FILE* fp;
	float x, y, z;
	int fx, fy, fz, ignore;
	int c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr) {
		std::cerr << "Error loading file: " << filename << std::endl;
		return false;
	}

	while (!feof(fp)) {
		c1 = fgetc(fp);
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		int ret = 1;
		if ((c1 == 'v') && (c2 == ' ')) {
			ret = fscanf(fp, "%f %f %f", &x, &y, &z);
			out_vertices.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			ret = fscanf(fp, "%f %f %f", &x, &y, &z);
			out_normals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if ((c1 == 'f'))
		{
			ret = fscanf(fp, "%d//%d %d//%d %d//%d", &fx, &ignore, &fy, &ignore, &fz, &ignore);
			out_indices.push_back(fx - 1);
			out_indices.push_back(fy - 1);
			out_indices.push_back(fz - 1);
		}
		if(ret == EOF) {
			std::cerr<<"ERROR reading file "<<filename<<"!\n";
		}
	}

	fclose(fp); // Finished parsing

	// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.02f;
	float avgZ = (minZ + maxZ) / 2.0f;
	for (unsigned int i = 0; i < out_vertices.size(); ++i) {
		glm::vec3 shiftedVertex = out_vertices[i] - glm::vec3(0.0f, avgY, avgZ);
		out_vertices[i] = shiftedVertex;
	}

	// check if one normal per vertex
	if(out_vertices.size() != out_normals.size())
	{
		std::cerr << "Number of normals and vertices differ in file: " << filename << std::endl;
		std::cerr << "Wrong file?"<< std::endl;
		return false;
	}

	return true;
}

// Save the current window as screenshot in file
//
void saveScreenshot(std::string filename) {
	// get the size of the OpenGL Viewport
	int viewport_extent[4]; // viewport has 4 values: x/y-start & x/y-size
	glGetIntegerv(GL_VIEWPORT,&viewport_extent[0]);

	const int windowWidth = viewport_extent[2];
	const int windowHeight = viewport_extent[3];
	int pix = windowWidth * windowHeight;
	unsigned char *pixels = new unsigned char[3*pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,windowWidth,windowHeight,GL_RGB,GL_UNSIGNED_BYTE,pixels);

	std::cout << "Saving screenshot to: "<<filename<<std::endl;
	stbi_flip_vertically_on_write(1);
	bool success = stbi_write_png(filename.c_str(),windowWidth,windowHeight, 3, pixels, 0);
	if(!success)
		std::cerr<<"Saving screenshot failed!"<<std::endl;

	delete[] pixels;
}

void checkOpenGLVersion() {
	const char *version_p = (const char *)glGetString(GL_VERSION);
	float version = 0.0f;

	if(version_p != NULL)
		version = (float)atof(version_p);

	if(version < 3.1f) {
		std::cout << std::endl << "*****************************************" << std::endl;

		if(version_p != NULL) {
			std::cout << "WARNING: Your OpenGL version is not supported." << std::endl;
			std::cout << "We detected version " << std::fixed << std::setprecision(1) << version;
			std::cout << ", but at least version 3.1 is required." << std::endl << std::endl;
		} else {
			std::cout << "WARNING: Your OpenGL version could not be detected." << std::endl << std::endl;
		}

		std::cout << "Please update your graphics drivers BEFORE posting on the forum. If this" << std::endl
				  << "doesn't work, ensure your GPU supports OpenGL 3.1 or greater." << std::endl;

		std::cout << "If you receive a 0xC0000005: Access Violation error, this is likely the reason." << std::endl;

		std::cout << std::endl;

		std::cout << "Additional OpenGL Info:" << std::endl;
		std::cout << "(Please include with support requests)" << std::endl;
		std::cout << "GL_VERSION: ";
		std::cout << glGetString(GL_VERSION) << std::endl;
		std::cout << "GL_VENDOR: ";
		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << "GL_RENDERER: ";
		std::cout << glGetString(GL_RENDERER) << std::endl;

		std::cout << std::endl << "*****************************************" << std::endl;
		std::cout << std::endl << "Select terminal and press <ENTER> to continue." << std::endl;
		std::cin.get();
		std::cout << "Select OpenGL window to use commands below." << std::endl;
	}
}

void printMatrix(glm::mat3 matrix)
{
	for(int row=0; row<3; row++)
	{
		std::cout<<"[ ";
		for(int col=0; col<3; col++)
			std::cout<<matrix[col][row]<<", ";
		std::cout<<"]\n";
	}
}
void printMatrix(glm::mat4 matrix)
{
	for(int row=0; row<4; row++)
	{
		std::cout<<"[ ";
		for(int col=0; col<4; col++)
			std::cout<<matrix[col][row]<<", ";
		std::cout<<"]\n";
	}
}
void printVector(glm::vec2 vector)
{
		std::cout<<"[ "<<vector[0]<<", "<<vector[1]<<"]^T";
}
void printVector(glm::vec3 vector)
{
	std::cout<<"[ "<<vector[0]<<", "<<vector[1]<<", "<<vector[2]<<"]^T";
}
void printVector(glm::vec4 vector)
{
	std::cout<<"[ "<<vector[0]<<", "<<vector[1]<<", "<<vector[2]<<", "<<vector[3]<<"]^T";
}
