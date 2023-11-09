#include "EngineEditor/Editor.h"
#include "components.h" // TODO: Find a way to remove this dependency
#include "components/iComponent.h"
#include "common/opengl.h"
#include "common/utils.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

double changeStepFloat = 0.01;
int changeStepInt = 1;

void Editor::m_PrintParameter(std::string parName, std::string parValue)
{
	printf("%s: %s\n", parName.c_str(), parValue.c_str());
}

void Editor::m_PrintParameter(std::string parName, float parValue)
{
	printf("%s: %.2f\n", parName.c_str(), parValue);
}

void Editor::m_PrintParameter(std::string parName, int parValue)
{
	printf("%s: %d\n", parName.c_str(), parValue);
}

void Editor::m_PrintParameter(std::string parName, bool parValue)
{
	printf("%s: %d\n", parName.c_str(), parValue);
}

void Editor::m_PrintParameter(std::string parName, glm::vec3 parValue)
{
	printf("%s: %s\n", parName.c_str(), glm::to_string(parValue).c_str());
}

void Editor::m_PrintParameter(std::string parName, glm::vec4 parValue)
{
	printf("%s: %s\n", parName.c_str(), glm::to_string(parValue).c_str());
}

void Editor::m_PrintParameter(std::string parName, std::vector<std::string> parValue)
{
	printf("%s: [ ", parName.c_str());

	for (std::string str : parValue)
	{
		printf("'%s' ", str.c_str());
	}

	printf("]\n");
}

Editor::Editor(KeyEvent* pKeyEvent, SceneView* pSceneView, iSceneDirector* pSceneDirector)
	: m_pSceneView(pSceneView), m_pSceneDirector(pSceneDirector)
{
	pKeyEvent->Attach(this);

	this->m_lastSelectedEntity = 0;
	this->m_selectedEntity = 0;
	this->m_selectedComponent = 0;
	this->m_selectedParameter = 0;
	this->m_isRunning = true;
}

Editor::~Editor()
{
}

void Editor::RedrawEntityUI()
{
	if (!this->m_isRunning)
	{
		// Only allow changes on editor mode
		return;
	}

	system("cls");
	printf("Editor / Play mode: P\n");

	this->m_vecCompInfos = this->m_pSceneView->GetComponentsInfo(this->m_selectedEntity);
	sComponentInfo componentInfo = this->m_vecCompInfos[this->m_selectedComponent];


	printf("Select entity: PAGE_UP/PAGE_DOWN\n");
	printf("Select component: ARROW_RIGHT/ARROW_LEFT\n");
	printf("Select parameter: ARROW_UP/ARROW_DOWN\n");
	printf("Modify change step floats: +/-\n");
	printf("Modify change step ints: CTRL+/CTRL-\n");
	printf("Go to camera/Go back to selected: C\n");
	printf("Set value manually: ENTER\n");
	printf("Save scene: F1\n");
	printf("Load Scene: F5\n\n");

	printf("Change step floats: %.2f\n", changeStepFloat);
	printf("Change step ints: %d\n", changeStepInt);
	printf("Entity ID #%d\n\n", this->m_selectedEntity);

	printf("Component: %s\n", componentInfo.componentName.c_str());
	printf("%s\n", std::string(10, '-').c_str());

	// TODO: Better way to print the values by its type without 
	// needing to convert every time we get from components?
	for (int i = 0; i < componentInfo.componentParameters.size(); i++)
	{
		if (i == this->m_selectedParameter)
		{
			printf(">> ");
		}
		else
		{
			printf("   ");
		}
		sParameterInfo parameterInfo = componentInfo.componentParameters[i];
		if (parameterInfo.parameterType == "string")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterStrValue);
		}
		else if (parameterInfo.parameterType == "int")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterIntValue);
		}
		else if (parameterInfo.parameterType == "bool")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterBoolValue);
		}
		else if (parameterInfo.parameterType == "float")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterFloatValue);
		}
		else if (parameterInfo.parameterType == "vec3")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterVec3Value);
		}
		else if (parameterInfo.parameterType == "vec4")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterVec4Value);
		}
		else if (parameterInfo.parameterType == "vecStr")
		{
			this->m_PrintParameter(parameterInfo.parameterName, parameterInfo.parameterVecStrValue);
		}
		else
		{
			printf("warning '%s' of unkown type '%s'\n", 
					parameterInfo.parameterName.c_str(), 
					parameterInfo.parameterType.c_str());
		}
	}

	printf("\n\n");
	return;
}

void Editor::Notify(iEvent* pEvent)
{
	KeyEvent* pKeyEvent = dynamic_cast<KeyEvent*>(pEvent);
	
	this->RedrawEntityUI();
	
	this->KeyActions(pKeyEvent->GetKeyInfo());
}

bool Editor::IsRunning()
{
	return this->m_isRunning;
}

void Editor::SetRunning(bool isRunning)
{
	this->m_isRunning = isRunning;
}

void Editor::KeyActions(sKeyInfo keyInfo)
{
	// Close window
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_ESCAPE && (keyInfo.action == GLFW_PRESS))
	{
		this->m_pSceneDirector->SetRunning(false);
		return;
	}

	// Editor mode/Game mode
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_P && (keyInfo.action == GLFW_PRESS))
	{
		this->m_pSceneDirector->ChangeMode();
		return;
	}

	// Save/Load scene
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_F1 && (keyInfo.action == GLFW_PRESS))
	{
		this->m_pSceneDirector->SaveScene();
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_F5 && (keyInfo.action == GLFW_PRESS))
	{
		this->m_pSceneDirector->LoadScene();
		return;
	}

	// Entity selection
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_PAGE_UP && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedEntity(1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_PAGE_DOWN && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedEntity(-1);
		return;
	}

	// Set value directly
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_KP_0 && (keyInfo.action == GLFW_PRESS))
	{
		this->SetParameterManually(0);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_KP_1 && (keyInfo.action == GLFW_PRESS))
	{
		this->SetParameterManually(1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_KP_2 && (keyInfo.action == GLFW_PRESS))
	{
		this->SetParameterManually(2);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_KP_3 && (keyInfo.action == GLFW_PRESS))
	{
		this->SetParameterManually(3);
		return;
	}

	// Component selection
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_RIGHT && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedComponent(-1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_LEFT && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedComponent(1);
		return;
	}

	// Parameter selection
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_UP && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedParameter(-1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_DOWN && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ChangeSelectedParameter(1);
		return;
	}

	// Change step
	// --------------------------------------------
	if (keyInfo.mods == 0)
	{
		if (keyInfo.pressedKey == GLFW_KEY_KP_ADD && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepFloat += 0.01f;
			return;
		}
		if (keyInfo.pressedKey == GLFW_KEY_KP_SUBTRACT && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepFloat -= 0.01f;
			return;
		}
		if (keyInfo.pressedKey == GLFW_KEY_KP_DIVIDE && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepFloat += 0.1f;
			return;
		}
		if (keyInfo.pressedKey == GLFW_KEY_KP_MULTIPLY && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepFloat -= 0.1f;
			return;
		}
	}

	if ((keyInfo.mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
	{

		if (keyInfo.pressedKey == GLFW_KEY_KP_ADD && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepInt += 1;
			return;
		}
		if (keyInfo.pressedKey == GLFW_KEY_KP_SUBTRACT && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
		{
			changeStepInt -= 1;
			return;
		}
	}

	// Change to camera/back to selected
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_C && (keyInfo.action == GLFW_PRESS))
	{
		if (this->m_selectedEntity == 0)
		{
			this->m_selectedEntity = this->m_lastSelectedEntity;
			this->m_selectedParameter = 0;
			this->m_selectedComponent = 0;
		}
		else
		{
			this->m_lastSelectedEntity = this->m_selectedEntity;
			this->m_selectedEntity = 0;
			this->m_selectedParameter = 0;
			this->m_selectedComponent = 0;
		}
		return;
	}

	// Parameter edition
	// --------------------------------------------
	if (keyInfo.pressedKey == GLFW_KEY_D && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(0, 1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_A && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(0, -1);
		return;
	}

	if (keyInfo.pressedKey == GLFW_KEY_E && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(1, 1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_Q && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(1, -1);
		return;
	}

	if (keyInfo.pressedKey == GLFW_KEY_W && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(2, 1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_S && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(2, -1);
		return;
	}

	if (keyInfo.pressedKey == GLFW_KEY_X && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(3, 1);
		return;
	}
	if (keyInfo.pressedKey == GLFW_KEY_Z && (keyInfo.action == GLFW_PRESS || keyInfo.action == GLFW_REPEAT))
	{
		this->ModifySelectedParameter(3, -1);
		return;
	}
}

void Editor::ChangeSelected(int& selected, int orientation, int count)
{
	using namespace myutils;

	// Make sure we don't overstep
	orientation = Sign(orientation);

	selected += orientation;
	WrapMinMax(0, count - 1, selected);
}

void Editor::ChangeSelectedEntity(int orientation)
{
	int numEntities = this->m_pSceneView->GetNumEntities();
	ChangeSelected(this->m_selectedEntity, orientation, numEntities);
	this->m_selectedComponent = 0;
	this->m_selectedParameter = 0;
}

void Editor::ChangeSelectedComponent(int orientation)
{
	int numComponents = this->m_vecCompInfos.size();
	ChangeSelected(this->m_selectedComponent, orientation, numComponents);
	this->m_selectedParameter = 0;
}

void Editor::ChangeSelectedParameter(int orientation)
{
	if (this->m_selectedComponent >= this->m_vecCompInfos.size())
	{
		return;
	}
	int numParameters = this->m_vecCompInfos[this->m_selectedComponent].componentParameters.size();
	ChangeSelected(this->m_selectedParameter, orientation, numParameters);
}

void Editor::m_ModifySelected(int& value, int orientation)
{
	value += (orientation * changeStepInt);
}

void Editor::m_ModifySelected(bool& value, int orientation)
{
	value += (orientation * changeStepInt);
}

void Editor::m_ModifySelected(float& value, int orientation)
{
	value += (orientation * changeStepFloat);
}

void Editor::m_ModifySelected(glm::vec3& value, int orientation, int axis)
{
	if (axis > 2)
	{
		return;
	}

	value[axis] += (orientation * changeStepFloat);
}

void Editor::m_ModifySelected(glm::vec4& value, int orientation, int axis)
{
	if (axis > 3)
	{
		return;
	}

	value[axis] += (orientation * changeStepFloat);
}

void Editor::m_ModifySelectedCamera(glm::vec3& value, int orientation, int axis)
{
	if (axis > 2)
	{
		return;
	}

	value[axis] += (orientation * changeStepInt	);
}

void Editor::ModifySelectedParameter(int axis, int orientation)
{
	using namespace myutils;

	if (this->m_selectedComponent >= this->m_vecCompInfos.size())
	{
		return;
	}

	sComponentInfo compInfo = this->m_vecCompInfos[this->m_selectedComponent];
	sParameterInfo paramInfo = compInfo.componentParameters[this->m_selectedParameter];
	
	if (paramInfo.parameterType == "string")
	{
		// TODO: Been able to change models at name changes etc.
		return;
	}

	// Make sure we don't overstep
	orientation = Sign(orientation);

	if (paramInfo.parameterType == "int" || 
		paramInfo.parameterType == "bool" ||
		paramInfo.parameterType == "float")
	{
		if (axis > 0)
		{
			return;
		}

		if (paramInfo.parameterType == "int")
		{
			this->m_ModifySelected(paramInfo.parameterFloatValue, orientation);
		}
		else if (paramInfo.parameterType == "float")
		{
			this->m_ModifySelected(paramInfo.parameterFloatValue, orientation);
		}
		else
		{
			this->m_ModifySelected(paramInfo.parameterBoolValue, orientation);
		}
	}
	else if (paramInfo.parameterType == "vec3")
	{
		// Avoid using floats for the camera
		if (compInfo.componentName == "camera")
		{
			this->m_ModifySelectedCamera(paramInfo.parameterVec3Value, orientation, axis);
		}
		else
		{
			this->m_ModifySelected(paramInfo.parameterVec3Value, orientation, axis);
		}
	}
	else if (paramInfo.parameterType == "vec4")
	{
		this->m_ModifySelected(paramInfo.parameterVec4Value, orientation, axis);
	}

	iComponent* pComponent = this->m_pSceneView->GetComponent<iComponent>(this->m_selectedEntity, 
																		  compInfo.componentName);
	pComponent->SetParameter(paramInfo);

	return;
}

void Editor::SetParameterManually(int axis)
{
	using namespace myutils;

	// Get the value to set from the user
	printf("Value to set: ");
	std::string value;
	std::cin >> value;

	sComponentInfo compInfo = this->m_vecCompInfos[this->m_selectedComponent];
	sParameterInfo paramInfo = compInfo.componentParameters[this->m_selectedParameter];

	if (paramInfo.parameterType == "string")
	{
		// TODO: Been able to change models at name changes etc.
		return;
	}

	// TODO: Remove this code repetition validations
	if (paramInfo.parameterType == "int" ||
		paramInfo.parameterType == "bool" ||
		paramInfo.parameterType == "float")
	{
		if (axis > 0)
		{
			return;
		}

		if (paramInfo.parameterType == "float")
		{
			paramInfo.parameterFloatValue = std::stof(value);
		}
		else
		{
			paramInfo.parameterFloatValue = std::stoi(value);
		}
	}
	else if (paramInfo.parameterType == "vec3")
	{
		if (axis > 3)
		{
			return;
		}
		paramInfo.parameterVec3Value[axis] = std::stof(value);
	}
	else if (paramInfo.parameterType == "vec4")
	{
		paramInfo.parameterVec4Value[axis] = std::stof(value);
	}

	iComponent* pComponent = this->m_pSceneView->GetComponent<iComponent>(this->m_selectedEntity,
		compInfo.componentName);
	pComponent->SetParameter(paramInfo);
}
