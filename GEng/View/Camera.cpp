#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace GEng
{

GLfloat sensitivityMouse = 0.005f;

// Camera ///////////////////////////////////////////////////////////
Vec3 Camera::GetLook() const
{	return vLook;
}
Mat4 Camera::GetMatrix() const
{
	Mat4 mCam = GetProjection() * GetView();
	return mCam;
}
Mat4 Camera::GetView() const
{
	return glm::lookAt(pos, pos + vLook, vUp);
}
Mat4 Camera::GetProjection() const
{
	return glm::perspective(fFov, fAspect, fNear, fFar);
}
void Camera::ProcessEventInput(SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION && SDL_GetRelativeMouseMode())
    {
		angle.z -= event.motion.xrel * sensitivityMouse;
		angle.x -= event.motion.yrel * sensitivityMouse;
		Limit(angle.x, -(pi2 - 0.001f), pi2 - 0.001f);
		UpdateLook();
    }
}
void Camera::ProcessStateInput(Val timeDelta)
{
	// Обновление позиции.
	Val dist = 1.0f * timeDelta;
	const Uint8* keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_UP])          angle.x += 5 * sensitivityMouse;
	if (keys[SDL_SCANCODE_DOWN])        angle.x -= 5 * sensitivityMouse;
	if (keys[SDL_SCANCODE_LEFT])        angle.z += 5 * sensitivityMouse;
	if (keys[SDL_SCANCODE_RIGHT])       angle.z -= 5 * sensitivityMouse;
	Limit(angle.x, -(pi2 - 0.001f), pi2 - 0.001f);
	UpdateLook();

	if (keys[SDL_SCANCODE_LSHIFT] ||
		keys[SDL_SCANCODE_RSHIFT])		dist *= 10;
	if (keys[SDL_SCANCODE_W])			pos += vLook * dist;
	if (keys[SDL_SCANCODE_S])			pos -= vLook * dist;
	if (keys[SDL_SCANCODE_A])			pos -= glm::cross(vLook, vUp) * dist;
	if (keys[SDL_SCANCODE_D])			pos += glm::cross(vLook, vUp) * dist;
	if (keys[SDL_SCANCODE_PAGEUP])      pos += vUp * dist;
	if (keys[SDL_SCANCODE_PAGEDOWN])    pos -= vUp * dist;
}
void Camera::UpdateLook()
{
    float cosX = cos(angle.x);
    vLook.x = cosX * cos(angle.z);
    vLook.y = cosX * sin(angle.z);
    vLook.z = sin(angle.x);
    vLook = glm::normalize(vLook);
}

}
