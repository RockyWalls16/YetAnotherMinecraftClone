#ifndef CLIENT_AUDIO_AUDIOMANAGER_H_
#define CLIENT_AUDIO_AUDIOMANAGER_H_

#include <portaudio/portaudio.h>
#include <libsndfile/sndfile.h>
#include <util/Logger.h>

struct BufferData
{
	SNDFILE* file;
	SF_INFO info;
};

class AudioManager
{
public:
	void init();
	void cleanup();

	void loadAudioFile(const std::string& filePath);

	static AudioManager& getInstance();

	static int bufferCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
};

#endif