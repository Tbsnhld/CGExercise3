/** \file cgLight.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef CGLIGHT_H_
#define CGLIGHT_H_

#include <glm/glm.hpp>

/*! \brief This is a simple struct to describe lights in the scene.
 *
 *  We support only point lights.
 */
struct cgLight {
	cgLight(glm::vec3 _color, glm::vec3 _position) {
		color = _color;
		position = _position;
	};

	cgLight() = default;

	glm::vec3 color = {1.0, 1.0, 1.0};  // color as RGB
    glm::vec3 position = {0.0, 1.0, 0.0};   // position in world coordinates
};
#endif // CGLIGHT_H_
