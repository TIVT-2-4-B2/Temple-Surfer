#pragma once
#include <bass.h>
#include <string>
#include <map>
#include <memory>

class AudioManager {
public:
	void initAudio();
	void loopMusic(std::string path);
	void stopMusic(std::string path);
	void onceMusic(std::string path);
	void killMusic();

	static std::shared_ptr<AudioManager> instance();
private:
	std::map<std::string, HSTREAM> IDMap;
};