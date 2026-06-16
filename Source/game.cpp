/******************************************************************************/
/**
 * @file        Game.cpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary) - 20%
 * @author      Chloe Lau Rey En (secondary) - 20%
 * @author      Guok Yi Yong (secondary) - 20%
 * @author      Choi Meng Yew (secondary) - 20%
 * @author      Javier Lee (secondary) - 20%
 * @brief       Controls the overall game state, including initialization, update, and cleanup.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/


#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include "../Header/Game.hpp"

//==================================================
// Define global variables
//==================================================
//float Health = MAX_HEALTH;
float PlayerX = 0.0f;
float PlayerY = -50.0f;
float EarthHealth = MAX_HEALTH;							 // Initial health of Earth
AEVec2 EarthPosition = { 450.0f, -20.0f }; // Earth's position
bool gamePaused = false;
int enemiesDestroyed = 0;
int level = 0;
extern int currentLevel;
extern int levelCounts;
extern int levelCount;

std::vector<Particle> particles;				// Initialize the Particle Field
std::vector<Explosion> explosions;				// Initialize particle explosions
std::vector<VectorField> gravityField;			// Initialize the Vector Field

// Create an instance of OuterPlanetManager, which manages the outer planets in the game.
extern OuterPlanetManager outerPlanetManager;

// Retrieve a reference to the vector of GravityPoint objects managed by outerPlanetManager.
// This allows you to directly access and modify the gravity points without copying the vector.
std::vector<GravityPoint>& gravityPoints = outerPlanetManager.getGravityPoints();

MeshManager* meshManager = nullptr;
WeaponManager* weaponManager = nullptr;
Missle* playerMissle = nullptr;

HUD* hud = nullptr;

//==================================================
// Meshes
//==================================================
AEGfxVertexList* pMeshBullet = nullptr;
AEGfxVertexList* pMeshRect = nullptr;
AEGfxVertexList* pMeshCircles = nullptr;
AEGfxVertexList* starMesh = nullptr;
AEGfxVertexList* supernovaStarMesh = nullptr;
AEGfxVertexList* gravityPointMesh = nullptr;
AEGfxVertexList* arrowMesh = nullptr;
AEGfxVertexList* particleMesh = nullptr;
AEGfxVertexList* explosionMesh = nullptr;

Background* background = nullptr;

void Game_Load()
{
	MeshManager::GetInstance().LoadMesh();
	MeshManager::GetInstance().LoadTexture();

	starMesh = createCircleMesh(0xFFFFFFFF);
	supernovaStarMesh = createFourPointedStarMesh(0xFF59e1ff);				// Blue 4-pointed stars
	gravityPointMesh = createCircleMesh(0xFFff0000);						// Red gravity points
	arrowMesh = createArrowMesh(0xFFFFFFFF);								// White arrow meshes
	particleMesh = createCircleMesh(0xFF33ff00);							// Yellow particles
	explosionMesh = createCircleMesh(0xFFFFFF00);							// Yellow explosion

	background = new Background();

	AEGfxTexture* layer1Texture = AEGfxTextureLoad("Assets/BG_cloud1.png");
	if (layer1Texture) background->addLayer(layer1Texture, 0.2f);

	AEGfxTexture* layer2Texture = AEGfxTextureLoad("Assets/BG_cloud2.png");
	if (layer2Texture) background->addLayer(layer2Texture, 0.23f);

	AEGfxTexture* layer3Texture = AEGfxTextureLoad("Assets/BG_cloud3.png");
	if (layer3Texture) background->addLayer(layer3Texture, 0.1f);
}

void Game_Initialize()
{
	srand(static_cast<unsigned int>(time(0)));
	AESysReset();

	// For Pause Button
	PauseShowConfirmation = false;

	AudioPlayer::GetInstance();

	float sfxVolume = AudioManager::GetInstance().GetSFXVolume();
	float bgmVolume = AudioManager::GetInstance().GetBGMVolume();

	// Either initialize or resume music
	if (!AudioManager::GetInstance().IsBgMusicInitialized()) {
		AudioManager::GetInstance().InitializeDefaultAudio(true);
	}
	else {
		// First try resuming paused music
		AudioManager::GetInstance().ResumeMusic();

		// If no music is currently active, start playing again
		if (!AEAudioIsValidAudio(AudioManager::GetInstance().GetCurrentBGM())) {
			AudioManager::GetInstance().PlayMusic("gameTheme", true);
		}
	}

	// Always explicitly set volumes
	AudioManager::GetInstance().SetGroupVolume(true, sfxVolume);
	AudioManager::GetInstance().SetGroupVolume(false, bgmVolume);

	meshManager = &MeshManager::GetInstance();
	weaponManager = &WeaponManager::GetInstance();

	hud = new HUD();
	CameraManager& cameraManager = CameraManager::GetInstance();
	CurrencyManager::GetInstance();
	weaponManager->SpawnPlayer();
	uiManager->InitializeFonts();

	//==================================================
	// Variable Initialization
	//==================================================

	InitializeStars();														// Initialize the Star Field
	InitializeVectorField(gravityField, 20);								// Creates grid of vectors
	EarthInit();

	cameraPosX = 0.0f;														// Start towards the right side of a 1600-wide map
	cameraPosY = 0.0f;														// Center vertically

	InitCamera(cameraManager.GetCurrentCamera());				// Initialize the camera
	ApplyCameraTransform(cameraManager.GetCurrentCamera());
	InitializeEnemies();

	ExplosionManager::GetInstance().OnInit();

	Cameralevel();
	outerPlanetManager.Init(level);

	SpawnInitialEnemies();
	InitDifficultyManager();

	CurrencyManager::GetInstance();
	hud->Init();
	// Make sure game isn't paused when starting
	PauseShowConfirmation = false;
	ShowQuitConfirmation = false;
	gamePaused = false;
}

void Game_Update()
{
	// Only update game logic if not paused
	if (!gamePaused) {
		float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());	// Frame time
		static float enemySpawnTimer = 0.0f;
		enemySpawnTimer += dt;

		// Update the coin animation
		CurrencyManager::GetInstance().Update(dt);

		HandleDifficultyManager();

		if (EarthHealth <= 0) {											// Player lost
			CurrencyManager::GetInstance().SetGameOver(true);			// Stop currency updates
		}

		UpdateStars(dt);
		UpdateVectorField(gravityField, gravityPoints);

		// assuming gravity field is containing normalized vectors per grid.
		weaponManager->Update(gravityField, gravityPoints);

		// Periodically spawn particles
		if (rand() % 10 == 0) {											// Spawn every 10 frames on average
			SpawnParticles(particles, 5, 0, gravityPoints);				// particles vector, number to spawn, enable/disable particle spawning
		}

		UpdateParticles(particles, gravityField, dt, gravityPoints, explosions);
		UpdateExplosions(explosions, dt);

		EnemyMovement(dt, EarthPosition, EarthHealth);					// Move enemies toward Earth
		BulletMovement(dt);												// Move bullets
		CheckBulletCollisions(EarthHealth, EarthPosition, 50.0f);		// Check bullet-Earth collisions
		ExplosionManager::GetInstance().OnUpdate(dt);

		// Update HUD
		hud->Update();
	}
	
	PauseButton_Update();

	if (EarthHealth == 0) {
		next = GS_GAMEOVER;
	}

	//==================================================
	// FOR DEBUGGING
	//==================================================
	//if (AEInputCheckTriggered(AEVK_4)) {
	//	EarthHealth = 0.0f;
	//}
	//if (AEInputCheckTriggered(AEVK_5)) {
	//	next = GS_GAMEWIN;
	//}
	//==================================================
	CheckLevelProgression();

}

void Game_Draw()
{
	// Set render modes and background color
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetBackgroundColor(0.051f, 0.02f, 0.08f);

	//==================================================
	// Rendering logic (Ordered for proper layering)
	//==================================================

	// Render background layers (distant background)
	background->renderLayerRange(0, 1, CameraManager::GetInstance().GetCurrentCamera());

	// Render stars
	RenderStars(starMesh, supernovaStarMesh);

	// Render middle background layers
	background->renderLayerRange(2, 2, CameraManager::GetInstance().GetCurrentCamera());
	background->renderLayerRange(1, 1, CameraManager::GetInstance().GetCurrentCamera());

	//==================================================
	// FOR DEBUGGING
	//==================================================
	// Render vector field (white arrow)
	RenderVectorField(gravityField, arrowMesh, IsVectorEnabled, CameraManager::GetInstance().GetCurrentCamera(), gravityPoints);
	//==================================================

	// Render particles (yellow)
	RenderParticles(particles, particleMesh, CameraManager::GetInstance().GetCurrentCamera());

	// Render weapons and missiles
	weaponManager->DrawWeapons();

	outerPlanetManager.Draw();

	// Render explosions (Yellow)
	RenderExplosions(explosions, explosionMesh, CameraManager::GetInstance().GetCurrentCamera());

	//--------------------------------------------------------------------------
	// Enemy and Bullet Rendering
	//--------------------------------------------------------------------------
	EnemyRender();  // Draw enemies
	BulletRender();  // Draw bullets

	EarthRender();

	ExplosionManager::GetInstance().OnRender();

	// Save current camera position before drawing UI
	f32 oldCamX, oldCamY;
	AEGfxGetCamPosition(&oldCamX, &oldCamY);

	// Reset camera for UI elements
	AEGfxSetCamPosition(0, 0);

	// Draw currency UI
	CurrencyManager::GetInstance().Draw();

	// Restore camera position
	AEGfxSetCamPosition(oldCamX, oldCamY);

	//--------------------------------------------------------------------------
	// Earth and Health Bar Render
	//--------------------------------------------------------------------------
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	EarthHealthBar(EarthHealth);

	hud->Draw();
	uiManager->DrawLevelIndicator(GetCurrLevel());

	PauseButton_Draw();
}

void Game_Free()
{
	uiManager->DestroyFonts(); // Idk if need to put this here because without is no mem leak as well but backbutton font not being freed
							   // so remove if causes error because destroying only when got valid fontID
	meshManager->UnloadTexture();
	meshManager->UnloadMesh();

	background->cleanup();
	weaponManager->Unload();

	delete background;

	if (hud) {
		hud->Reset();
		delete hud;
		hud = nullptr;
	}

	if (levels) {
		free(levels);
		levels = NULL;
	}

	FreeDifficultyManager();

	{
		AEGfxMeshFree(starMesh);
		AEGfxMeshFree(supernovaStarMesh);
		AEGfxMeshFree(gravityPointMesh);
		AEGfxMeshFree(arrowMesh);
		AEGfxMeshFree(particleMesh);
		AEGfxMeshFree(explosionMesh);
	}

	CurrencyManager::DestroyInstance();

	AudioManager::GetInstance().StopSFXOnly();
	AudioPlayer::DestroyInstance();
	outerPlanetManager.Clear();

	// For Pause Button
	PauseShowConfirmation = false;
	ShowQuitConfirmation = false;
}

void Game_Unload() {
	// Unload game-specific assets if needed
}