#include "Camera.h"
#include "glm/gtx/transform.hpp"

const float DEFAULT_FOV = 45.0f;

// --------------------------------
// CAMERA BASE CLASS
// --------------------------------

Camera::Camera() 
	: 
	mTarget(glm::vec3(0.0f, 0.0f, 0.0f)), 
	mUp(glm::vec3(0.0f, 0.1f, 0.0f)),
	mRight(0.0f, 0.0f, 0.0f),
	WORLD_UP(0.0f, 1.0f, 0.0f),
	mYaw(glm::pi<float>()),
	mPitch(0.0f),
	mRoll(0.0f),
	mFOV(DEFAULT_FOV)
{

}

glm::mat4 Camera::GetViewMatrix()const {
	return glm::lookAt(entity->getComponent<Transform>().position, mTarget, mUp);
}

const glm::vec3& Camera::GetLook()	const { //forward
	return mLook;
}

const glm::vec3& Camera::GetRight()	const { 
	return mRight;
}

const glm::vec3& Camera::GetUp()	const { 
	return mUp;
}

const glm::vec3 Camera::GetPosition()const {
	return entity->getComponent<Transform>().position;
}


// --------------------------------
// FPS CAMERA
// --------------------------------

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) :
ImGuiWindowSize(10,10)
{
	//mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

void FPSCamera::SetPosition(const glm::vec3& position){
	entity->getComponent<Transform>().position = position;
}

void FPSCamera::Move(const glm::vec3& offsetPos) {
	entity->getComponent<Transform>().position += offsetPos;
	UpdateCameraVectors();
}

void FPSCamera::OnUpdate(double deltaTime)
{
	if (ImGui::IsKeyDown('E') && m_mouseFlag == false) { m_mouseEnabled = !m_mouseEnabled; m_mouseFlag = true; }
	if (ImGui::IsKeyReleased('E')) { m_mouseFlag = false; }
	if (m_mouseEnabled) { return; }

	if (m_ignoreForXFrames > 0) {
		m_ignoreForXFrames--;
		if (m_ignoreForXFrames == 0) {
			m_lastMousePos.x = ImGui::GetMousePos().x;
			m_lastMousePos.y = ImGui::GetMousePos().y;
		}
		return;
	}

	//----------------------------------//
	//			MOUSE					//
	//----------------------------------//
	glm::vec2 currentMousePos;


	currentMousePos.x = ImGui::GetMousePos().x;
	currentMousePos.y = ImGui::GetMousePos().y;

	glm::vec2 mouseDelta = (m_lastMousePos - currentMousePos) / m_mouseSpeedDelta;
	if (mouseDelta.x != 0 || mouseDelta.y != 0){
		std::cout << mouseDelta.x << "  " << mouseDelta.y << std::endl;
		Rotate(mouseDelta.x * deltaTime * 30, mouseDelta.y * deltaTime * 30);
	}

	//----------------------------------//
	//			KEYBOARD				//
	//----------------------------------//

	if (ImGui::IsKeyDown('W')) { w = true; }
	if (ImGui::IsKeyDown('A')) { a = true; }
	if (ImGui::IsKeyDown('S')) { s = true; }
	if (ImGui::IsKeyDown('D')) { d = true; }
	if (ImGui::IsKeyDown('L')) { d = true; }
	if (ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) { l_shift = true; }

	if (ImGui::IsKeyReleased('W')) { w = false; }
	if (ImGui::IsKeyReleased('A')) { a = false; }
	if (ImGui::IsKeyReleased('S')) { s = false; }
	if (ImGui::IsKeyReleased('D')) { d = false; }
	if (ImGui::IsKeyReleased(GLFW_KEY_LEFT_SHIFT)) { l_shift = false; }

	float moveSpeed = m_MoveSpeed;
	float moveSpeedDelta = 1;
	if (l_shift) {
		moveSpeedDelta = m_MoveSpeedDelta;
	}

	if (w) { Move((moveSpeed * moveSpeedDelta) * (float)deltaTime * GetLook()); }
	else if (s) { Move((moveSpeed * moveSpeedDelta) * (float)deltaTime * -GetLook()); }

	if (a) { Move((moveSpeed * moveSpeedDelta) * (float)deltaTime * -GetRight()); }
	else if (d) { Move((moveSpeed * moveSpeedDelta) * (float)deltaTime * GetRight()); }

	m_lastMousePos = currentMousePos;
}

void FPSCamera::OnImGuiRender()
{
	ImGui::Begin("Inspector"); 
	{
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Camera - FPS", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap)) {
			ImGui::Indent();
			ImGui::Checkbox("usingImGuiWindow",&usingImGuiWindow);
			ImGui::DragFloat("FOV", &mFOV,0.01f);
			ImGui::DragFloat("Move Speed", &m_MoveSpeed,0.01f);
			ImGui::DragFloat("Move Speed Delta", &m_MoveSpeedDelta,0.01f);

			ImGui::DragFloat2("Mouse Speed", &m_mouseSpeedDelta.x, -.05, 0.05);
			ImGui::Unindent();
			glm::vec3 rot;

			entity->getComponent<Transform>().rotation = GetRotation();





		}
		ImGui::Separator();
	}
	ImGui::End();
}

glm::vec3 FPSCamera::GetRotation() {
	return glm::vec3(mRoll, mPitch, mYaw);
}

bool FPSCamera::init(){
	//entity->getComponent<Transform>().position = glm::vec3(0.0f,0.0f,-18.0f);
	std::cout << "init from Cam" << std::endl;
	return true;
}

void FPSCamera::Rotate(float yaw, float pitch) {
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// constrain pitch
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
	UpdateCameraVectors();
}

void FPSCamera::UpdateCameraVectors() {

	//spherical to cartesian coords // y is up not z

	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	//recalc right & up vector
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook));

	mTarget = entity->getComponent<Transform>().position + mLook;
}

// --------------------------------
// ORBIT CAMERA
// --------------------------------

OrbitCamera::OrbitCamera() 
	:mRadius(10.0f)
{
}

void OrbitCamera::SetLookAt(const glm::vec3& target) {
	mTarget = target;
}

void OrbitCamera::SetRadius(float radius) {
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::Rotate(float yaw, float pitch) {
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	UpdateCameraVectors();
}

void OrbitCamera::UpdateCameraVectors() {
	entity->getComponent<Transform>().position.x = mTarget.x + mRadius * cosf(mPitch) * sinf(mYaw);
	entity->getComponent<Transform>().position.y = mTarget.y + mRadius * sinf(mPitch);
	entity->getComponent<Transform>().position.z = mTarget.z + mRadius * cosf(mPitch) * cosf(mYaw);
}