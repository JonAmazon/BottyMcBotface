#include "AudioSystem.h"

void AudioSink::initialize(WAVEFORMATEX * pwfx)
{
	if(pwfx->nChannels != 2){printf("Audio Sink Error :: Audio not stereo format\n");}
	if(pwfx->wBitsPerSample != 32){printf("Audio Sink Error :: Audio not 16 bit dual channel\n");}

	registerLen = (pwfx->nSamplesPerSec)/10; //100ms sample time
	cursor = 0;
	dataL = new float[registerLen];
	dataR = new float[registerLen];

	for(int i = 0; i < registerLen; ++i)
	{
		dataL[i] = 0;
		dataR[i] = 0;
	}
}

void AudioSink::copyData(BYTE * data, UINT32 nFrames)
{
	for(int i = 0; i < nFrames; ++i)
	{
		dataL[cursor%registerLen] = *(float *)&data[8*i];
		dataR[cursor%registerLen] = *(float *)&data[8*i+4];

		cursor = (cursor+1)%registerLen;
	}
}

void AudioSystem::initialize()
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
	HRESULT hr;
	REFERENCE_TIME hnsRequestDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator * pEnumerator = NULL;
	IMMDevice * pDevice = NULL;
	WAVEFORMATEX * pwfx = NULL;
	
	pCaptureClient = NULL;
	pAudioClient = NULL;

	hr = CoInitialize(NULL);
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void **)&pEnumerator);

	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL,
						   NULL, (void **)&pAudioClient);
	hr = pAudioClient->GetMixFormat(&pwfx);

	printf("Audio Format :: channels = %d\n",pwfx->nChannels);
	printf("Audio Format :: samplesPerSec = %d\n",pwfx->nSamplesPerSec);
	printf("Audio Format :: bitsPerSample = %d\n",pwfx->wBitsPerSample);
	printf("Audio Format :: byteAlign = %d\n",pwfx->nBlockAlign);
	printf("Audio Format :: wFormatTag = %d\n",pwfx->wFormatTag);

	hr = pAudioClient->Initialize(
								  AUDCLNT_SHAREMODE_SHARED,AUDCLNT_STREAMFLAGS_LOOPBACK,
								  hnsRequestDuration,0,
								  pwfx,NULL);
	hr = pAudioClient->GetService(IID_IAudioCaptureClient,
								  (void **)&pCaptureClient);

	audioSink.initialize(pwfx);

	numberOfFreq = 400;

	powerSpecL = new float[audioSink.registerLen/2];
	powerSpecR = new float[audioSink.registerLen/2];
	freqL = new float[numberOfFreq];
	freqR = new float[numberOfFreq];
	leftSpec = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex)*audioSink.registerLen);
	rightSpec = (fftwf_complex *)fftwf_malloc(sizeof(fftwf_complex)*audioSink.registerLen);
	pL = fftwf_plan_dft_1d(audioSink.registerLen,leftSpec,leftSpec,FFTW_FORWARD,FFTW_ESTIMATE);
	pR = fftwf_plan_dft_1d(audioSink.registerLen,rightSpec,rightSpec,FFTW_FORWARD,FFTW_ESTIMATE);
}

void AudioSystem::startAudioStream()
{
	pAudioClient->Start();
}

void AudioSystem::captureAudioFrames()
{
	HRESULT hr;
	UINT32 packetLength = 0;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	BYTE *data;
	DWORD flags;

	hr = pCaptureClient->GetNextPacketSize(&packetLength);
	while(packetLength)
	{
		hr = pCaptureClient->GetBuffer(&data,&numFramesAvailable,&flags,NULL,NULL);
		audioSink.copyData(data,numFramesAvailable);
		hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
	}
}

void AudioSystem::analyzeCapturedAudio()
{
	float logScale = 1.002, nextBinSize;
	int sidx,bidx,nextBinStop;
	int c = audioSink.cursor;
	int len = audioSink.registerLen;

	for(int i = 0; i < len; ++i)
	{
		leftSpec[i][0] = audioSink.dataL[(c+i)%len];
		leftSpec[i][1] = 0.0;
		rightSpec[i][0] = audioSink.dataR[(c+i)%len];
		rightSpec[i][1] = 0.0;
	}

	fftwf_execute(pL);
	fftwf_execute(pR);

	powerSpecL[0] = leftSpec[0][0]*leftSpec[0][0] + leftSpec[0][1]*leftSpec[0][1];
	powerSpecR[0] = rightSpec[0][0]*rightSpec[0][0] + rightSpec[0][1]*rightSpec[0][1];
	for(int i = 1; i < len/2; ++i)
	{
		powerSpecL[i] = 0.5*(leftSpec[i][0]*leftSpec[i][0] + leftSpec[i][1]*leftSpec[i][1] + leftSpec[len-1-i][0]*leftSpec[len-1-i][0] + leftSpec[len-1-i][1]*leftSpec[len-1-i][1]);
		powerSpecR[i] = 0.5*(rightSpec[i][0]*rightSpec[i][0] + rightSpec[i][1]*rightSpec[i][1] + rightSpec[len-1-i][0]*rightSpec[len-1-i][0] + rightSpec[len-1-i][1]*rightSpec[len-1-i][1]);
	}

	sidx = 0;
	bidx = 0;
	nextBinSize = 1.0;
	while((sidx < numberOfFreq) && (bidx < len/2))
	{
		freqL[sidx] = 1.0;
		freqR[sidx] = 1.0;

		nextBinStop = bidx + int(nextBinSize);
		while((bidx < len/2) && (bidx < nextBinStop))
		{
			freqL[sidx] += powerSpecL[bidx];
			freqR[sidx] += powerSpecR[bidx];
			bidx++;
		}

		if(freqL[sidx] > 1){freqL[sidx] = log(freqL[sidx]);}
		else{freqL[sidx] = 0;}
		if(freqR[sidx] > 1){freqR[sidx] = log(freqR[sidx]);}
		else{freqR[sidx] = 0;}

		nextBinSize *= logScale;
		sidx++;
	}
}

void AudioSystem::stopAudioStream()
{
	pAudioClient->Stop();
}