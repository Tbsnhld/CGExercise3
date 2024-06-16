/** \file scenes.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef _SCENES_H_
#define _SCENES_H_
#include <vector>
#include "cgLight.h"
#include "cgCamera.h"
#include "cgShader.h"

// Scene 1: a scene with teapot on a table
void draw_table_scene(cgCamera camera, cgShader shader, std::vector<cgLight> lights);

// other scenes will be added later

#endif
