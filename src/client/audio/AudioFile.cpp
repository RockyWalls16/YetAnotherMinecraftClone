#include <client/audio/AudioFile.h>

AudioFile::AudioFile(const std::string & filePath) : filePath(AUDIO_PATH + filePath)
{
}

const std::string & AudioFile::getFilePath()
{
	return filePath;
}
