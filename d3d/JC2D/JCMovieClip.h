#ifndef __JC_MOVIECLIP_H__
#define __JC_MOVIECLIP_H__

#include <windows.h>
#include <d3d9.h>

#include "JCDisplayObject.h"
#include "JCTexture.h"

class JCMovieClip : public JCDisplayObject
{
public:
	JCMovieClip(IDirect3DDevice9* lpd3dd);
	virtual ~JCMovieClip();

	struct Frame
	{
		JCTexture* lpTexture;
		FLOAT refX;
		FLOAT refY;
	};

	VOID setFrames(Frame* lpFrames, INT numFrames);
	Frame* getFrames();
	Frame* gotoFrame(INT index);

private:
	JCMovieClip();
	JCMovieClip(CONST JCMovieClip& mc);

	Frame* m_lpFrames;
	INT m_numFrames;
};

#endif