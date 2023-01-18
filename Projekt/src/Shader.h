#pragma once

#include <string>

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

	void AddShader(const std::string& filePath, const int type);
	void LinkShader();

	void Bind();
	void Unbind();

	void SetInt(unsigned int id, int value);
private:
	std::string GetShaderSource(const std::string&) const;
	unsigned int GetIndex(int type) const;
	void CheckCompileErrors(unsigned int shader, const std::string& type);
	std::string GetTypeAsString(int type) const;
};