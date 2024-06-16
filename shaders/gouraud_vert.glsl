#version 330 core                                                                       
// Do not modify the above version directive to anything older.                                                                  

//
// Note that uniform variables defined but not used are removed from 
// the GLSL code. 
// In this case you will get error messages at runtime like
// ERROR: variable material.specular of type vec3 does not exist in shader sources!
// although the variable is defined correctly in the shader.
// 

// input variables from OpenGL buffers:
// position and normal in model coordinates                                                              
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal; 

// define a struct type for object materials
// given by color, specular color, reflection coeffs and shininess
struct Material {
    vec3 color;           // color of the material
    vec3 specular_color;  // color of specular reflection (needed for some materials)
    float ka;             // to compute ambient color: ka*color
    float kd;             // to compute diffuse color: kd*color
    float ks;             // to compute specular color: ks*specular_color
    float shininess;
}; 

// define a struct type for point lights
struct PointLight {
    vec3 position;
    vec3 color;
};

// transformation matrices
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

// Variables for illumination (Gouraud shading is applied to the vertices!)
uniform vec3 ambientLightColor; // ambient light color in scene
uniform vec3 cameraPosition;    // poition of camera in world coords
uniform int numLights;          // actual number of used lights
uniform PointLight pointLight[5]; // we support up to 5 lights
// the material properties of the object we render
uniform Material material;

// output variable for the fragment shader
// you can use a variable with same type and name in the fragment shader
out vec3 myColor;

// Define a local function for the Lambertian (diffuse) illumination model
// per Light:     C = O_d * I_L * cos(\theta) (if theta < 90° else 0)
//                C = O_d * I_L * <N,L>
// Note that N,L have to be normalized.
vec3 ComputeLambertian(const in vec3 dir_to_light, const in vec3 lightColor, 
                       const in vec3 normal, const in vec3 diffuseColor) {

        float nDotL = dot(normal, dir_to_light);         
        vec3 lambert = diffuseColor * lightColor * max (nDotL, 0.0) ; 
        return lambert;            
}       

// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
// Define a local function for the specular illumination model of Phong
// per Light:     C = O_s * I_L * cos^s(\alpha) (if alpha < 90° else 0)
//                C = O_s * I_L * <N,R>^s
// Note that N,L,R have to be normalized, R is the reflected light vector L.
vec3 specularIllumination(const in vec3 dir_to_light, const in vec3 lightColor,
                          const in vec3 normal, const in vec3 spectralColor,  
                          const in float spectralExponent, const in vec3 view) {
        vec3 reflectedDir = normalize(reflect(-dir_to_light, normal));
        float nDotR =max(dot(view, reflectedDir), 0.0);
        float nDotRpowerS = pow(nDotR, spectralExponent);
        vec3 spectral = spectralColor * lightColor * nDotRpowerS;
        return spectral;

    }

void main()                                                              
{
	// compute vertex position in normalized viewing volume
	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(inPosition, 1.0);
	
	//
	// now we start with the illumination model
	//
	// get world coordinates because lights are defined in world coords
	vec4 myPosition_world = modelMatrix*vec4(inPosition, 1.0);
	// compute the transformed normal (see lectures)                                       
    vec3 normal =normalize(transpose(inverse(mat3(modelMatrix))) *  inNormal);
    
    // compute ambient component of Phongs model
    myColor = ambientLightColor*material.ka*material.color; 
      
	// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
    // Add your code to compute Lambertian and specular components of 
    // Phongs illumination model and Phong shading here.
    //
    // Process all light sources using a for loop depending on numLights 
    //
    if (numLights > 0) {
        // Lambertian componen for one point light is given, extend the code for multiple lights
        // compute the direction to the light (GLSL swizzeling is used)
        for(int i = 0; i < numLights; i ++){
            vec3 dir_to_light = normalize(pointLight[i].position - myPosition_world.xyz);
            // Compute the lambertian color component for this light source nd add to total color
            myColor += ComputeLambertian(dir_to_light, pointLight[i].color, normal, material.kd*material.color);
            // Compute the specular color component for this light source nd add to total color
            vec3 viewDir = normalize(cameraPosition - myPosition_world.xyz);
            myColor += specularIllumination(dir_to_light, pointLight[i].color, normal, material.ks*material.specular_color, material.shininess, viewDir);

        }

    }
}
