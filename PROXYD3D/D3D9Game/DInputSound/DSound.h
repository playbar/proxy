/////////////////////////////////////////////////////
//DSound.h
/////////////////////////////////////////////////////

#pragma once

#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <direct.h>

#define DM_NUM_SEGMENTS 64 // number of midi segments that can be cached in memory


#define MIDI_NULL     0
#define MIDI_LOADED   1
#define MIDI_PLAYING  2
#define MIDI_STOPPED  3


#define MAX_SOUNDS     64

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3


#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME )

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, y,-1,x,_MAX_PATH);

#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }



typedef struct
{
	LPDIRECTSOUNDBUFFER pDSBuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int id;                         // id number of the sound
}PCM_SOUND,*PCM_SOUND_PTR;


typedef struct
{
	IDirectMusicSegment        *pDMSegment;  // the directmusic segment
	IDirectMusicSegmentState   *pDMSegstate; // the state of the segment
	int                        id;           // the id of this segment               
	int                        state;        // state of midi song
}DMUSIC_MIDI,*DMUSIC_MIDI_PTR;



class CDSound
{
public:
	CDSound()
	{
		m_nDMActiveId = -1;
	}
	~CDSound(){}

	LPDIRECTSOUND		m_pDSound;    // directsound interface pointer
	DSBUFFERDESC		m_DSBD;           // directsound description
	DSCAPS				m_DSCaps;         // directsound caps
	HRESULT				m_DSResult;       // general directsound result
	DSBCAPS				m_DSBCaps;        // directsound buffer caps
	LPDIRECTSOUNDBUFFER	m_pDSPrimary;   // the primary mixing buffer
	PCM_SOUND			m_Sound[MAX_SOUNDS];    // the array of secondary sound buffers

	WAVEFORMATEX		m_PCMWF;          // generic waveformat structure

	IDirectMusicPerformance    *m_pDMPerf;    // the directmusic performance manager 
	IDirectMusicLoader         *m_pDMLoader;  // the directmusic loader

	DMUSIC_MIDI                m_Midi[DM_NUM_SEGMENTS];
	int						   m_nDMActiveId;

public:

	bool Init(HWND hWND);
	void Release();


	int LoadWAV(TCHAR *filename, INT nflags = DSBCAPS_CTRLDEFAULT);
	int ReplicateSound(WORD wId);
	bool Play(WORD wId, INT nFlags = 0, WORD Volume = 0);
	void StopSound(WORD wId);
	void StopAllSounds();
	void DeleteSound(WORD wId);
	void DeleteAllSounds();
	int StatusSound(WORD wId);
	bool SetVolume(WORD wId,WORD wVol);
	bool SetFreq(WORD wId,DWORD dwFreq);
	bool SetPan(WORD wId,LONG lPan);

	int DMusicLoadMIDI(char *filename);
	bool DMusicPlay(WORD wId);
	bool DMusicStop(WORD wId);
	void DMusicRelease();
	void DMusicDeleteMIDI(WORD wId);
	void DMusicDeleteAllMIDI();
	int DMusicStatusMIDI(WORD wId);
	bool DMusicInit(HWND hWND);
};


extern CDSound * g_pDSound;