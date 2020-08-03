#include "Shader.h"



Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vertShaderFile;
	std::ifstream fragShaderFile;

	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertShaderFile.open(vertexPath);
		fragShaderFile.open(fragmentPath);
		std::stringstream vertShaderStream, fragShaderStream;

		vertShaderStream << vertShaderFile.rdbuf();
		fragShaderStream << fragShaderFile.rdbuf();

		vertShaderFile.close();
		fragShaderFile.close();

		vertexCode = vertShaderStream.str();
		fragmentCode = fragShaderStream.str();
	} 
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
	}

	const char* vertShaderCode = vertexCode.c_str();
	const char* fragShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\N" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\N" << infoLog << std::endl;
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\N" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(m_id);
}

void Shader::SetBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), int(value));
}

void Shader::SetInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetMatrix(const std::string & name, glm::mat4 value) const
{
	unsigned int transformLocation = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(value));
}
