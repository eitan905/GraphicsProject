#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Utils.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		//std::cout << curLine << std::endl;

		// based on the type parse data
		if (lineType == "v")
		{
			
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	/*for (int i = 0; i < faces.size(); i++) {
		std::cout << "faces i: "<<i<<" 0: "<<faces[i].GetVertexIndex(0) <<" 1: "<< faces[i].GetVertexIndex(1)<<" 2: " <<faces[i].GetVertexIndex(2)<<   std::endl;   // problem line
	}
	for (int j = 0; j < vertices.size(); j++) {
		std::cout << "vertices j: " << j << " 0: " << vertices[j][0] << " 1: " << vertices[j][1] << " 2: " << vertices[j][2] << std::endl;   // problem line

	}*/
	glm::mat4x4 m = glm::mat4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		700, 300, 500, 1
	);
	glm::mat3x3 s = glm::mat3x3(
		400, 0, 0, 
		0, 400, 0, 
		0, 0, 400 
	);
	glm::vec3 b;
	for (int j = 0; j < vertices.size(); j++) {
		b = s * vertices[j]; 
		vertices[j] = b;
	}
	//-------------------------
	/*for (int j = 0; j < vertices.size(); j++) {
		std::cout << "vertices num: " << j << " 0: " << vertices[j][0] << " 1: " << vertices[j][1] << " 2: " << vertices[j][2] << std::endl;   // problem line
	}*/
	//------------------------------
	glm::vec4 a;
	std::vector <glm::vec4> vertices_4;
	std::vector <glm::vec4> ans;

	for (int j = 0; j < vertices.size(); j++) {
		a = { vertices[j][0] ,vertices[j][1] ,vertices[j][2] ,1 };
		vertices_4.push_back(a);
	}
	
	for (int j = 0; j < vertices_4.size(); j++) {
		a = m * vertices_4[j];
		ans.push_back(a);
	}

	for (int j = 0; j < ans.size(); j++) {
		vertices[j][0] = ans[j][0];
		vertices[j][1] = ans[j][1];
		vertices[j][2] = ans[j][2];

	}
	/*for (int j = 0; j < vertices.size(); j++) {
		std::cout << "vertices numnum: " << j << " 0: " << vertices[j][0] << " 1: " << vertices[j][1] << " 2: " << vertices[j][2] << std::endl;   // problem line
	}*/

	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}