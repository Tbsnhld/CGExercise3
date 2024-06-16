/** \file cgCamera.cpp
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2024 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/reciprocal.hpp>
#include "utils.h"
#include "cgCamera.h"

/*! \brief This is a simple class to describe the camera in the scene.
 *
 *  This will be extended in project 2
 */
cgCamera::cgCamera(glm::vec3 _pos, glm::vec3 _center, glm::vec3 _up, float _fovy, float _aspect) {
	position = _pos;
	center = _center;
	view_up = _up;
	fovy_degree = _fovy;
	aspectRatio = _aspect;
}

// moves the camera towards or away from the center
void cgCamera::dolly(float amount) {
	// change position of the camera here
	// Remember: distance between center and position should not be less than 1 (approximately)
	if (amount < 0 || glm::distance(center, position) > 1)
		position += (center - position) * amount;
}

// rotates the camera horizontally around the center
void cgCamera::left(float degrees) {
	// YOUR CODE FOR Exercise2 HERE
	// change position and/or view up of the camera here
	// you can optionally add your code for Exercise2 here
	// (implemented using glm functions as transform.cpp is not present in the project)
}

// rotates the camera vertically around the center
void cgCamera::up(float degrees) {
	// YOUR CODE FOR Exercise2 HERE
	// change position and/or view up of the camera here
	// you can optionally add your code for Exercise2 here
	// (implemented using glm functions as transform.cpp is not present in the project)
}

