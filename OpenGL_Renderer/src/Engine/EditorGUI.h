#ifndef EDITORGUI_H
#define EDITORGUI_H

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"

#include "../vendor/ImGuizmo/ImGuizmo.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int G_GetWindowWidth();
int G_GetWindowHeight();
bool G_GetWindowResizeFlag();

class EditorGUI
{
public:
	EditorGUI();
	~EditorGUI();

	void UpdateFrameBufferTextureSize(GLuint renderTexture);
	void RenderHierarchy();
	void RenderScene(GLuint& renderTexture);
	void RenderInspector();
	void RenderProject();
	void RenderConsole();
	void RenderMainMenuBar();
	void RenderActiveInspector();
	void OnGizmoRender();

	void Demo(bool &demo);
public:
	ImVec2 lastFrameWindowSize;
	bool windowSizeChangeFlag;


};


#endif // !EDITORGUI_H
