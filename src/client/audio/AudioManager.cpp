#include <client/audio/AudioManager.h>
#include <client/audio/AudioFile.h>
#include <math/MathUtil.h>
#include <util/GameSettings.h>
#include <ctime>

void AudioManager::start()
{
	shallStop = false;
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

	error = Pa_OpenDefaultStream(&opennedStream, 0, NB_CHANNEL, paFloat32, 44100, SAMPLE_COUNT, bufferCallback, this);
	if (error != paNoError)
	{
		Error("Failed to open  default sound stream: ");
		Error("Reason: " + std::string(Pa_GetErrorText(error)));
		return;
	}

	error = Pa_StartStream(opennedStream);
	if (error != paNoError)
	{
		Error("Failed to start default sound stream");
		Error("Reason: " + std::string(Pa_GetErrorText(error)));
		return;
	}

	while (!shallStop && Pa_IsStreamActive(opennedStream))
	{
		Pa_Sleep(100);
	}

	error = Pa_CloseStream(opennedStream);
	if (error != paNoError)
	{
		Error("Failed to close default sound stream");
		Error("Reason: " + std::string(Pa_GetErrorText(error)));

		return;
	}

	Info("Terminating audio...");
	Pa_Terminate();
}

void AudioManager::stop()
{
	shallStop = true;
}

void AudioManager::playSound(AudioFile& audioFile)
{
	std::lock_guard<std::mutex> lock(inputMutex);

	BufferData fileData;

	fileData.file = sf_open(audioFile.getFilePath().c_str(), SFM_READ, &fileData.info);
	if (sf_error(fileData.file) != SF_ERR_NO_ERROR)
	{
		Error("Failed to load sound file: " + audioFile.getFilePath());
		Error("Reason: " + std::string(sf_strerror(fileData.file)));

		sf_close(fileData.file);
		return;
	}

	playingAudio.push_back(fileData);
}

AudioManager & AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

int AudioManager::bufferCallback(const void * input, void * output, unsigned long frameCount, const PaStreamCallbackTimeInfo * timeInfo, PaStreamCallbackFlags statusFlags, void * userData)
{
	AudioManager* audioManager = (AudioManager*) userData;

	float* out;
	out = (float*)output;

	memset(out, 0, sizeof(float) * SAMPLE_COUNT * NB_CHANNEL);

	float audioBuffer[NB_CHANNEL * SAMPLE_COUNT];
	std::vector<BufferData>::iterator it;

	// For all currently playing audio files
	for (it = audioManager->playingAudio.begin(); it != audioManager->playingAudio.end();)
	{
		sf_count_t sampleRead = sf_read_float(it->file, audioBuffer, SAMPLE_COUNT * it->info.channels);
		for (int i = 0; i < sampleRead; i++)
		{
			out[i] += audioBuffer[i] * GameSettings::volume;
		}

		// If file is readed entirely
		if (sampleRead < SAMPLE_COUNT)
		{
			std::lock_guard<std::mutex> lock(audioManager->inputMutex);

			// Remove & close it
			sf_close(it->file);
			it = audioManager->playingAudio.erase(it);
		}
		else
		{
			it++;
		}
	}

	return paContinue;
}
