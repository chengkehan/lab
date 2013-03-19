#include "camera.h"

cCamera::cCamera()
{
	m_camera_type = AIR_CRAFT;

	m_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_up	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

cCamera::cCamera(eCameraType camera_type)
{
	m_camera_type = camera_type;

	m_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_up	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void cCamera::get_right(D3DXVECTOR3* right)
{
	*right = m_right;
}

void cCamera::get_up(D3DXVECTOR3* up)
{
	*up = m_up;
}

void cCamera::get_look(D3DXVECTOR3* look)
{
	*look = m_look;
}

void cCamera::walk(float units)
{
	// move only on xz plane for land object
	if(m_camera_type == LAND_OBJECT)
		m_pos += D3DXVECTOR3(m_look.x, 0.0f, m_look.z) * units;
	else	// AIR_CRAFT
		m_pos += m_look * units;
}

void cCamera::strafe(float units)
{
	// move only on xz plane for land object
	if(m_camera_type == LAND_OBJECT)
		m_pos += D3DXVECTOR3(m_right.x, 0.0f, m_right.z) * units;
	else	// AIR_CRAFT
		m_pos += m_right * units;
}

void cCamera::fly(float units)
{
	// move only on y_axis for land object
	if(m_camera_type == LAND_OBJECT)
		m_pos.y += units;
	else
		m_pos += m_up * units;
}

void cCamera::pitch(float angle)
{
	D3DXMATRIX transform_matrix;
	D3DXMatrixRotationAxis(&transform_matrix, &m_right, angle);

	// rotate m_up and m_look around m_right vector
	D3DXVec3TransformCoord(&m_up,   &m_up,   &transform_matrix);
	D3DXVec3TransformCoord(&m_look, &m_look, &transform_matrix);
}

void cCamera::yaw(float angle)
{
	D3DXMATRIX transform_matrix;

	// rotate around world y-axis (0, 1, 0) always for land object
	if(m_camera_type == LAND_OBJECT)
		D3DXMatrixRotationY(&transform_matrix, angle);
	else	// rotate around own up vector for aircraft
		D3DXMatrixRotationAxis(&transform_matrix, &m_up, angle);

	// rotate m_right and m_look around m_up or y-axis
	D3DXVec3TransformCoord(&m_right, &m_right, &transform_matrix);
	D3DXVec3TransformCoord(&m_look,  &m_look,  &transform_matrix);
}

void cCamera::roll(float angle)
{
	// only roll for aircraft type
	if(m_camera_type == AIR_CRAFT)
	{
		D3DXMATRIX transform_matrix;
		D3DXMatrixRotationAxis(&transform_matrix, &m_look, angle);

		// rotate m_up and m_right around m_look vector
		D3DXVec3TransformCoord(&m_up,	 &m_up,		&transform_matrix);
		D3DXVec3TransformCoord(&m_right, &m_right,  &transform_matrix);
	}
}

void cCamera::get_view_matrix(D3DXMATRIX* v)
{
	// keep camera's axis orthogonal to each other
	D3DXVec3Normalize(&m_look, &m_look);

	D3DXVec3Cross(&m_up, &m_look, &m_right);
	D3DXVec3Normalize(&m_up, &m_up);

	D3DXVec3Cross(&m_right, &m_up, &m_look);
	D3DXVec3Normalize(&m_right, &m_right);

	// build the view matrix
	float x = -D3DXVec3Dot(&m_right, &m_pos);
	float y = -D3DXVec3Dot(&m_up,    &m_pos);
	float z = -D3DXVec3Dot(&m_look,  &m_pos);

	(*v)(0, 0) = m_right.x;		(*v)(0, 1) = m_up.x;	(*v)(0, 2) = m_look.x;		(*v)(0, 3) = 0.0f;
	(*v)(1, 0) = m_right.y;		(*v)(1, 1) = m_up.y;	(*v)(1, 2) = m_look.y;		(*v)(1, 3) = 0.0f;
	(*v)(2, 0) = m_right.z;		(*v)(2, 1) = m_up.z;	(*v)(2, 2) = m_look.z;		(*v)(2, 3) = 0.0f;
	(*v)(3, 0) = x;				(*v)(3, 1) = y;			(*v)(3, 2) = z;				(*v)(3, 3) = 1.0f;
}

void cCamera::set_camera_type(eCameraType camera_type)
{
	m_camera_type = camera_type;
}
