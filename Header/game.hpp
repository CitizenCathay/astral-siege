/******************************************************************************/
/**
 * @file        Game.hpp
 * @project     Astral Siege
 * @author      Celeste Tong Jia Xuan (primary) - 70%
 * @author      Chloe Lau Rey En (secondary) - 30%
 * @brief       This file handles core game operations, including loading,
 *              initialization, updates, rendering, and cleanup.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#pragma once

#include "../Header/AudioManager.hpp"
#include "../Header/AudioPlayer.hpp"
#include "../Header/AudioConfigLoader.hpp"
#include "../Header/Background.hpp"
#include "../Header/Bullet.hpp"
#include "../Header/BackButton.hpp"
#include "../Header/CurrencyManager.hpp"
#include "../Header/Camera.hpp"
#include "../Header/CoinAnimation.hpp"
#include "../Header/CreditsScreen.hpp"
#include "../Header/ExplosionManager.hpp"
#include "../Header/Enemy.hpp"
#include "../Header/Enemybullet.hpp"
#include "../Header/EnemyConfigLoader.hpp"
#include "../Header/EnemyFactory.hpp"
#include "../Header/EnemyManager.hpp"
#include "../Header/Gamestatemanager.hpp"
#include "../Header/GravitySystem.hpp"
#include "../Header/GameObject.hpp"
#include "../Header/GameStateList.hpp"
#include "../Header/GameOver.hpp"
#include "../Header/GameWin.hpp"
#include "../Header/HUD.hpp" 
#include "../Header/Level.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Missle.hpp"
#include "../Header/MainMenu.hpp"
#include "../Header/OptionsMenu.hpp"
#include "../Header/OuterPlanetManager.hpp"
#include "../Header/Player.hpp"
#include "../Header/Particles.hpp"
#include "../Header/PauseButton.hpp"
#include "../Header/Stars.hpp"
#include "../Header/SplashScreen.hpp"
#include "../Header/TransitionManager.hpp"
#include "../Header/TutorialsMenu.hpp"
#include "../Header/UIManager.hpp"
#include "../Header/Vector.hpp"
#include "../Header/WeaponManager.hpp"
#include "../Header/DifficultyManager.hpp"
#include "AEEngine.h"

// Expose star meshes
extern AEGfxVertexList* starMesh;
extern AEGfxVertexList* supernovaStarMesh;
extern HUD* hud;
extern bool gamePaused;

void Game_Load();
void Game_Initialize();
void Game_Update();
void Game_Draw();
void Game_Free();
void Game_Unload();