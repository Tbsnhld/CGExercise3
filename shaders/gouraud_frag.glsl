#version 330 core                                                                       
// Do not modify the above version directive to anything older.                                                                  

// Do not modify this code. Gouraud shading is implemented in the
// vertex shader.

// input variables from vertex shader:
// color computed by Gouraud shading in RGB
// note that values are interpolated for all fragments in the rasterizer
in vec3 myColor;                                                                    
                                                                                    
// NEEDED: output variable: the fragment color in RGBA
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                                 
{                
	// here we set the interpolated color from vertex shader
	// as final output color (as RGBA)                                                                           
	FragColor = vec4(myColor,1.0);
}
