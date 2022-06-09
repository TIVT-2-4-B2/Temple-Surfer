#include "OBJComponent.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <mutex>



/**
* Replaces a substring in a string
*/
static std::string replace(std::string str, const std::string& toReplace, const std::string& replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

/**
* Splits a string into substrings, based on a seperator
*/
static std::vector<std::string> split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

/**
* Turns a string to lowercase
*/
static inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}


/**
* Cleans up a line for processing
*/
static inline std::string cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}

/*Loads the objectfile and adds it to the list of objects for the animation*/
void OBJComponent::loadObjectFile(const std::string fileName, std::shared_ptr<ObjectBuilder> context, int listIndex)
{
	// Checking wheter the file actually exists
	/*std::cout << "Loading " << fileName << std::endl;*/
	std::string dirName = fileName;
	if (dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if (dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if (fileName == dirName)
		dirName = "";

	// Opening file
	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	// The object groups.
	std::shared_ptr<ObjectGroup> currentGroup = std::make_shared<ObjectGroup>();
	currentGroup->materialIndex = -1;

	// Information for building the object
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<tigl::Vertex> renderData;
	std::shared_ptr<ObjectFile> file = std::make_shared<ObjectFile>();

	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if (line == "" || line[0] == '#') //skip empty or commented line
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		// Location coordinate
		if (params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		// Normal coordinate
		else if (params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		// Texture coordinate
		else if (params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), 1 - (float)atof(params[2].c_str())));

		// Structure data
		else if (params[0] == "f")
		{
			for (size_t ii = 4; ii <= params.size(); ii++)
			{
				for (size_t i = ii - 3; i < ii; i++)
				{
					int position;
					int normal;
					int texcoord = -1;
					std::vector<std::string> indices = split(params[i == (ii - 3) ? 1 : i], "/");
					if (indices.size() >= 1)	//er is een positie
						position = atoi(indices[0].c_str()) - 1;
					if (indices.size() == 2)		//alleen texture
						texcoord = atoi(indices[1].c_str()) - 1;
					if (indices.size() == 3)		//v/t/n of v//n
					{
						if (indices[1] != "")
							texcoord = atoi(indices[1].c_str()) - 1;
						normal = atoi(indices[2].c_str()) - 1;
					}
					renderData.push_back(tigl::Vertex::PCTN(vertices.at(position), file->materials.at(currentGroup->materialIndex)->diffuse, (texcoord == -1? glm::vec3(0): texcoords.at(texcoord)), normals.at(normal)));
				}
			}
		}
		else if (params[0] == "mtllib")
		{
			loadMaterialFile(dirName + "/" + params[1], dirName, file, context);
		}
		else if (params[0] == "usemtl")
		{
			if (renderData.size() > 0) {
				currentGroup->bufferedObjectVertices = context->asyncObjectVBOCall(renderData);
				if (currentGroup->bufferedObjectVertices != nullptr) {
					file->groups.push_back(currentGroup);
					renderData.clear();
				}
			}

			currentGroup = std::make_shared<ObjectGroup>();
			currentGroup->materialIndex = -1;

			for (size_t i = 0; i < file->materials.size(); i++)
			{
				std::shared_ptr<MaterialInfo> info = file->materials.at(i);
				if (info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if (currentGroup->materialIndex == -1)
				std::cout << "Could not find material name " << params[1] << std::endl;
		}
	}

	if (renderData.size() > 0) {
		currentGroup->bufferedObjectVertices = context->asyncObjectVBOCall(renderData);
		if (currentGroup->bufferedObjectVertices != nullptr) {
			file->groups.push_back(currentGroup);
			renderData.clear();
		}
	}



	// File is done
	objectDataLock.lock();
	file->animationIndex = listIndex;
	objectData.push_back(file);
	objectDataLock.unlock();

	amountWorkers--;

	// Printing debug information 
	/*std::cout << "Amount of vertices: " << vertices.size() << std::endl;
	std::cout << "Amount of normals: " << normals.size() << std::endl;
	std::cout << "Amount of textures: " << file->materials.size() << std::endl;
	std::cout << "Amount of texcoords: " << texcoords.size() << std::endl;
	std::cout << "Amount of groups: " << file->groups.size() << std::endl;*/
}

/**
* Reads a material file
*/
void OBJComponent::loadMaterialFile(const std::string& fileName, const std::string& dirName, std::shared_ptr<ObjectFile>& file, std::shared_ptr<ObjectBuilder> context)
{
	//std::cout << "Loading " << fileName << std::endl;
	std::ifstream pFile(fileName.c_str());
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	std::shared_ptr<MaterialInfo> currentMaterial = NULL;

	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if (line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if (params[0] == "newmtl")
		{
			if (currentMaterial != NULL)
			{
				file->materials.push_back(currentMaterial);
			}
			currentMaterial = std::make_shared<MaterialInfo>();
			currentMaterial->name = params[1];
		}
		else if (params[0] == "map_kd")
		{
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);
			if (currentMaterial != NULL) {
				currentMaterial->texture = context->asyncObjectTextureCall(dirName + "/" + tex);
			}
		}
		else if (params[0] == "kd")
		{
			currentMaterial->diffuse = glm::vec4((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str()), 1.0f);
		}
		else if (params[0] == "ka")
		{
			currentMaterial->ambient = glm::vec4((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str()), 1.0f);
		}
		else if (params[0] == "ks")
		{
			currentMaterial->specular = glm::vec4((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str()), 1.0f);
		}
		else if (
			params[0] == "illum" ||
			params[0] == "map_bump" ||
			params[0] == "map_ke" ||
			params[0] == "map_ka" ||
			params[0] == "map_d" ||
			params[0] == "d" ||
			params[0] == "ke" ||
			params[0] == "ns" ||
			params[0] == "ni" ||
			params[0] == "td" ||
			params[0] == "tf" ||
			params[0] == "tr" ||
			false)
		{
			//these values are usually not used for rendering at this time, so ignore them
		}
		else
			std::cout << "Didn't parse " << params[0] << " in material file" << std::endl;
	}
	if (currentMaterial != NULL)
		file->materials.push_back(currentMaterial);
}


OBJComponent::OBJComponent(const std::string& fileName)
{
	std::shared_ptr<ObjectBuilder> build = std::make_shared<ObjectBuilder>();
	amountWorkers = 1;
	std::thread thread(&OBJComponent::loadObjectFile, this, fileName, build, 0);

	// Awaiting vbo add calls.
	while (amountWorkers > 0) {
		build->awaitObjectGLCall();
	}

	thread.join();
}


OBJComponent::OBJComponent(const std::string& folderName, float animationDelayIn)
{
	// Setting variables
	animationDelay = animationDelayIn;
	amountWorkers = 0;

	// Booting up threads
	std::vector<std::shared_ptr<ObjectBuilder>> builders;
	std::vector<std::thread> threads;
	int index = 0;
	for (const auto& entry : std::filesystem::directory_iterator(folderName)) {
		if (entry.path().extension().string()._Equal(".obj")){
			// Setting up thread
			amountWorkers++;
			std::shared_ptr<ObjectBuilder> build = std::make_shared<ObjectBuilder>();
			builders.push_back(build);
			threads.push_back(std::thread(&OBJComponent::loadObjectFile, this, entry.path().string(), build, index));
			index++;
		}
	}

	// Awaiting gl add calls.
	while (amountWorkers > 0) {
		for (std::shared_ptr<ObjectBuilder> b : builders) {
			b->awaitObjectGLCall();
		}
	}

	// Joining threads;
	for (std::thread& t : threads) {
		t.join();
	}
}

OBJComponent::~OBJComponent()
{
}

void OBJComponent::draw()
{
	if (objectData.size() == 1) objectDrawer(objectData.at(0));
	else {
		for (std::shared_ptr<ObjectFile> file : objectData) {
			if (file->animationIndex == animationIndex) {
				objectDrawer(file);
			}
		}
	}
}

void OBJComponent::objectDrawer(std::shared_ptr<ObjectFile> file) {
	// Looping through list of obj-groups
	for (std::shared_ptr<ObjectGroup> group : file->groups) {
		if (file->materials.at(group->materialIndex)->texture != nullptr) {
			// Enabling textures because standard disabled
			tigl::shader->enableTexture(true);
			file->materials.at(group->materialIndex)->texture->bind();
		}
		if (group->bufferedObjectVertices != nullptr)
			tigl::drawVertices(GL_TRIANGLES, group->bufferedObjectVertices);
		tigl::shader->enableTexture(false);
	}

	// Disabling textures else components with colors will get textures
	tigl::shader->enableTexture(false);
}

void OBJComponent::update(float elapsedTime)
{
	if ((animationTime += elapsedTime) > animationDelay) {
		//std::cout << animationIndex << std::endl;
		animationIndex++;
		if (animationIndex >= objectData.size()) {
			animationIndex = 0;
		}
		
	}
}

OBJComponent::MaterialInfo::MaterialInfo()
{
	texture = NULL;
}

tigl::VBO* OBJComponent::ObjectBuilder::asyncObjectVBOCall(std::vector<tigl::Vertex> vertices)
{
	buildLock.lock();
	vboResponse = nullptr;
	verticesRequest = vertices;
	inputGiven = true;
	outputGiven = false;
	operation = 0;
	buildLock.unlock();

	while (true) {
		buildLock.lock();
		if (outputGiven) {
			inputGiven = false;
			outputGiven = false;
			buildLock.unlock();
			return vboResponse;
		}
		buildLock.unlock();

		// Sleep to give other threads time to edit.
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}

std::shared_ptr<TextureComponent> OBJComponent::ObjectBuilder::asyncObjectTextureCall(std::string path)
{
	buildLock.lock();
	textureResponse = nullptr;
	pathRequest = path;
	inputGiven = true;
	outputGiven = false;
	operation = 1;
	buildLock.unlock();

	while (true) {
		buildLock.lock();
		if (outputGiven) {
			inputGiven = false;
			outputGiven = false;
			buildLock.unlock();
			return textureResponse;
		}
		buildLock.unlock();

		// Sleep to give other threads time to edit.
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}

void OBJComponent::ObjectBuilder::awaitObjectGLCall()
{
	buildLock.lock();
	if (inputGiven) {
		if (operation == 0) 
		{
			vboResponse = tigl::createVbo(verticesRequest);
		}
		else if (operation == 1) 
		{
			textureResponse = std::make_shared<TextureComponent>(pathRequest);
		}
		inputGiven = false;
		outputGiven = true;
	}
	buildLock.unlock();

	// Sleep to give other threads time to edit.
	std::this_thread::sleep_for(std::chrono::microseconds(50));
}
