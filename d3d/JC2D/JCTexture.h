#ifndef __JC_TEXTURE_H__
#define __JC_TEXTURE_H__

#include <Windows.h>
#include <d3d9.h>

#include "JCObject.h"

class JCTexture : public JCObject
{
public:
	~JCTexture();

private:
	JCTexture();
	JCTexture(CONST JCTexture& texture);
};

#endif