#ifndef SoundManager_H
#define SoundManager_H

#define SOUND_DIR "Sounds\\"
#include "Lego.h"

class SoundManager
{
public:

	typedef struct tagWAVEFILE
	{
		DWORD           cbSize;                // Size of file
		LPWAVEFORMATEX  pwfxInfo;              // Wave Header
		LPBYTE          pbData;                // Wave Bits
	}
	WAVEFILE, *LPWAVEFILE;

	static char c_szWAV[];

	LPDIRECTSOUND8 ds;
	std::vector<LPDIRECTSOUNDBUFFER> savedBuffs;
    std::vector< std::string > filenames;

	std::vector<LPDIRECTSOUNDBUFFER> loopBuffs;
	std::vector< std::string > loopFilenames;

	std::list<LPDIRECTSOUNDBUFFER> activeChannels;

	bool sfxCapable;
	static int sfxOn;
	bool pauseSfxOff;

	SoundManager( );
	~SoundManager( );

	int addLoop( char *name );
	void playLoop( int id );
	void stopLoop( int id );
	void restartLoop( int id );

	int addSound( char *name ); // returns the sound ID used to play it

	void playSound( int id, float lrPercent = .5f, float depthPercent = 0.0f );

	void turnSfxOff( );
	void turnSfxOn( );

	bool InitSound();
	void OnPause();
	void Update();
	void OnResume();
	void Stop();


private:


	bool ParseWaveMemory( LPVOID lpChunkOfMemory,            // Points to raw ram
		                  LPWAVEFORMATEX  *lplpWaveHeader,   // Points to pointer to header
		                  LPBYTE          *lplpWaveSamples,  // Points to pointer to samples
		                  LPDWORD         lpcbWaveSize );    // Points to size

    LPVOID SoundManager::LoadWaveFile( LPSTR		szFileName,  // Filename to use
		                               LPWAVEFILE   pWaveFile ); // Points to the struct to fill

};

extern SoundManager SOUND;

#endif