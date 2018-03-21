#ifndef CLIENT_AUDIO_AUDIOMANAGER_H_
#define CLIENT_AUDIO_AUDIOMANAGER_H_

#include <portaudio/portaudio.h>
#include <libsndfile/sndfile.h>
#include <util/Logger.h>
#include <thread>
#include <vector>
#include <mutex>

#define NB_CHANNEL 2
#define SAMPLE_COUNT 512

class AudioFile;

struct BufferData
{
	SNDFILE* file;
	SF_INFO info;
};

class AudioManager
{
private:
	std::thread* audioThread;
	PaStream* opennedStream;
	std::vector<BufferData> playingAudio;
	std::mutex inputMutex;
	bool shallStop;

public:
	void start();
	void stop();

	void playSound(AudioFile& audioFile);

	static AudioManager& getInstance();

private:
	void onThreadStart();
	static int bufferCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
};

#endif