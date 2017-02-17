////////////////////////////////////////////////
//DSound.cpp
////////////////////////////////////////////////

#include "DSound.h"



CDSound * g_pDSound;


int CDSound::LoadWAV(TCHAR *filename, INT nFlags)
{
	HMMIO 			hwav;    // handle to wave file
	MMCKINFO		parent,  // parent chunk
		            child;   // child chunk
	WAVEFORMATEX    wfmtx;   // wave format structure

	int	sound_id = -1,
		index;

	UCHAR *snd_buffer,         // temporary sound buffer to hold voc data
		  *audio_ptr_1 = NULL, // data ptr to first write buffer 
		  *audio_ptr_2 = NULL; // data ptr to second write buffer

	DWORD audio_length_1 = 0,  // length of first write buffer
		  audio_length_2 = 0;  // length of second write buffer

	for (index=0; index < MAX_SOUNDS; index++)
	{	
		if (m_Sound[index].state == SOUND_NULL)
		{
			sound_id = index;
			break;
		}
	}
	if (sound_id == -1)
		return -1;

	parent.ckid 	    = (FOURCC)0;
	parent.cksize 	    = 0;
	parent.fccType	    = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	child = parent;

	if ((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL)
		return -1;

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
	{
		mmioClose(hwav, 0);
		return -1; 	
	}

	child.ckid = mmioFOURCC('f', 'm', 't', ' ');

	if (mmioDescend(hwav, &child, &parent, 0))
	{
		mmioClose(hwav, 0);
		return -1; 	
	}

	if (mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx))
	{
		mmioClose(hwav, 0);
		return -1;
	}
	
	if (wfmtx.wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(hwav, 0);
		return -1; 
	}

	if (mmioAscend(hwav, &child, 0))
	{
		mmioClose(hwav, 0);
		return -1; 	
	}

	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
	{
		mmioClose(hwav, 0);
		return -1; 	
	}

	snd_buffer = (UCHAR *)malloc(child.cksize);

	// read the wave data 
	mmioRead(hwav, (char *)snd_buffer, child.cksize);

	mmioClose(hwav, 0);

	m_Sound[sound_id].rate  = wfmtx.nSamplesPerSec;
	m_Sound[sound_id].size  = child.cksize;
	m_Sound[sound_id].state = SOUND_LOADED;

	memset(&m_PCMWF, 0, sizeof(WAVEFORMATEX));

	m_PCMWF.wFormatTag	  = WAVE_FORMAT_PCM;
	m_PCMWF.nChannels		  = 1;
	m_PCMWF.nSamplesPerSec  = 11025;
	m_PCMWF.nBlockAlign	  = 1;                
	m_PCMWF.nAvgBytesPerSec = m_PCMWF.nSamplesPerSec * m_PCMWF.nBlockAlign;
	m_PCMWF.wBitsPerSample  = 8;
	m_PCMWF.cbSize		  = 0;

	m_DSBD.dwSize		  = sizeof(DSBUFFERDESC);
	m_DSBD.dwFlags		  = nFlags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	m_DSBD.dwBufferBytes  = child.cksize;
	m_DSBD.lpwfxFormat	  = &m_PCMWF;
	m_DSBD.dwReserved     = 0;

	if (FAILED(m_pDSound->CreateSoundBuffer(&m_DSBD,&m_Sound[sound_id].pDSBuffer,NULL)))
	{
		free(snd_buffer);
		return -1;
	}

	if (FAILED(m_Sound[sound_id].pDSBuffer->Lock(0,child.cksize,(void **) &audio_ptr_1,&audio_length_1,
		                                                        (void **)&audio_ptr_2,&audio_length_2,DSBLOCK_FROMWRITECURSOR)))
		return 0;

	memcpy(audio_ptr_1, snd_buffer, audio_length_1);

	memcpy(audio_ptr_2, (snd_buffer + audio_length_1),audio_length_2);

	if (FAILED(m_Sound[sound_id].pDSBuffer->Unlock(audio_ptr_1,audio_length_1,audio_ptr_2,audio_length_2)))
		return 0;

	free(snd_buffer);

	return(sound_id);
}

///////////////////////////////////////////////////////////

int CDSound::ReplicateSound(WORD wId)
{
	// this function replicates the sent sound and sends back the
	// id of the replicated sound, you would use this function
	// to make multiple copies of a gunshot or something that
	// you want to play multiple times simulataneously, but you
	// only want to load once

	if (wId != -1)
	{
		for (int i=0;i<MAX_SOUNDS;i++)
		{
			if (m_Sound[i].state == SOUND_NULL)
			{
				m_Sound[i] = m_Sound[wId];

				if (FAILED(m_pDSound->DuplicateSoundBuffer(m_Sound[wId].pDSBuffer,&m_Sound[i].pDSBuffer)))
				{
					m_Sound[i].pDSBuffer = NULL;
					m_Sound[i].state    = SOUND_NULL;

					return -1;
				}
				m_Sound[i].id = i;
				return i;
			}
		}
	}
	else
		return -1;

	return -1;
}

//////////////////////////////////////////////////////////

bool CDSound::Init(HWND hWND)
{
	static int first_time = 1;
	if (first_time)
	{		
		memset(m_Sound,0,sizeof(PCM_SOUND) * MAX_SOUNDS);
		first_time = 0;

		if (FAILED(DirectSoundCreate(NULL, &m_pDSound, NULL)))
			return false;

		if (FAILED(m_pDSound->SetCooperativeLevel(hWND,DSSCL_NORMAL)))
			return false;
	}

	for (int index=0; index<MAX_SOUNDS; index++)
	{
		if (m_Sound[index].pDSBuffer)
		{
			m_Sound[index].pDSBuffer->Stop();
			m_Sound[index].pDSBuffer->Release();

		}
		
		memset(&m_Sound[index],0,sizeof(PCM_SOUND));

		m_Sound[index].state = SOUND_NULL;
		m_Sound[index].id    = index;
	}

	return true;
}

///////////////////////////////////////////////////////////

void CDSound::Release()
{
    StopAllSounds();

	for (int index=0; index<MAX_SOUNDS; index++)
		if (m_Sound[index].pDSBuffer)
			m_Sound[index].pDSBuffer->Release();

	if (m_pDSound)
		m_pDSound->Release();
}

///////////////////////////////////////////////////////////

bool CDSound::Play(WORD wId, INT nFlags, WORD Volume)
{
	if (m_Sound[wId].pDSBuffer)
	{
		if (FAILED(m_Sound[wId].pDSBuffer->SetCurrentPosition(0)))
			return false;

		SetVolume(wId,Volume);

		if (FAILED(m_Sound[wId].pDSBuffer->Play(0,0,nFlags)))
			return false;
	}

	return true;
}

///////////////////////////////////////////////////////////

bool CDSound::SetVolume(WORD wId,WORD wVol)
{
	// this function sets the volume on a sound 0-100

	if (m_Sound[wId].pDSBuffer->SetVolume(DSVOLUME_TO_DB(wVol))!=DS_OK)
		return false;

	return true;
}

///////////////////////////////////////////////////////////

bool CDSound::SetFreq(WORD wId,DWORD dwFreq)
{
	// this function sets the playback rate

	if (m_Sound[wId].pDSBuffer->SetFrequency(dwFreq) != DS_OK)
		return false;

	return true;
}

///////////////////////////////////////////////////////////

bool CDSound::SetPan(WORD wId,LONG lPan)
{
	// this function sets the pan, -10,000 to 10,0000

	if (m_Sound[wId].pDSBuffer->SetPan(lPan) != DS_OK)
		return false;

	return true;
}

////////////////////////////////////////////////////////////

void CDSound::StopSound(WORD wId)
{
	if (m_Sound[wId].pDSBuffer)
	{
		m_Sound[wId].pDSBuffer->Stop();
		m_Sound[wId].pDSBuffer->SetCurrentPosition(0);
	}
}

///////////////////////////////////////////////////////////

void CDSound::DeleteAllSounds()
{
	for (int index=0; index < MAX_SOUNDS; index++)
		DeleteSound(index);
}

///////////////////////////////////////////////////////////

void CDSound::DeleteSound(WORD wId)
{
	StopSound(wId);

	if (m_Sound[wId].pDSBuffer)
	{
		m_Sound[wId].pDSBuffer->Release();
		m_Sound[wId].pDSBuffer = NULL;
	}
}

///////////////////////////////////////////////////////////

void CDSound::StopAllSounds()
{
	for (int index=0; index<MAX_SOUNDS; index++)
		StopSound(index);	
}

///////////////////////////////////////////////////////////

int CDSound::StatusSound(WORD wId)
{
	// this function returns the status of a sound
	if (m_Sound[wId].pDSBuffer)
	{
		ULONG status;
		m_Sound[wId].pDSBuffer->GetStatus(&status);
		return status;

	}
	else
		return -1;
}

///////////////////////////////////////////////////////////

int CDSound::DMusicLoadMIDI(char *filename)
{
	// this function loads a midi segment

	DMUS_OBJECTDESC ObjDesc; 
	HRESULT hr;
	IDirectMusicSegment* pSegment = NULL;

	int index;
	int id = -1;

	for (index = 0; index < DM_NUM_SEGMENTS; index++)
	{
		if (m_Midi[index].state == MIDI_NULL)
		{
			id = index;
			break;
		}
	}
	if (id == -1)
		return -1;

	char szDir[_MAX_PATH];
	WCHAR wszDir[_MAX_PATH]; 

	if(_getcwd( szDir, _MAX_PATH ) == NULL)
	{
		return -1;
	}

	MULTI_TO_WIDE(wszDir, szDir);

	// tell the loader were to look for files
	hr = m_pDMLoader->SetSearchDirectory(GUID_DirectMusicAllTypes,wszDir, FALSE);

	if (FAILED(hr)) 
	{
		return -1;
	}

	// convert filename to wide string
	WCHAR wfilename[_MAX_PATH]; 
	MULTI_TO_WIDE(wfilename, filename);

	// setup object description
	DD_INIT_STRUCT(ObjDesc);
	ObjDesc.guidClass = CLSID_DirectMusicSegment;
	wcscpy(ObjDesc.wszFileName, wfilename );
	ObjDesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;

	// load the object and query it for the IDirectMusicSegment interface
	// This is done in a single call to IDirectMusicLoader::GetObject
	// note that loading the object also initializes the tracks and does 
	// everything else necessary to get the MIDI data ready for playback.

	hr = m_pDMLoader->GetObject(&ObjDesc,IID_IDirectMusicSegment, (void**) &pSegment);

	if (FAILED(hr))
		return -1;

	// ensure that the segment plays as a standard MIDI file
	// you now need to set a parameter on the band track
	// Use the IDirectMusicSegment::SetParam method and let 
	// DirectMusic find the trackby passing -1 (or 0xFFFFFFFF) in the dwGroupBits method parameter.

	hr = pSegment->SetParam(GUID_StandardMIDIFile,-1, 0, 0, (void*)m_pDMPerf);

	if (FAILED(hr))
		return -1;

	// This step is necessary because DirectMusic handles program changes and 
	// bank selects differently for standard MIDI files than it does for MIDI 
	// content authored specifically for DirectMusic. 
	// The GUID_StandardMIDIFile parameter must be set before the instruments are downloaded. 

	// The next step is to download the instruments. 
	// This is necessary even for playing a simple MIDI file 
	// because the default software synthesizer needs the DLS data 
	// for the General MIDI instrument set
	// If you skip this step, the MIDI file will play silently.
	// Again, you call SetParam on the segment, this time specifying the GUID_Download parameter:

	hr = pSegment->SetParam(GUID_Download, -1, 0, 0, (void*)m_pDMPerf);

	if (FAILED(hr))
		return -1;

	// at this point we have MIDI loaded and a valid object

	m_Midi[id].pDMSegment  = pSegment;
	m_Midi[id].pDMSegstate = NULL;
	m_Midi[id].state       = MIDI_LOADED;

	return id;
}

//////////////////////////////////////////////////////////

bool CDSound::DMusicPlay(WORD wId)
{
	if (m_Midi[wId].pDMSegment && m_Midi[wId].state != MIDI_NULL)
	{
		if (m_nDMActiveId != -1)
			DMusicStop(m_nDMActiveId);

		m_pDMPerf->PlaySegment(m_Midi[wId].pDMSegment, 0, 0, &m_Midi[wId].pDMSegstate);
		m_Midi[wId].state = MIDI_PLAYING;

		m_nDMActiveId = wId;
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////

bool CDSound::DMusicStop(WORD wId)
{
	if (m_Midi[wId].pDMSegment && m_Midi[wId].state != MIDI_NULL)
	{
		m_pDMPerf->Stop(m_Midi[wId].pDMSegment, NULL, 0, 0);
		m_Midi[wId].state = MIDI_STOPPED;

		m_nDMActiveId = -1;
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////

void CDSound::DMusicDeleteMIDI(WORD wId)
{
	if (m_Midi[wId].pDMSegment)
	{
		m_Midi[wId].pDMSegment->SetParam(GUID_Unload, -1, 0, 0, (void*)m_pDMPerf); 

		m_Midi[wId].pDMSegment->Release(); 
		m_Midi[wId].pDMSegment  = NULL;
		m_Midi[wId].pDMSegstate = NULL;
		m_Midi[wId].state       = MIDI_NULL;
	}
}

//////////////////////////////////////////////////////////

void CDSound::DMusicDeleteAllMIDI()
{
	int index;
	for (index = 0; index < DM_NUM_SEGMENTS; index++)
	{
		if (m_Midi[index].pDMSegment)
		{
			m_Midi[index].pDMSegment->SetParam(GUID_Unload, -1, 0, 0, (void*)m_pDMPerf);
			m_Midi[index].pDMSegment->Release();
			m_Midi[index].pDMSegment  = NULL;
			m_Midi[index].pDMSegstate = NULL;
			m_Midi[index].state       = MIDI_NULL;
		}
	}
}

//////////////////////////////////////////////////////////

int CDSound::DMusicStatusMIDI(WORD wId)
{
	if (m_Midi[wId].pDMSegment && m_Midi[wId].state != MIDI_NULL )
	{
		if (m_pDMPerf->IsPlaying(m_Midi[wId].pDMSegment,NULL) == S_OK) 
			m_Midi[wId].state = MIDI_PLAYING;
		else
			m_Midi[wId].state = MIDI_STOPPED;

		return m_Midi[wId].state;
	}
	else
		return 0;
}

///////////////////////////////////////////////////////////

bool CDSound::DMusicInit(HWND hWND)
{
	int index;
	if (FAILED(CoInitialize(NULL)))
	{    
		return false;
	}

	if (FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,NULL,CLSCTX_INPROC,IID_IDirectMusicPerformance,(void**)&m_pDMPerf)))    
	{
		return false;
	}

	if (FAILED(m_pDMPerf->Init(NULL,m_pDSound,hWND)))
	{
		return false;
	}

	if (FAILED(m_pDMPerf->AddPort(NULL)))
	{    
		return false;
	}

	if (FAILED(CoCreateInstance(CLSID_DirectMusicLoader,NULL,CLSCTX_INPROC,IID_IDirectMusicLoader,(void**)&m_pDMLoader)))
	{
		return false;
	}

	for (index = 0; index < DM_NUM_SEGMENTS; index++)
	{
		m_Midi[index].pDMSegment  = NULL;  
		m_Midi[index].pDMSegstate = NULL;  
		m_Midi[index].state       = MIDI_NULL;
		m_Midi[index].id          = index;
	}
	m_nDMActiveId = -1;
	return true;
}

////////////////////////////////////////////////////////////

void CDSound::DMusicRelease()
{
	if (m_pDMPerf)
		m_pDMPerf->Stop(NULL, NULL, 0, 0 ); 

	DMusicDeleteAllMIDI();
 
	if (m_pDMPerf)
	{
		m_pDMPerf->CloseDown();
		m_pDMPerf->Release();     
	}

	if (m_pDMLoader)
		m_pDMLoader->Release();     

	CoUninitialize(); 
}



