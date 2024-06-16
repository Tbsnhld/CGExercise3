/** \file exercise3.cpp
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#include <iostream>     // include header for std::cout
#include <vector>       // include header for std::vector
#include <chrono>       // C++11 time functions (milliseconds())
#include <thread>       // C++11 thread functions (here: sleep_for())

#include <glm/glm.hpp>  // GLM is a header-only library with many useful
					    // matrix-vector operations
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>    // GLEW provids OpenGL headers in a
                        // platform independent way
#include <GL/glut.h>    // used to create and manage windows
                        // in a platform independent way
#endif

// include files of this project
#include "utils.h"
#include "validation.h"
#include "cgCamera.h"
#include "cgShader.h"
#include "cgMesh.h"
#include "scenes.h"

/*
 * define some global variables
 */
bool usePhongShading = false;

// define initial window size
const unsigned int inititalWindowWidth = 500;
const unsigned int inititalWindowHeight = 500;
// the initial position of our camera (needed for reset)
cgCamera initialCamera {{0.0, 1.0, 3.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, 30.0, 1.0};
// the current position of our camera
cgCamera camera = initialCamera;
// the GLSL shaders we use to render the scene
cgShader phongShader;
cgShader gouraudShader;
// the geometries we use to render the scenes
cgMesh teapot_mesh;
cgMesh cube_mesh;
cgMesh plane_mesh;
// an array withlights we use in the scene, start with one light
std::vector<cgLight> scene_ligths = {cgLight(glm::vec3(1.0, 1.0, 1.0) * 0.7f,glm::vec3(1.0, 1.0, 2.0)) };

// needed for mouse interaction
glm::vec2 oldMousePosition = {0, 0};
// needed for keyboard interaction
const float initialRotationAmount = 2.0f;
float rotationAmount = initialRotationAmount;

/**
 *  This function performs the rendering of the scene
 *  is called whenever the window needs to be re-drawn
 */
void onDisplay(void) {
	// clear the current color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!usePhongShading)
		draw_table_scene(camera,gouraudShader, scene_ligths);
	else
		draw_table_scene(camera,phongShader, scene_ligths);

	// swap hidden rendering buffer with displayed frame-buffer
	glutSwapBuffers();
	glutPostRedisplay();
}

void onResize(int w, int h) {
	if (h == 0)
		h = 1;
	// compute aspect ratio (float division!)
	float ratio = (float) w / (float) h;
	// update the aspect ratio in the scene camera
	camera.aspectRatio = ratio;
	// set new viewport size in OpenGL
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	// redraw scene
	glutPostRedisplay();
}

void printHelp() {
	std::cout << "\npress 'h' to print this message again.\n"
			<< "press '1','2','3' or '0' to change the number of lights in the scene\n"
			<< "press 'g' to switch between Gouraud and Phong shading.\n"
			<< "press 'r' to reset the camera transformations.\n"
			<< "press 'o' to reset camera and window and save a screenshot.\n"
			<< "press 'c' to run the automatic validation.\n"
			<< "press ESC to quit.\n";
}

/*! Defines a Mouse callback to zoom in and out
 *  This is done by modifying gluLookAt
 *  The actual motion is in mousedrag
 *  mouse simply sets state for mousedrag */
void onMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			oldMousePosition[0] = float(x);
			oldMousePosition[1] = float(y); // so we can move wrt x , y
		}
	}
}

/*! Simulates some camera motion on mouse drag.
 *  Note that this camera motion is not a clever interaction mode. */
void onMouseDrag(int x, int y) {
	float xloc = float(x) - oldMousePosition[0];    // use the x coord to move camera left/rigth
	float yloc = float(y) - oldMousePosition[1];    // use the y coord to move camera up/down
	// clamp camera position to avoid problems with view direction
	camera.position[0] = float(glm::clamp(camera.position[0] - 0.005*xloc, -2.0 , 2.0));
	camera.position[1] = float(glm::clamp(camera.position[1] + 0.005*yloc, -4.0 , 4.0));
	oldMousePosition[0] = float(x);
	oldMousePosition[1] = float(y);
	// re-display the window
	glutPostRedisplay();
}

void onKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:  // Escape to quit
		exit(0);
		break;
	case 'r': {
		// reset camera parameters, but keep aspect ratio (not changed)
		float aspect =  camera.aspectRatio;
		camera = initialCamera;
		camera.aspectRatio = aspect;
		rotationAmount = initialRotationAmount;
	    }
		break;
	case 'o':
		// reset window size to default
		glutReshapeWindow(inititalWindowWidth, inititalWindowHeight);
		// reset camera to default
		camera = initialCamera;
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); // for some reasons we have to wait ...
		// call resize so that GL_VIEWPORT is set correctly
		onResize(inititalWindowWidth, inititalWindowHeight);
		onDisplay();// re-draw before saving
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// save a screen-shot
		saveScreenshot("screenshot.png");
		break;
	case '0':
		// remove all point lights -- ambient light only
		scene_ligths.clear();
		std::cout<<"Ambient light only.\n"<<std::flush;
		break;
	case '1':
		// set one point light in the scene
		scene_ligths.clear();
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 1.0, 1.0) * 0.7f,glm::vec3(1.0, 1.0, 2.0)));
		std::cout<<"Use 1 Light.\n"<<std::flush;
		break;
	case '2':
		// set two point lights in the scene
		scene_ligths.clear();
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 1.0, 1.0) * 0.5f,glm::vec3(1.0, 1.0, 2.0)));
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 0.3, 0.3) * 0.5f,glm::vec3(-1, 1.0, 2.0)));
		std::cout<<"Use 2 Lights.\n"<<std::flush;
		break;
	case '3':
		// set three point lights in the scene
		scene_ligths.clear();
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 1.0, 1.0) * 0.4f,glm::vec3(1, 1.0, 2.0)));
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 0.3, 0.3) * 0.4f,glm::vec3(-1, 1.0, 2.0)));
		scene_ligths.push_back(cgLight(glm::vec3(1.0, 1.0, 1.0) * 0.4f,glm::vec3(0.0, 1.0, 1.0)));
		std::cout<<"Use 3 Lights.\n"<<std::flush;
		break;
	case 'g':
	case 'p':
		// toggle between Gouraud and Phong Shading
		usePhongShading = !usePhongShading;
		std::cout<<"Using "<<((usePhongShading)? "Phong" : "Gouraud")<<" Shading.\n"<<std::flush;
		break;
	case 'c':
		std::cout<<"\n=========================================\n";
		std::cout<<"Checking your Implementation:\n";
		runCheck();
		break;
	case 'h':
	case '?':
		printHelp();
		break;
	}
	glutPostRedisplay();
}

void onSpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT: //left
		camera.left(rotationAmount);
		break;
	case GLUT_KEY_UP: //up
		camera.up(rotationAmount);
		break;
	case GLUT_KEY_RIGHT: //right
		camera.left(-rotationAmount);
		break;
	case GLUT_KEY_DOWN: //down
		camera.up(-rotationAmount);
		break;
	case GLUT_KEY_PAGE_UP: //page up
		camera.dolly(0.005f*rotationAmount);
		break;
	case GLUT_KEY_PAGE_DOWN: //page down
		camera.dolly(-0.005f*rotationAmount);
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	// Initialize freeglut library
	glutInit(&argc, argv);
	// OSX systems require another flag
#ifdef __APPLE__
	glutInitDisplayMode (GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#else
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
#endif
	// define window size and position
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	// create the window for our OpenGL output
	glutCreateWindow("Computergrafik Projekt 3");
	// register the callback function used for
	// refreshing the window content
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onResize);
	glutMouseFunc(onMouseClick) ;
	glutMotionFunc(onMouseDrag) ;
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onSpecialKey);

#ifndef __APPLE__
	// initialize GLEW (used for more platform independence)
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Error: " << glewGetString(err) << std::endl;
		exit(EXIT_FAILURE);
	}
#endif
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION)
			<< std::endl;

	checkOpenGLVersion();
	printHelp();

	//
	// OpenGL Initialization
	//
	// define background color as RGBA
	glClearColor(0.1f, 0.1f, 0.4f, 0.0f);
	// enable z-buffering (see lecture XX)
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//
	// Initialize GLSL Shader objects
	//
	// We initialize two shaders here for Gouraud-Shading and Phong-Shading
	// so w can switch between two GLSL shader programms at runtime

	// load GLSL code for vertex shader and fragment shader from file and
	// compile and link this code into a shader program
	bool success = gouraudShader.LoadShadersFromFile("shaders/gouraud_vert.glsl",
			"shaders/gouraud_frag.glsl");
	if (!success || !gouraudShader.IsCompiledAndLinked()) {
		std::cerr << "Gouraud Shader initialization failed!";
		return EXIT_FAILURE;
	}
	// Setup the Phong shader with your shader source code
	// load GLSL code for vertex shader and fragment shader from file and
	// compile and link this code into a shader program
	success = phongShader.LoadShadersFromFile("shaders/phong_vert.glsl",
			"shaders/phong_frag.glsl");
	if (!success || !phongShader.IsCompiledAndLinked()) {
		std::cerr << "Phong Shader initialization failed!";
		return EXIT_FAILURE;
	}

	//
	// Create and initialize the Geometries (Meshes)
	//
	teapot_mesh = create_teapot("teapot.obj");
	success = teapot_mesh.initOpenGLBuffers();
	cube_mesh = create_cube();
	success &= cube_mesh.initOpenGLBuffers();
	plane_mesh = create_plane();
	success &= plane_mesh.initOpenGLBuffers();
	if(!success) {
		std::cerr << "Geometry initialization failed!";
		return EXIT_FAILURE;
	}

	// here we enter the application event loop, so we can later
	// interact with the window
	glutMainLoop();

	phongShader.deleteOpenGLShaders();
	gouraudShader.deleteOpenGLShaders();
	return 0; // ANSI C requires main to return int.
}

