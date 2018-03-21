#include <client/audio/AudioManager.h>
#include <math/MathUtil.h>
#include <ctime>

void AudioManager::start()
{
	audioThread = new std::thread(&AudioManager::onThreadStart, this);
}


void AudioManager::onThreadStart()
{
	Info("Audio manager initializing...");

	PaError error = Pa_Initialize();
	if (error != paNoError)
	{
		Error("Audio manager failed...");
		Error("Reason: " + std::string(Pa_GetErrorText(error)));
	}

	std::srand((unsigned int)std::time(nullptr));
	loadAudioFile("Assets/Audio/music-" + std::to_string(MathUtil::rand(0, 12)) + ".ogg");
}


void AudioManager::cleanup()
{
	Info("Terminating audio...");
	Pa_Terminate();
}

void AudioManager::loadAudioFile(const std::string& filePath)
{
	Info("Playing " + filePath);

	BufferData fileData;

	fileData.file = sf_open(filePath.c_str(), SFM_READ, &fileData.info);
	if (sf_error(fileData.file) != SF_ERR_NO_ERROR)
	{
		Error("Failed to load sound file: " + filePath);
		Error("Reason: " + std::string(sf_strerror(fileData.file)));

		sf_close(fileData.file);
		return;
	}

	PaStream *stream;
	PaError error = Pa_OpenDefaultStream(&stream, 0, fileData.info.channels, paFloat32, fileData.info.samplerate, 512, bufferCallback, &fileData);
	if (error != paNoError)
	{
		Error("Failed to open  default sound stream: " + filePath);
		Error("Reason: " + std::string(Pa_GetErrorText(error)));

		sf_close(fileData.file);
		return;
	}

	error = Pa_StartStream(stream);
	if (error != paNoError)
	{
		Error("Failed to start default sound stream: " + filePath);
		Error("Reason: " + std::string(Pa_GetErrorText(error)));

		sf_close(fileData.file);
		return;
	}

	while (Pa_IsStreamActive(stream))
	{
		Pa_Sleep(100);
	}

	sf_close(fileData.file);

	error = Pa_CloseStream(stream);
	if (error != paNoError)
	{
		Error("Failed to close default sound stream: " + filePath);
		Error("Reason: " + std::string(Pa_GetErrorText(error)));

		return;
	}
}

AudioManager & AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

int AudioManager::bufferCallback(const void * input, void * output, unsigned long frameCount, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
{
	float *out;
	BufferData *p_data = (BufferData*)userData;
	sf_count_t num_read;

	out = (float*)output;
	p_data = (BufferData*)userData;

	/* clear output buffer */
	memset(out, 0, sizeof(float) * frameCount * p_data->info.channels);

	/* read directly into output buffer */
	num_read = sf_read_float(p_data->file, out, frameCount * p_data->info.channels);

	/*  If we couldn't read a full frameCount of samples we've reached EOF */
	if (num_read < frameCount)
	{
		return paComplete;
	}

	return paContinue;
}
