#include "JCMovieClip.h"

JCMovieClip::JCMovieClip(IDirect3DDevice9* lpd3dd):JCDisplayObject(lpd3dd)
{

}

JCMovieClip::~JCMovieClip()
{

}

VOID JCMovieClip::setFrames(Frame* lpFrames, INT numFrames)
{
	m_lpFrames = lpFrames;
	m_numFrames = numFrames;
}

JCMovieClip::Frame* JCMovieClip::getFrames()
{
	return m_lpFrames;
}

JCMovieClip::Frame* JCMovieClip::gotoFrame(INT index)
{
	if(index < 0 || index >= m_numFrames)
	{
		return NULL;
	}

	Frame* frame = &m_lpFrames[index];
	setTexture(frame->lpTexture);
	setRefX(frame->refX);
	setRefY(frame->refY);

	return frame;
}