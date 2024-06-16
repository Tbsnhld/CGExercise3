/** \file validation.cpp
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#include <string>
#include <iostream>     // include header for std::cout
#include <vector>       // include header for std::vector
#include <chrono>       // C++11 time functions (milliseconds())
#include <thread>       // C++11 thread functions (here: sleep_for())

// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>    // GLEW provids OpenGL headers in a
                        // platform independent way (not for APPLE)
#include <GL/glut.h>    // used to create and manage windows
                        // in a platform independent way
#endif

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "stb_image.h"
#include "utils.h"
#include "cgShader.h"

extern bool usePhongShading;

// defined in main file
void onKeyboard(unsigned char key, int x, int y);

// forward declaration
bool compareImages(std::string filename1, std::string filename2, float epsilon = 1.1f);
bool checkShaderVariables(std::string shadername, std::vector<std::string> variables);

//
// Runs a litle test if your generated images are identical to the
// given ground truth images
// (note that plattform dependent difference might prevent that this
//  will work for you -- check with the initial image before changing
//  the shaders)
// ERROR messages might appear as long as your shader code is not finished
void runCheck() {
	// check shader implementations
	bool shader_check = checkShaderVariables("gouraud", { "cameraPosition", "material.shininess", "material.kd", "material.ks", "material.specular_color" });
	if (!shader_check) {
		std::cout << "Phong illumination model does not seem to be fully implemented in gouraud shader! Some checks will probably fail.\n";
	}
	shader_check = checkShaderVariables("gouraud", { "numLights", "pointLight[0].position", "pointLight[0].color",
			"pointLight[1].position", "pointLight[1].color", "pointLight[2].position", "pointLight[2].color"});
	if (!shader_check) {
		std::cout << "Multiple lights does not seem to be fully implemented in gouraud shader! Some checks will probably fail.\n";
	}
	shader_check = checkShaderVariables("phong", { "cameraPosition", "material.shininess", "material.kd", "material.ks", "material.specular_color"});
	if (!shader_check) {
		std::cout << "Phong illumination model does not seem to be fully implemented in phong shader! Some checks will probably fail.\n";
	}
	shader_check = checkShaderVariables("phong", { "numLights", "pointLight[0].position", "pointLight[0].color",
			"pointLight[1].position", "pointLight[1].color", "pointLight[2].position", "pointLight[2].color"});
	if (!shader_check) {
		std::cout << "Multiple lights does not seem to be fully implemented in phong shader! Some checks will probably fail.\n";
	}

	onKeyboard('1', 0, 0);  // use one lights
	usePhongShading = false;     // gouraud shading
	onKeyboard('r', 0, 0);
	onKeyboard('o', 0, 0);
	std::string outfile = "screenshot1.png";
	std::string testfile = "testimages/checkinitial.png";
	saveScreenshot(outfile);
	std::cout<<"###> Check with initial image ... ";
	if(compareImages(outfile, testfile, 1.1f))
		std::cout<<"is identical.\n";
	else
		std::cout<<"differs. (This difference is correct if specular illumination is implemented.)\n";
	std::cout<<"###> Check Gouraud shaded image (1 light) ... ";
	if(compareImages(outfile, "testimages/screenshot_gouraud_1light.png", 1.1f))
		std::cout<<"is identical.\n";
	else
		std::cout<<"differs.\n";
	onKeyboard('2', 0, 0);  // use two lights
	onKeyboard('r', 0, 0);  // gouraud shading
	onKeyboard('o', 0, 0);
	outfile = "screenshot2.png";
	testfile = "testimages/screenshot_gouraud_2lights.png";
	saveScreenshot(outfile);
	std::cout<<"###> Check Gouraud shaded image (2 lights) ... ";
	if(compareImages(outfile, testfile, 1.1f))
		std::cout<<"is identical.\n";
	else
		std::cout<<"differs.\n";
	usePhongShading = true;     // phong shading
	onKeyboard('1', 0, 0);  // use two lights
	onKeyboard('r', 0, 0);
	onKeyboard('o', 0, 0);
	outfile = "screenshot3.png";
	testfile = "testimages/screenshot_phong_1light.png";
	saveScreenshot(outfile);
	std::cout<<"###> Check Phong shaded image (1 light) ... ";
	if(compareImages(outfile, testfile, 1.1f))
		std::cout<<"is identical.\n";
	else
		std::cout<<"differs.\n";
	usePhongShading = true;     // phong shading
	onKeyboard('3', 0, 0);  // use two lights
	onKeyboard('r', 0, 0);
	onKeyboard('o', 0, 0);
	outfile = "screenshot4.png";
	testfile = "testimages/screenshot_phong_3lights.png";
	saveScreenshot(outfile);
	std::cout<<"###> Check Phong shaded image (3 lights) ... ";
	if(compareImages(outfile, testfile, 1.1f))
		std::cout<<"is identical.\n";
	else
		std::cout<<"differs.\n";

	onKeyboard('1', 0, 0); // use one lights
	onKeyboard('g', 0, 0); // gouraud shading
}


bool compareImages(std::string filename1, std::string filename2, float epsilon) {
// load image, create texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
// load image file with stbi library
	unsigned char *imageBuffer1 = stbi_load(filename1.c_str(), &width, &height,
			&nrChannels, 0);
	if (imageBuffer1 == nullptr) {
		std::cerr << "Can not open file " << filename1 << "!" << std::endl << std::flush;
		return false;
	}
	if (nrChannels != 3 && nrChannels != 4) {
		std::cerr << "Only RGB or RGBA images are supported for textures!"
				<< std::endl << std::flush;
		return false;
	}
	int size1 = width * height * nrChannels;

	unsigned char *imageBuffer2 = stbi_load(filename2.c_str(), &width, &height,
			&nrChannels, 0);
	if (imageBuffer2 == nullptr) {
		std::cerr << "Can not open file " << filename2 << "!" << std::endl << std::flush;
		return false;
	}
	if (nrChannels != 3 && nrChannels != 4) {
		std::cerr << "Only RGB or RGBA images are supported for textures!"
				<< std::endl << std::flush;
		return false;
	}

	int size = width * height * nrChannels;
	if(size1 != size)
	{
		std::cout<<"images have different size!\n;";
		return false;
	}
	unsigned int diff_count = 0;
	for (int i = 0; i < size; i++) {
		if (fabs((float) (imageBuffer2[i]) - (float) (imageBuffer1[i]))
				> epsilon)
			diff_count++;
	}
	float diff_ratio = (float)diff_count/(float)size;
	if(diff_ratio > 0.001) {
		std::cout<<(diff_ratio*100)<<"% ("<<diff_count<<") of image pixels differ.\n";
		return false;
	}

	return true;
}


bool checkShaderVariables(std::string shadername, std::vector<std::string> variables) {
	cgShader testShader;
	std::string vertfile = "shaders/" + shadername + "_vert.glsl";
	std::string fragfile = "shaders/" + shadername + "_frag.glsl";

	bool success = testShader.LoadShadersFromFile(vertfile, fragfile);
	if (!success || !testShader.IsCompiledAndLinked()) {
		std::cout << "Shader initialization failed!";
		return false;
	}
	bool retVal = true;
	for (std::string variable : variables) {
		if (!testShader.checkUniformVariable(variable)) {
			std::cout << "  Necessary variable " << variable << " is not used in files " << vertfile << " , " << fragfile << " !\n";
			retVal = false;
		}
	}
	return retVal;
}

