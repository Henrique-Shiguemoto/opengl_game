#include "Shader.h"

Shader::Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath){
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	
	vShaderFile.open(vertexShaderFilepath);
	fShaderFile.open(fragmentShaderFilepath);
	
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	
	vShaderFile.close();
	fShaderFile.close();
	
	std::string vertexCode = vShaderStream.str();
	std::string fragmentCode = fShaderStream.str();
	
	const char* vertexShaderSourceCode = vertexCode.c_str();
	const char* fragmentShaderSourceCode = fragmentCode.c_str();
	
	u32 vertexShaderID, fragmentShaderID;
	i32 success;
	i8 infoLog[512];

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, (char const * const *)&vertexShaderSourceCode, NULL);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "Failed to compile vertex shader: " << infoLog << std::endl;
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (char const * const *)&fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "Failed to compile fragment shader: " << infoLog << std::endl;
	}

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShaderID);
	glAttachShader(this->id, fragmentShaderID);
	glLinkProgram(this->id);
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "Failed to link shader program: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::Use(){
	glUseProgram(id);
}

void Shader::Delete(){
	glDeleteProgram(id);
}

void Shader::SetBool(const std::string& name, b8 value) const{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (i32)value);
}

void Shader::SetInt(const std::string& name, i32 value) const{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

