#ifndef CLIENT_AUDIO_AUDIOFILE_H_
#define CLIENT_AUDIO_AUDIOFILE_H_

#include <string>

#define AUDIO_PATH "Assets/Audio/"

class AudioFile
{
private:
	std::string filePath;

public:
	AudioFile(const std::string& filePath);
	const std::string& getFilePath();
};

#endif