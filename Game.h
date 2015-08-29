#ifndef _GAME_H                
#define _GAME_H                 
#define WIN32_LEAN_AND_MEAN

// To make a game from this engine, simply extend this Game class and override
// the private initialize method to set up the game, the private loadTextures 
// method to load any necessary textures during initialization (they can be 
// loaded and unloaded as needed), and the private update method to update 
// game logic.

// Initializing would involve loading any game entity definitions to the DefinitionManager
// and adding any necessary entities to the scene.

// Loading textures is done by simply calling the method in Graphics.
// For example, to load a texture in file (I need to change how textures 
// are used in the engine)

// Updating would involve editing the velocity, positions, and other properties of game entities,
// as well as adding or removing them from the scene. The engine handles physics (position 
// resulting from velocity and collision detections) internally, so worrying about that is 
// unnecessary, unless something else is required.

#include <windows.h>
#include <Mmsystem.h>
#include "Graphics.h"
#include "Input.h"
#include "Data.h"
#include "GameException.h"
#include "Scene.h"

namespace MEngineNS
{

// Game class that handles updating and rendering the scene.
class Game
{
protected:
	// common game properties
	Graphics* m_pGraphics;         // pointer to Graphics
	Input* m_pInput;             // pointer to Input
	HWND m_hwnd;               // window handle
	HRESULT m_hr;                 // standard return type
	double m_timeStart;    // Performance Counter start value
	double m_timeEnd;      // Performance Counter end value
	double m_frameTime;          // time required for last frame
	float m_fps;                // frames per second
	DWORD m_sleepTime;          // number of milli-seconds to sleep between frames
	bool m_paused;             // true if game is paused
	bool m_initialized;		// true if the game has been initialized
	Scene* m_pCurrentScene;	// current scene being updated and rendered

public:
	// Constructor
	Game();
	// Destructor
	virtual ~Game();

	// Member functions

	// Window message handler
	LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Initialize the game
	// Pre: hwnd is handle to window
	void Initialize(HWND hwnd);

	// Call run repeatedly by the main message loop in WinMain
	void Run(HWND);

	// Call when the graphics device was lost.
	// Release all reserved video memory so graphics device may be reset.
	void ReleaseAll();

	// Recreate all surfaces and reset all entities.
	void ResetAll();

	// Delete all reserved memory.
	void DeleteAll();

	// Render game items.
	void RenderGame();

	// Handle lost graphics device
	void HandleLostGraphicsDevice();

	// Return pointer to Graphics.
	Graphics* GetGraphics()
	{
		return m_pGraphics;
	}

	// Return pointer to Input.
	Input* GetInput()
	{
		return m_pInput;
	}

	// Exit the game
	void ExitGame()
	{
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

	// Update game items.
	void Update(double delta);

	// Render the scene
	void Render();

	// Load game textures
	void LoadTextures();

private:

	/** Time in milliseconds since last const update **/
	double m_dTimeSinceLastConstUpdate;

	/** Additional initialization logic (intended to be overridden) **/
	virtual void initialize() {}

	/** Additional update logic (intended to be overridden) **/
	virtual void update(double deltaT) {}

	/** Additional texture loading (intended to be overridden) **/
	virtual void loadTextures() {}

};

}

#endif
