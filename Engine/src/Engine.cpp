#include "Engine.h"
#include "scene/Scene.h"
#include "common/ConfigReadWriteFactory.h"
#include "events/KeyWrapper.h"
#include "common/utils.h"


// TODO: All this should come from a config file
double frameRate = 1.0 / 30.0; // 30 FPS (16ms)
std::string baseConfigsPath = "configs/";
std::string sceneFilePath = "";
std::string baseShadersPath = "assets/shaders/";
std::string baseModelPath = "assets/models/";
std::string shaderProgramName = "Shader01";
uint windowWidth = 1080;
uint windowHeight = 720;
std::string windowName = "";


// TODO: Map all data used by the other classes from the scene
// to pass only the needed data and decouple all from scene

Engine::Engine()
{
	this->m_pRenderer = nullptr;
	this->m_pEditor = nullptr;

	this->m_isInitialized = false;
	this->m_isRunning = false;

	this->m_pKeyEvent = nullptr;

	this->m_pScene = nullptr;
	this->m_pSceneView = nullptr;

	double m_lastTime = 0.0;
	m_frameTimes.clear();
}

Engine::~Engine()
{
}

bool Engine::Initialize(const std::string& sceneName)
{
	if (this->m_isInitialized)
	{
		// Already initialized
		return true;
	}

	// Events
	this->m_pCollisionEvent = new CollisionEvent();
	this->m_pKeyEvent = new KeyEvent();
	KeyWrapper::SetKeyEvent(this->m_pKeyEvent);

	printf("Initializing creation of scene '%s'\n", sceneName.c_str());

	this->m_pScene = new Scene(this->m_pKeyEvent);
	this->m_pSceneView = new SceneView(this->m_pScene);
	iConfigReadWrite* pConfigrw = ConfigReadWriteFactory::CreateConfigReadWrite("json");

	// TODO: This should come from a config file
	// Load all config variables 
	sceneFilePath = baseConfigsPath + sceneName + ".json";
	windowName = sceneName;

	printf("Loading configs...\n");
	// Load scene components and entities from database file
	bool isSceneLoaded = pConfigrw->ReadScene(sceneFilePath, this->m_pScene);
	if (!isSceneLoaded)
	{
		CheckEngineError("Scene loading");
		return false;
	}

	delete pConfigrw; // Used only to load all configs

	printf("Creating systems...\n");
	this->m_pRenderer = new Renderer();
	this->m_pEditor = new Editor(this->m_pKeyEvent, this->m_pSceneView, this);
	this->m_pPhysics = new Physics(this->m_pSceneView, this->m_pCollisionEvent);

	printf("Initializing systems...\n");
	// Initializes all systems
	bool isERInitialized = this->m_pRenderer->Initialize(baseShadersPath,
														 baseModelPath,
														 shaderProgramName,
														 windowWidth,
														 windowHeight,
														 windowName,
														 KeyWrapper::KeyCallback,
														 this->m_pSceneView);
	if (!isERInitialized)
	{
		CheckEngineError("Engine renderer initializing");
		return false;
	}

	m_lastTime = glfwGetTime();

	this->m_isInitialized = true;
	printf("Scene '%s' created scussesfully!\n", sceneName.c_str());

    return true;
}

void Engine::Run()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_isRunning = true;

	while (this->IsRunning())
	{
		double fixedDeltaTime = this->GetFixedDeltaTime();

		this->m_pPhysics->NewFrame();
		this->m_pRenderer->NewFrame();

		for (EntityID entityID = 0; entityID < this->m_pScene->GetNumEntities(); entityID++)
		{
			this->m_pPhysics->Update(entityID, fixedDeltaTime);
			this->m_pRenderer->DrawModel(entityID, fixedDeltaTime);
		}

		this->m_pRenderer->EndFrame();
	}
}

bool Engine::IsRunning()
{
	if (this->m_isRunning
		&& this->m_pRenderer->IsRunning())
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Engine::GetFixedDeltaTime()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - this->m_lastTime;
	this->m_lastTime = currentTime;

	// Debugging performance:
	// printf("delta real: %.2f\n", deltaTime);

	// Clamp delta time to the maximum frame time
	if (deltaTime > frameRate) {
		deltaTime = frameRate;
	}

	// Add the frame time to the list
	this->m_frameTimes.push_back(deltaTime);

	// Limit the number of frames
	const size_t maxFrameCount = 60; // Store frame times for a second
	if (this->m_frameTimes.size() > maxFrameCount) {
		this->m_frameTimes.erase(this->m_frameTimes.begin());
	}

	// Calculate the average frame time
	double averageFrameTime = 0;
	for (double time : this->m_frameTimes) {
		averageFrameTime += time;
	}
	averageFrameTime /= this->m_frameTimes.size();

	return averageFrameTime;
}

void Engine::Exit()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	this->m_pRenderer->Destroy();
	this->m_isInitialized = false;
	this->m_isRunning = false;

	delete this->m_pRenderer;
	delete this->m_pEditor;
	delete this->m_pScene;
	delete this->m_pSceneView;

	delete this->m_pKeyEvent;

	return;
}

void Engine::SetRunning(bool isRunning)
{
	this->m_isRunning = isRunning;
}

void Engine::ChangeMode()
{
	bool editorRunning = this->m_pEditor->IsRunning();

	if (editorRunning)
	{
		// Entering play mode
		this->SaveScene();

		this->m_pEditor->SetRunning(false);
		this->m_pPhysics->SetRunning(true);
		this->m_pScene->SetPlaying(true);

		this->LoadScene();
	}
	else
	{
		this->m_pEditor->SetRunning(true);
		this->m_pPhysics->SetRunning(false);
		this->m_pScene->SetPlaying(false);

		this->LoadScene();
	}

	return;
}

void Engine::SaveScene(std::string filePath)
{
	iConfigReadWrite* pConfigrw = ConfigReadWriteFactory::CreateConfigReadWrite(filePath);

	bool isSceneSaved = pConfigrw->WriteScene(filePath, this->m_pScene);
	if (!isSceneSaved)
	{
		CheckEngineError("Scene saving to file");
		return;
	}

	delete pConfigrw; // Used only to save configs

	return;
}

void Engine::SaveScene()
{
	this->SaveScene(sceneFilePath);
}

void Engine::LoadScene(std::string filePath)
{
	this->m_pScene->Clear();

	iConfigReadWrite* pConfigrw = ConfigReadWriteFactory::CreateConfigReadWrite(filePath);

	bool isSceneLoaded = pConfigrw->ReadScene(filePath, this->m_pScene);
	if (!isSceneLoaded)
	{
		CheckEngineError("Scene loading from file");
		return;
	}

	delete pConfigrw; // Used only to load configs

	this->m_pRenderer->LoadScene(baseModelPath);

	return;
}

void Engine::LoadScene()
{
	this->LoadScene(sceneFilePath);
}