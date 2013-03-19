#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>

enum eCameraType { LAND_OBJECT, AIR_CRAFT };

class cCamera
{
private:
	eCameraType		m_camera_type;
	D3DXVECTOR3		m_right;
	D3DXVECTOR3		m_up;
	D3DXVECTOR3		m_look;	

public:
	D3DXVECTOR3		m_pos;

public:
	cCamera();
	cCamera(eCameraType camera_type);
	~cCamera() { };

	void strafe(float units);	// left/right
	void fly(float units);		// up/down
	void walk(float units);		// forward/backward

	void pitch(float angle);	// rotate on right vector
	void yaw(float angle);		// rotate on up vector
	void roll(float angle);		// rotate on look vector

	void get_view_matrix(D3DXMATRIX* v);
	void set_camera_type(eCameraType camera_type);
	
	void get_right(D3DXVECTOR3* right);
	void get_up(D3DXVECTOR3* up);
	void get_look(D3DXVECTOR3* look);
};

#endif
