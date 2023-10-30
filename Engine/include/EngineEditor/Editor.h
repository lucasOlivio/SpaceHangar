#pragma once

#include "common/types.h"
#include "scene/SceneView.h"
#include "scene/iSceneDirector.h"
#include "events/KeyEvent.h"
#include "events/iListener.h"

// For now only some info been printed on console
// and handling keycallbacks from glfw
class Editor : public iListener
{
private:
	int m_selectedEntity;
	int m_selectedComponent;
	int m_lastSelectedEntity;
	int m_selectedParameter;
	bool m_isRunning;
	std::vector<sComponentInfo> m_vecCompInfos;

	SceneView* m_pSceneView;
	iSceneDirector* m_pSceneDirector;

	// TODO: Better way to handle multiple value types from parameters
	void m_PrintParameter(std::string parName, std::string parValue);
	void m_PrintParameter(std::string parName, float parValue);
	void m_PrintParameter(std::string parName, int parValue);
	void m_PrintParameter(std::string parName, glm::vec3 parValue);
	void m_PrintParameter(std::string parName, glm::vec4 parValue);
	void m_PrintParameter(std::string parName, bool parValue);
	void m_PrintParameter(std::string parName, std::vector<std::string> parValue);

	void m_ModifySelected(int& value, int orientation);
	void m_ModifySelected(bool& value, int orientation);
	void m_ModifySelected(float& value, int orientation);
	void m_ModifySelected(glm::vec3& value, int orientation, int axis);
	void m_ModifySelected(glm::vec4& value, int orientation, int axis);
	void m_ModifySelectedCamera(glm::vec3& value, int orientation, int axis);
public:
	// ctors & dtors
	Editor(KeyEvent* pKeyEvent, SceneView* pSceneView, iSceneDirector* pSceneDirector);
	~Editor();

	// TODO: The scene should be changed so that the systems can get only the
	// componen they need, to avoid the whole thing been passed everywhere 

	// Draw selected entity UI info
	void RedrawEntityUI();

	// On key callback we manage entities on the scene accordingly
	virtual void Notify(iEvent* pEvent);

	bool IsRunning();
	void SetRunning(bool isRunning);

	// TODO: Set changes to be handled by each component I think
	// without editor needing to know about all
	// 
	// Actions to take based on key pressed
	void KeyActions(sKeyInfo keyInfo);

	void ChangeSelected(int& selected, int orientation, int count);
	void ChangeSelectedEntity(int orientation);
	void ChangeSelectedComponent(int orientation);
	void ChangeSelectedParameter(int orientation);

	void ModifySelectedParameter(int axis, int orientation);
	void SetParameterManually(int axis);
};