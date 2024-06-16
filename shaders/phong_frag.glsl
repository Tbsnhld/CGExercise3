#version 330 core                                                                       
// Do not modify the above version directive to anything older.                                                                  
                                                                                    
//
// Note that uniform variables defined but not used are removed from 
// the GLSL code. 
// In this case you will get error messages at runtime like
// ERROR: variable material.shininess of type vec3 does not exist in shader sources!
// although the variable is defined correctly in the shader.
// 

// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
// Add input variables needed for illumination from vertex shader
in vec3 normal;
in vec3 position;


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

// Variables for illumination (Phong shading is applied to the fragments!)
uniform vec3 ambientLightColor; // ambient light color in scene
uniform vec3 cameraPosition;    // poition of camera in world coords
uniform int numLights;          // actual number of used lights
uniform PointLight pointLight[5]; // we support up to 5 lights
// the material properties of the object we render
uniform Material material;

// NEEDED: output variable: the fragment color in RGBA
out vec4 FragColor;                                                                 
                                                                                    
// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
// You can add functions for Lambertian and specular illumination here 
vec3 ComputeLambertian(const in vec3 dir_to_light, const in vec3 lightColor, 
        const in vec3 normal, const in vec3 diffuseColor) {

    float nDotL = dot(normal, dir_to_light);         
    vec3 lambert = diffuseColor * lightColor * max (nDotL, 0.0) ; 
    return lambert;            
}       

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
	//
	// Compute the illumination model
	//
 
    // compute ambient component of Phongs model
    vec3 myColor = ambientLightColor*material.ka*material.color;
      
	// !!!!!!!!!!!! TODO: cgExercise3 Add Code here !!!!!!!!!!!!!!!!!
    // Add your code to compute Lambertian and specular components of 
    // Phongs illumination model and Phong shading here.
    //
    // Process all light sources using a for loop depending on numLights 
    //
    // You can define functions to compute the illumination model components
    //
    // You need to pass variables from the vertex shader to the fragment shader
    //
    // Note that interpolated vertex normals have to be re-normalized after interpolation
    // in the rasterizer

    vec3 n = normalize(normal);


    if (numLights > 0){
        for(int i = 0; i < numLights; i++){
            vec3 dir_to_light = normalize(pointLight[i].position - position.xyz);
            myColor += ComputeLambertian(dir_to_light, pointLight[i].color, n, material.kd*material.color);
            // Compute the specular color component for this light source nd add to total color
            vec3 viewDir = normalize(cameraPosition - position.xyz);
            myColor += specularIllumination(dir_to_light, pointLight[i].color, n, material.ks*material.specular_color, material.shininess, viewDir);
        }
    }

	// needed output, this should be the last line of your shader code   
    FragColor = vec4(myColor, 1.0);
}
