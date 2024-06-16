/** \file scenes.cpp
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
#include <vector>
#include <string>
#include "cgMesh.h"
#include "cgCamera.h"
#include "cgShader.h"
#include "cgMaterial.h"
#include "cgLight.h"
#include "scenes.h"

// definition of global variables needed for scene drawing
// Note 'extern' variables are defined and initialized somewhere else (in exercise3.cpp)
extern cgMesh cube_mesh;
extern cgMesh plane_mesh;
extern cgMesh teapot_mesh;

// this is a helper function to avoid many duplicated code lines
void setMaterialToShader(cgShader shader, cgMaterial material)
{
	shader.setUniformVec3("material.color", material.color); // object color
	shader.setUniformFloat("material.ka", material.ka);  // ambient reflection coefficient
	if (shader.checkUniformVariable("material.kd"))
		shader.setUniformFloat("material.kd", material.kd);  // diffuse reflection coefficient
	// The following shader variables are only defined if specular illumination is 
	// implemented in the GLSL shader code
	// Note the GLS variables have to be defined AND used to be present
	if (shader.checkUniformVariable("material.specular_color"))
		shader.setUniformVec3("material.specular_color", material.specular_color); // object specular color
	if (shader.checkUniformVariable("material.ks"))
		shader.setUniformFloat("material.ks", material.ks);  // specular reflection coefficient
	if (shader.checkUniformVariable("material.shininess"))
		shader.setUniformFloat("material.shininess", material.shininess); // specular exponent
}


void draw_table_scene(cgCamera camera, cgShader shader, std::vector<cgLight> lights) {
	//
	// make some error checking only to be sure
	//
	if (!shader.IsCompiledAndLinked()) {
		std::cerr << "Shader object is not correctly initialized!\n";
		return;
	}
	// enable the shader object to use it
	shader.enableShader();

	// get projection matrix from glm::perspective() function
	glm::mat4 projectionMatrix = camera.getProjectionMatrix();
	// get view matrix from glm::lookAt() function
	glm::mat4 viewMatrix = camera.getViewMatrix();

	// setup view and projection matrix
	shader.setUniformMat4("projectionMatrix", projectionMatrix);
	shader.setUniformMat4("viewMatrix", viewMatrix);

	// The following shader variables are only defined if specular illumination is 
	// implemented in the GLSL shader code, so we first check to avoid runtime errors
	// Note the GLS variables have to be defined AND used to be present
	if (shader.checkUniformVariable("cameraPosition"))
		// set position of the camera (needed for specular illumination)
		shader.setUniformVec3("cameraPosition", camera.position);

	//
	// Setup the lights in the scene
	//
	// set ambient light
	shader.setUniformVec3("ambientLightColor", glm::vec3(0.3,0.3,0.3));
	// set number of lights to the shader (maximum 5)
	int numLights = std::min((int)lights.size(), 5);
	if (shader.checkUniformVariable("numLights"))
		shader.setUniformInt("numLights", numLights);
	// setup the point lights location and colors
	for (int i = 0; i < numLights; i++) {
		// note that pointLight is an array of structs in the GLSL code
		// build the GLSLS variable name as string
		std::string lightpos_name = "pointLight[" + std::to_string(i)
			+ "].position";
		std::string lightcol_name = "pointLight[" + std::to_string(i)
			+ "].color";
		if (shader.checkUniformVariable(lightpos_name)) {
			// set the variables with generated name in the shader
			shader.setUniformVec3(lightpos_name, lights[i].position);
			shader.setUniformVec3(lightcol_name, lights[i].color);
		}
	}

	//////////////////////////////////////////////////////////////
	//
	// Render the Scene
	//

	//
	// Render the TABLE
	//
	// setup model matrix of the floor plane
	glm::mat4 modelMatrix = glm::mat4(1.0); // set to identity
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the floor in the shader (only visible if no texture is drawn)
	setMaterialToShader(shader, whitePlastic);
	// draw the floor plane mesh
	plane_mesh.drawSolid();

	//
	// Render the teapot
	//
	// setup model matrix of the teapot
	float size = 0.235f; // Teapot size
	modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(size, size, size));
	modelMatrix = modelMatrix * glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.5, 0.0));
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the teapot in the shader
	setMaterialToShader(shader, gold);
	// draw the teapot mesh
	teapot_mesh.drawSolid();


	//
	// Render first pillar
	//
	modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.4, -0.2501, 0.4));
	modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.5, 0.1));
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the pillar
	setMaterialToShader(shader, yellowRubber);
	// draw the cuboid mesh
	cube_mesh.drawSolid();

	//
	// Render second pillar
	//
	modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-0.4, -0.2501, 0.4));
	modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.5, 0.1));
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the pillar
	setMaterialToShader(shader, cyanPlastic);
	// draw the cuboid mesh
	cube_mesh.drawSolid();

	//
	// Render 3rd pillar
	//
	modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-0.4, -0.2501, -0.4));
	modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.5, 0.1));
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the pillar
	setMaterialToShader(shader, redPlastic);
	// draw the cuboid mesh
	cube_mesh.drawSolid();

	//
	// Render 4th pillar
	//
	modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.4, -0.2501, -0.4));
	modelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.5, 0.1));
	shader.setUniformMat4("modelMatrix", modelMatrix);
	// setup material of the pillar
	setMaterialToShader(shader, bluePlastic);
	// draw the cuboid mesh
	cube_mesh.drawSolid();

	// end shader here
	shader.endShader();
}

