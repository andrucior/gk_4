#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	
	// Activates the Shader Program
	void Activate();

	// Deletes the Shader Program
	void Delete();

	// Helper functions for variables setting
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix4(const std::string& name, const glm::mat4& matrix);
	void setVec4(const std::string& name, const glm::vec4& value);
	void setInt(const std::string& name, int value);

private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};


