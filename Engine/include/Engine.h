#pragma once

#include "scene/Scene.h"
#include "scene/SceneView.h"
#include "scene/iSceneDirector.h"
#include "EngineRenderer/Renderer.h"
#include "EngineEditor/Editor.h"
#include "EnginePhysics/Physics.h"
#include "events/KeyEvent.h"
#include "events/CollisionEvent.h"

class Engine :
	public iSceneDirector
{
private:
	bool m_isRunning;
	bool m_isInitialized;
	double m_lastTime;
	std::vector<double> m_frameTimes;

	Renderer* m_pRenderer;
	Editor* m_pEditor;
	Physics* m_pPhysics;

	KeyEvent* m_pKeyEvent;
	CollisionEvent* m_pCollisionEvent;

	// TODO: Pass all other systems need to the "scene mediator" 
	// (which should be the Engine itself I think)
	Scene* m_pScene;
	SceneView* m_pSceneView;
public:
	// ctors & dtors
	Engine();
	~Engine();

	// Initializes all the systems, if anything go wrong we should stop here
	bool Initialize(const std::string& sceneName);
	
	// Run app and update all systems every frame
	void Run();

	bool IsRunning();

	double GetFixedDeltaTime();

	// Close app and destroy all
	void Exit();

	virtual void SetRunning(bool isRunning);
	// Change between edit mode and play mode
	virtual void ChangeMode();

	// Save all components data to file
	virtual void SaveScene(std::string filePath);
	virtual void SaveScene();

	// Load all components data from file
	virtual void LoadScene(std::string filePath);
	virtual void LoadScene();
};