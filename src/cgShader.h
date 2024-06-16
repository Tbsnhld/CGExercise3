/** \file cgShader.h
 *
 * Module:    CS3205
 *            Computergrafik
 *
 * Authors:   Jan Ehrhardt
 *
 * Copyright (c) 2020 Institute of Medical Informatics,
 *            University of Luebeck
 ********************************************************/
#ifndef _CGSHADER_H_
#define _CGSHADER_H_

#include <string>

// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>      // GLEW provids OpenGL headers in
						  // a platform independent way
#endif

#include <glm/glm.hpp>

/*! \brief This class is an interface to GLSL shaders.
 *
 *  This class loads, compiles, runs GLSL shaders. You can load and compile
 *  GLSL vertex and fragment shaders from files with LoadShadersFromFile().
 *  Use enableShader() to use a compiled shader in OpenGL.
 *  To set uniform variables in the shaders use setUniformX() methods.
 */
class cgShader {
public:
	cgShader() = default;

	bool LoadShadersFromFile(std::string vertexShaderFilename, std::string fragmentShaderFilename);
	bool LoadShadersFromStrings(std::string vertexShaderSource, std::string fragmentShaderSource);

	bool IsCompiledAndLinked() const {return m_bIsCompiledAndLinked;};

	// enable shader program for OpenGL drawings
	bool enableShader() const;
	// disable shader program
	void endShader() const;

	// call this function to delete shaders in openGL
	void deleteOpenGLShaders();

	GLuint GetProgramID() const {return m_ProgramID;};
	GLuint GetVertexShaderID() const {return m_VertexShaderID;};
	GLuint GetFragmentShaderID() const {return m_FragmentShaderID;};

	static void printCompilerLog(const GLint shader);
	static void printLinkerLog(const GLint program);
	void printShaderInfo();

	// check if a uniform variable is present
	bool checkUniformVariable(const std::string &name) const;
	// setUniform functions to set OpenGL variables (with error messages)
    void setUniformBool(const std::string &name, bool value) const;
    void setUniformInt(const std::string &name, int value) const;
    void setUniformFloat(const std::string &name, float value) const;
    void setUniformVec2(const std::string &name, const glm::vec2 &value) const;
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
    void setUniformVec4(const std::string &name, const glm::vec4 &value) const;
    void setUniformMat3(const std::string &name, const glm::mat3 &mat) const;
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;

protected:
	static std::string loadFromTextFile(std::string filename);
	static GLuint compileShader(GLenum shaderType, std::string shaderSource);
	// print error message for setUniform.. methods
	void printSetUniformError(const std::string &variable_name, const std::string &type_name) const;

	bool m_bIsCompiledAndLinked = false;
	GLuint m_ProgramID = 0;
	GLuint m_VertexShaderID = 0;
	GLuint m_FragmentShaderID = 0;
	std::string m_vertexShaderFilename = "";
	std::string m_fragmentShaderFilename = "";
};

#endif /* _CGSHADER_H_ */
