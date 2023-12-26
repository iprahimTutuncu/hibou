#include "pch.h"
#include "target_camera.h"


namespace tsm {

	void TargetCamera::yaw(float rotation)
	{
		yaw_value += rotation;
		update();
	}
	void TargetCamera::roll(float rotation)
	{
		pitch_value += rotation;
		update();
	}
	void TargetCamera::pitch(float rotation)
	{
		pitch_value += rotation;
		update();
	}
	void TargetCamera::rotate(const float yaw, const float pitch, const float roll)
	{
		yaw_value += yaw;
		pitch_value += pitch;
		pitch_value = std::min(std::max(pitch_value, min_Ry), max_Ry);
		update();
	}
	void TargetCamera::setYaw(float rotation)
	{
		yaw_value = rotation;
		update();
	}
	void TargetCamera::setRoll(float rotation)
	{
		roll_value = rotation;
		update();
	}
	void TargetCamera::setPitch(float rotation)
	{
		pitch_value = rotation;
		update();
	}
	void TargetCamera::setRotation(const float yaw, const float pitch, const float roll)
	{
		yaw_value = yaw;
		roll_value = pitch;
		pitch_value = roll;
		update();
	}
}