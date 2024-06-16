#version 330 core                                                                       
// Do not modify the above version directive to anything older.                                                                  

// input variables from OpenGL buffers:
// position and normal in model coordinates                                                              
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

// transformation matrices
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
// Add output variables needed for illumination to fragment shader
                                                                 
void main()                                                              
{
	// Needed: compute vertex position in normalized viewing volume
	// gl_Position is a predefined GLSL variable of type vec4 you have to compute
	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(inPosition, 1.0);
	
	// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
    // Compute the output variables for the fragment shader
    // Note that all spatial transformations have to applied here (per vertex and not per fragment)
}