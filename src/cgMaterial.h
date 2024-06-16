/** \file cgMaterial.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef CGMATERIAL_H_
#define CGMATERIAL_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/*! \brief This class describes the material of an object,
 *         i.e. the object color and lighting coefficients
 *
 *  This class describes the object color using the Phong illumination
 *  model. Ambient, diffuse and specular color are computed by the object
 *  color multiplied with the lighting coefficients (ka, kd or ks).
 *  Some objects have specular reflections in a different color, so specular
 *  colors can set separately if required.
 */
class cgMaterial {
public:
	/*! Constructor with given color, lighting coefficients and shininess */
	cgMaterial(glm::vec3 _color, float _ka, float _kd, float _ks, float _shininess =	5.0) :
		color(_color), specular_color(_color), ka(_ka), kd(_kd), ks(_ks),
			shininess(_shininess) {}
	/*! Constructor with separate specular color, lighting coefficients and shininess */
    cgMaterial(glm::vec3 _color, glm::vec3 _specular_color, float _ka,
            float _kd, float _ks, float _shininess = 30.0) :
            	color(_color), specular_color(_specular_color), ka(_ka),
				kd(_kd), ks(_ks), shininess(_shininess){};
    /*! default constructor using default values, (black material) */
	cgMaterial() = default;


	glm::vec3 color = { 0.0, 0.0, 0.0 };
    glm::vec3 specular_color = { 0.0, 0.0, 0.0 };
	float ka = 0.2f;
	float kd = 0.8f;
	float ks = 0.0f;
	float shininess = 5.0f;


};

/*
 * define some global constants for different materials
 */
const cgMaterial yellowRubber {
        { 1.0, 1.0, 0.0 },  // color
          0.3, 0.5, 0.2      // ka, kd, ks
};
const cgMaterial whitePlastic {
       { 1.0, 1.0, 1.0 },  // color (ambient + diffuse)
       { 1.0, 1.0, 1.0 },  // specular color
         0.5, 0.9, 0.9,      // ka, kd, ks
        32 // shininess
};
const cgMaterial redPlastic {
       { 1.0, 0.0, 0.0 },  // color (ambient + diffuse)
       { 1.0, 0.85, 0.85 },  // specular color
         0.3, 0.5, 0.8,      // ka, kd, ks
        32 // shininess
};
const cgMaterial greenPlastic {
       { 0.0, 1.0, 0.0 },  // color (ambient + diffuse)
       { 0.85, 1.0, 0.85 },  // specular color
         0.3, 0.5, 0.7,      // ka, kd, ks
        32 // shininess
};
const cgMaterial bluePlastic {
       { 0.0, 0.0, 1.0 },  // color (ambient + diffuse)
       { 0.85, 0.85,1.0 },  // specular color
         0.3, 0.5, 0.7,      // ka, kd, ks
        32 // shininess
};
const cgMaterial cyanPlastic {
       { 0.0, 1.0, 1.0 },  // color (ambient + diffuse)
       { 0.85, 1.0, 1.0 },  // specular color
         0.1, 0.5, 0.7,      // ka, kd, ks
        32 // shininess
};
const cgMaterial yellowPlastic {
       { 1.0, 1.0, 0.0 },  // color (ambient + diffuse)
       { 1.0, 1.0, 0.85 },  // specular color
         0.1, 0.5, 0.7,      // ka, kd, ks
        32 // shininess
};
const cgMaterial whiteRubber {
       { 1.0, 1.0, 1.0 },  // color (ambient + diffuse)
         0.3, 0.5, 0.2      // ka, kd, ks
};
const cgMaterial gold {
        { 1.0, 0.8, 0.3},  // color (ambient + diffuse)
        { 1.0, 0.88, 0.58 },  // specular color
        0.25, 0.75, 0.6, // ka, kd, ks
        100 // shininess
};

#endif // CGMATERIAL_H_
