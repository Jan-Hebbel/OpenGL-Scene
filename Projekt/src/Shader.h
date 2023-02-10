#pragma once

#include <string>

#include <glm/glm.hpp>

constexpr auto VERTEX_SHADER = 0;
constexpr auto FRAGMENT_SHADER = 1;
constexpr auto TESSELATION_CONTROL_SHADER = 2;
constexpr auto TESSELATION_EVAL_SHADER = 3;
constexpr auto GEOMETRY_SHADER = 4;
constexpr auto COMPUTE_SHADER = 5;
constexpr auto MAX_AMOUNT_OF_SHADERS = 6;

class Shader
{
private:
	unsigned int mProgramID;
	unsigned int mShaderIDs[6];
public: 
	Shader();
	~Shader();

	void Create();
	void AddShader(const std::string& filePath, const int type);
	void LinkShader();

	void Bind() const;
	void Unbind() const;

	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const glm::mat4 &matrix) const;
	void SetVec3(const std::string &name, const glm::vec3 &vector) const;
private:
	std::string GetShaderSource(const std::string&) const;
	unsigned int GetIndex(int type) const;
	void CheckCompileErrors(unsigned int shader, const std::string& type);
	std::string GetTypeAsString(int type) const;
};