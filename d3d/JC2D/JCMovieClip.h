#ifndef __JC_MOVIECLIP_H__
#define __JC_MOVIECLIP_H__

#include <windows.h>
#include <d3d9.h>

#include "JCDisplayObject.h"

class JCMovieClip : public JCDisplayObject
{
public:
	struct Frame
	{
		IDirect3DTexture9* lpTexture;
		FLOAT refX;
		FLOAT refY;
	};

public:
	JCMovieClip(IDirect3DDevice9* lpd3dd);
	virtual ~JCMovieClip();

public:
	VOID setFrames(Frame* lpFrames, INT numFrames);
	Frame* getFrames();
	Frame* gotoFrame(INT index);

private:
	JCMovieClip();
	JCMovieClip(CONST JCMovieClip& mc);

private:
	Frame* m_lpFrames;
	INT m_numFrames;
};

#endif