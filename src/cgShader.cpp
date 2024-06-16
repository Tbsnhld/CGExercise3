/** \file cgShader.cpp
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
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include "cgShader.h"


void cgShader::deleteOpenGLShaders()
{
	if(m_bIsCompiledAndLinked)
	{
		endShader();

		m_bIsCompiledAndLinked = false;
		glDetachShader(m_ProgramID, m_VertexShaderID);
		glDetachShader(m_ProgramID, m_FragmentShaderID);

		glDeleteShader(m_VertexShaderID);
		glDeleteShader(m_FragmentShaderID);
		glDeleteProgram(m_ProgramID);
		m_ProgramID = 0;
		m_VertexShaderID = 0;
		m_FragmentShaderID = 0;
	}
}

bool cgShader::enableShader() const
{
	if(!m_bIsCompiledAndLinked)
		return false;

	glUseProgram(m_ProgramID);
	return true;
}

void cgShader::endShader() const
{
	glUseProgram(0);
}

bool cgShader::LoadShadersFromFile(std::string vertexShaderFilename,
		std::string fragmentShaderFilename) {
	std::string vertexShaderProg = loadFromTextFile(vertexShaderFilename);
	std::string fragmentShaderProg = loadFromTextFile(fragmentShaderFilename);
	// safe for error messages
	m_vertexShaderFilename = vertexShaderFilename;
	m_fragmentShaderFilename = fragmentShaderFilename;

	return LoadShadersFromStrings(vertexShaderProg, fragmentShaderProg);
}

bool cgShader::LoadShadersFromStrings(std::string vertexShaderSource,
		std::string fragmentShaderSource) {

	// delete current shader and program
	deleteOpenGLShaders();

	GLint succes;
	// Compile Vertex Shader
	m_VertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
	glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		std::cerr << "***COMPILER ERROR (Vertex Shader):\n";
		printCompilerLog(m_VertexShaderID);
		return false;
	}

	// Compile Fragment Shader
	m_FragmentShaderID = compileShader(GL_FRAGMENT_SHADER,
			fragmentShaderSource);
	glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &succes);
	if (!succes) {
		std::cerr << "***COMPILER ERROR (Fragment Shader):\n";
		printCompilerLog(m_FragmentShaderID);
		return false;
	}

	// Link the shader programm
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	glLinkProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &succes);
	if (!succes) {
		std::cerr << "***Linker ERROR (Shader Program):\n";
		printLinkerLog(m_ProgramID);
		return false;
	}

	m_bIsCompiledAndLinked = true;

	return true;
}

std::string cgShader::loadFromTextFile(std::string filename) {
	std::string codeString = "";
	std::ifstream inFile(filename, std::ios::in);
	if(inFile.is_open()){
		std::stringstream sstr;
		sstr << inFile.rdbuf();
		codeString = sstr.str();
		inFile.close();
	}else{
		std::cerr << "Unable to Open Shader File " << filename << "\n";
		throw 2;
	}
	return codeString;
}

GLuint cgShader::compileShader(GLenum shaderType,
		std::string shaderSource) {
	GLuint shader = glCreateShader(shaderType);
	const char * cstr = shaderSource.c_str();
	glShaderSource(shader, 1, &cstr, NULL);
	glCompileShader(shader);
	return shader;
}

void cgShader::printCompilerLog(const GLint shader) {
	GLint length;
	GLchar * log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		log = new GLchar[length + 1];
		glGetShaderInfoLog(shader, length, &length, log);
		std::cout << "Compiler Log:\n" << log << "\n";
		delete[] log;
	} else
		std::cout << "No log." << "\n";
}

void cgShader::printLinkerLog(const GLint program) {
	GLint length;
	GLchar * log;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		log = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, &length, log);
		std::cout << log << "\n";
		delete[] log;
	} else
		std::cout << "No log." << "\n";

}

void cgShader::printShaderInfo() {
	if(IsCompiledAndLinked()) {
		std::cout << "Shader is compiled and linked."<< "\n";
		std::cout << "     shader ids: "<< m_ProgramID<<", "<< m_VertexShaderID<<", "<< m_FragmentShaderID<< "\n";
	}
	else {
		std::cout << "Shader is NOT compiled and linked."<< "\n";
	}
	std::cout << "     vertex shader file: "<< m_vertexShaderFilename<< "\n";
	std::cout << "   fragment shader file: "<< m_fragmentShaderFilename<< "\n";
}


// ------------------------------------------------------------------------
bool cgShader::checkUniformVariable(const std::string &name) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	return (loc >= 0);
}

// ------------------------------------------------------------------------
void cgShader::setUniformBool(const std::string &name, bool value) const
{
    setUniformInt(name, (int)value);
}
// ------------------------------------------------------------------------
void cgShader::setUniformInt(const std::string &name, int value) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "int");
	else
		glUniform1i(loc, value);
}
// ------------------------------------------------------------------------
void cgShader::setUniformFloat(const std::string &name, float value) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "float");
	else
		glUniform1f(loc, value);
}
// ------------------------------------------------------------------------
void cgShader::setUniformVec2(const std::string &name, const glm::vec2 &value) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "vec2");
	else
		glUniform2fv(loc, 1, glm::value_ptr(value));
}
// ------------------------------------------------------------------------
void cgShader::setUniformVec3(const std::string &name, const glm::vec3 &value) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "vec3");
	else
		glUniform3fv(loc, 1, glm::value_ptr(value));
}
// ------------------------------------------------------------------------
void cgShader::setUniformVec4(const std::string &name, const glm::vec4 &value) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "vec4");
	else
		glUniform4fv(loc, 1, glm::value_ptr(value));
}
// ------------------------------------------------------------------------
void cgShader::setUniformMat3(const std::string &name, const glm::mat3 &mat) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "mat3");
	else
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
// ------------------------------------------------------------------------
void cgShader::setUniformMat4(const std::string &name, const glm::mat4 &mat) const
{
	GLint loc = glGetUniformLocation(m_ProgramID, name.c_str());
	if(loc < 0)
		printSetUniformError(name, "mat4");
	else
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}
// ------------------------------------------------------------------------
void cgShader::printSetUniformError(const std::string &variable_name, const std::string &type_name) const
{
	std::cerr<<"ERROR: variable "<<variable_name<<" of type "<<type_name<<" does not exist in shader sources!"<<std::endl;
	if(!glIsProgram(m_ProgramID))
	{
		std::cerr<<"ERROR: Shaderprogram is not compiled/linked or deleted!"<<std::endl;
	}
	else
		{
		GLint currProg;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currProg);
		if(currProg != (GLint)m_ProgramID) {
			std::cerr<<"ERROR: Shaderprogram is not active! Use enableShader()!"<<std::endl;
		}
		}
	std::cerr<<"  Vertex shader source file: "<<m_vertexShaderFilename<<std::endl;
	std::cerr<<"Fragment shader source file: "<<m_fragmentShaderFilename<<std::endl;
}
