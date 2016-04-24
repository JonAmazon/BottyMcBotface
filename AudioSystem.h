#ifndef _H_AUDIOSYSTEM
#define _H_AUDIOSYSTEM

#include <cmath>
#include <cstdio>
#include <Windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <Audiopolicy.h>
#include "fftw3.h"

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC 10000

struct AudioSink
{
	int cursor;
	int registerLen;
	float * dataL;
	float * dataR;

	void initialize(WAVEFORMATEX * pwfx);
	void copyData(BYTE * data, UINT32 nFrames);
};

class AudioSystem
{
private:
	int numberOfFreq;
	float * powerSpecL, * powerSpecR;
	float * freqL, * freqR;
	fftwf_complex * leftSpec, * rightSpec;
	fftwf_plan pL,pR;

	IAudioClient * pAudioClient;
	IAudioCaptureClient * pCaptureClient;

	AudioSink audioSink;
public:
	void initialize();
	void startAudioStream();
	void captureAudioFrames();
	void analyzeCapturedAudio();
	void stopAudioStream();

	AudioSink * getAudioSink(){return &audioSink;}
	int getNumberOfFreq(){return numberOfFreq;}
	float * getLeftSpectrum(){return freqL;}
	float * getRightSpectrum(){return freqR;}
};

#endif