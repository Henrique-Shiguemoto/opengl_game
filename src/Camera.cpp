#include "Camera.h"

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(this->position_f, this->position_f + this->front_f, this->up_f);
}

void Camera::ProcessMouseMovement(f32 xoffset, f32 yoffset){
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw   += xoffset;
    this->pitch -= yoffset;

    if (this->pitch > 89.0f)  this->pitch = 89.0f;
    if (this->pitch < -89.0f) this->pitch = -89.0f;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(f32 yoffset){
        this->fovDegrees -= yoffset;
        if (this->fovDegrees < 1.0f) 	this->fovDegrees = 1.0f;
        if (this->fovDegrees > 45.0f) 	this->fovDegrees = 45.0f;
}

void Camera::updateCameraVectors(){
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front_f = glm::normalize(front);
	
	this->right_f = glm::normalize(glm::cross(this->front_f, this->worldUp_f));
	this->up_f    = glm::normalize(glm::cross(this->right_f, this->front_f));
}
