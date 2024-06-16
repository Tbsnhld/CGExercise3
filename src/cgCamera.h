/** \file cgCamera.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef _CGCAMERA_H_
#define _CGCAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*! \brief This is a simple class to describe the camera in the scene.
 *
 *  This class provides a looking class interface to rotate the
 *  camera around a scene.
 */
class cgCamera {
public:
	cgCamera(glm::vec3 _pos, glm::vec3 _center, glm::vec3 _up, float _fovy, float _aspect=1.0);
	cgCamera() = default;

	// all member variables are public so you can access with camera.position etc.
	glm::vec3 position = {0.0, 0.0, 2.0};   // position of the camera
	glm::vec3 center = {0.0, 0.0, 0.0};     // center point to view at
	glm::vec3 view_up = {0.0, 1.0, 0.0};  // view-up vector
	float fovy_degree = 30;               // FOV y in degree
	float aspectRatio = 1.0;              // aspect ratio of screen size
	                                      // width / height
	float znear = 0.1f;    // distance to the front plane
	float zfar = 100.0f;   // distance to the back plane

	// moves the camera towards or away from the center
	// already implemented in cgCamera.cpp
	void dolly(float amount);
	// rotates the camera horizontally around the center
	// implement this method in cgCamera.cpp
	void left(float degrees);
	// rotates the camera vertically around the center
	// implement this method in cgCamera.cpp
	void up(float degrees);

	// returns the 4x4 view matrix of the camera
	glm::mat4 getViewMatrix() const {
		return glm::lookAt(position, center, view_up);
	}
	// returns a 4x4 perspective projection matrix of the camera
	glm::mat4 getProjectionMatrix() const {
		return glm::perspective(glm::radians(fovy_degree), aspectRatio, znear, zfar);
	}
};

#endif // _CGCAMERA_H_
