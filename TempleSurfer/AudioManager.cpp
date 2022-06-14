#include "AudioManager.h"

void AudioManager::initAudio()
{
	int device = -1; // Default Sounddevice
	int freq = 44100; // Sample rate (Hz)
	BASS_Init(device, freq, 0, 0, NULL);
}

void AudioManager::loopMusic(std::string path)
{
	HSTREAM music;
	if (IDMap.contains(path)) music = IDMap.at(path);
	else music = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(music, TRUE);
}

void AudioManager::stopMusic(std::string path)
{
	BASS_ChannelStop(IDMap.at(path));
}

void AudioManager::onceMusic(std::string path)
{
	HSTREAM music;
	if (IDMap.contains(path)) music = IDMap.at(path);
	else music = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, 0);
	BASS_ChannelPlay(music, FALSE);
}

void AudioManager::killMusic()
{
	for (auto const& [key, val] : IDMap) {
		BASS_ChannelStop(val);
	}
}

std::shared_ptr<AudioManager> AudioManager::instanceAudio()
{
	static std::shared_ptr<AudioManager> s{ new AudioManager };
	return s;
}