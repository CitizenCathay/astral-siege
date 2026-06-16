/******************************************************************************/
/**
 * @file        HUD.cpp
 * @project     Astral Siege
 * @author      Javier Lee (Primary)
 * @brief       Manages the heads-up display (HUD) elements.
 * @copyright   All content © 2025 DigiPen Institute of Technology Singapore.
 *              All rights reserved.
 */
 /******************************************************************************/

#include "../Header/HUD.hpp"
#include "../Header/AudioPlayer.hpp"
#include "../Header/MeshManager.hpp"
#include "../Header/Utils.hpp"
#include "../Header/camera.hpp"
#include "../Header/currencyManager.hpp"
#include "../Header/WeaponManager.hpp"
#include "../Header/OuterPlanetManager.hpp"
#include "../Header/UIManager.hpp"
#include "../Header/DifficultyManager.hpp"

extern OuterPlanetManager outerPlanetManager;
extern int currentLevel;

HUD::HUD() :
	zoomCount(0),
	hasClick(false), // Initialize `hasClick` to `false`
	fontId(0),
	screenWidth(0.0f),
	screenHeight(0.0f) {
	for (int i = 0; i < TOTALUI; ++i) {
		hoverFlags[i] = false;  // Initialize all elements of `hoverFlags` to `false`
	}
}

int counter = 0; // zoom quick fix

void HUD::Init() 
{
	zoomCount = 0;

	fontId = AEGfxCreateFont("Assets/Fonts/ChakraPetch-Bold.ttf", 30);

	hudItems.reserve(HUDUI::TOTALUI); // capcity = TOTALUI, size = 0, during push_back no more run time memory alocation and assignment
	hudItems.clear();
	screenWidth = static_cast<f32>(AEGfxGetWindowWidth()) * 0.5f;
	screenHeight = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;
	MeshManager& mm = MeshManager::GetInstance();
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), true, TextureType::OPENSHOP));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 150.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::ZOOM));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 300.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::WPN1));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 450.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::TURR1));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 600.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::TURR2));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 750.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::SHIELD));
	hudItems.push_back(GameObject(AEVec2{ -screenWidth * 0.88f + 900.f, -screenHeight * 0.8f }, AEVec2{ 100.f, 100.f }, 0, mm.GetMesh(MeshType::QUAD), false, TextureType::SELL));
}


void HUD::Update() 
{
	AEVec2 mousePos = ASUtils::GetMousePositionInUI();

	// hovering function
	int itemIndex = 0;
	for (GameObject& item : hudItems) 
	{
		if (item.isActive) {
			if (AETestPointToRect(&mousePos, &item.pos, item.scale.x, item.scale.y)) 
			{
				hoverFlags[itemIndex] = true;
			}
			else 
			{
				hoverFlags[itemIndex] = false;
			}
		}
		++itemIndex;
	}
	
	// on click
	if (AEInputCheckTriggered(VK_LBUTTON)) 
	{
		CurrencyManager &cm = CurrencyManager::GetInstance();

		for (int i = 0; i < HUDUI::TOTALUI; ++i) 
		{
			if (hoverFlags[i]) 
			{
				GameObject& item = hudItems[i];
				if (i == HUDUI::SHOPUI) 
				{
					
					if (item.textureType == TextureType::OPENSHOP) 
					{
						// turn the texture from openshop to closeshop
						item.textureType = TextureType::CLOSESHOP;
						item.pTex = MeshManager::GetInstance().GetTexture(TextureType::CLOSESHOP);
						for (GameObject& hudElement : hudItems) 
						{
							hudElement.isActive = true;
						}
					}
					else 
					{
						item.textureType = TextureType::OPENSHOP;
						item.pTex = MeshManager::GetInstance().GetTexture(TextureType::OPENSHOP);
						for (GameObject& hudElement : hudItems)
						{
							hudElement.isActive = false;
						}
						item.isActive = true;
					}
				}
				else if (i == HUDUI::ZOOMUI) 
				{
					if (cm.GetCurrency() >= 100 && counter < 2)
					{
					ShopZoomOut(CameraManager::GetInstance().GetCurrentCamera());
					cm.DeductCurrency(100);
					++counter;
					AudioPlayer::GetInstance().PlayPurchase();

					// Progress to next level when zooming out
					CheckLevelProgression();
					extern int currentLevel;
					currentLevel = (currentLevel + 1) % 3;
					// Reinitialize planet obstacles for the new level
					outerPlanetManager.Clear();
					outerPlanetManager.Init(currentLevel);
					}
				}
				else if (i == HUDUI::WEAPONUI) 
				{
					// upgrade 1
					Missle* player = WeaponManager::GetInstance().GetPlayer();
					
					if (cm.GetCurrency() >= 30 && (player && player->GetMissleType() == Missile_BT)) 
					{
						cm.DeductCurrency(30);
						AudioPlayer::GetInstance().PlayPurchase();

						// turn the texture from wpn1 to wpn2
						item.textureType = TextureType::WPN2;
						item.pTex = MeshManager::GetInstance().GetTexture(TextureType::WPN2);

						player->UpgradeMissle();
					}
					
					// upgrade 2
					else if (cm.GetCurrency() >= 60 && (player && player->GetMissleType() == Missile_BT2 && !player->GetSpreadshot()))
					{
						cm.DeductCurrency(60);
						AudioPlayer::GetInstance().PlayPurchase();

						player->ActivateSpreadshot();
					}
				}
				else if (i == HUDUI::TURR1UI) 
				{
					WeaponManager::GetInstance().SetSelectedTurret(WeaponType::TURRET1_WP);
				}
				else if (i == HUDUI::TURR2UI) 
				{
					WeaponManager::GetInstance().SetSelectedTurret(WeaponType::TURRET2_WP);
				}
				else if (i == HUDUI::SHIELDUI) 
				{
						if (cm.GetCurrency() >= 30)
						{
							cm.DeductCurrency(30);
							AudioPlayer::GetInstance().PlayPurchase();	
							WeaponManager::GetInstance().ActivateShield();
						}
				}
				else if (i == HUDUI::SELLUI) 
				{
					WeaponManager& wm = WeaponManager::GetInstance();
					wm.SetSelling(!wm.GetSelling());
					if (wm.GetSelling())
					{
						// turn the texture from openshop to closeshop
						item.textureType = TextureType::SELL_HOVER;
						item.pTex = MeshManager::GetInstance().GetTexture(TextureType::SELL_HOVER);
						
					}
					else
					{
						item.textureType = TextureType::SELL;
						item.pTex = MeshManager::GetInstance().GetTexture(TextureType::SELL);
					}
				}
			}
		}
	}
}

void HUD::Draw() {
	int i = 0;
	CurrencyManager& cm = CurrencyManager::GetInstance();

	for (GameObject& item : hudItems) {
		if (item.isActive) {
			// Tell the Alpha Engine to get ready to draw something with texture.
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			// Set the the color to multiply to white, so that the sprite can
			// display the full range of colors (default is black).
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

			// Set the color to add to nothing, so that we don't alter the sprite's color
			if (hoverFlags[i] == true) 
			{
				const char* pText = "?";
				const char* amount = nullptr;
				if (i == HUDUI::SHOPUI) 
				{
					AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f); // set colour when hover is true
					if (item.textureType == TextureType::OPENSHOP)
					{
						pText = "Open Shop";
					}
					else {
						pText = "Close Shop";
					}
				}
			
				else if (i == HUDUI::ZOOMUI) 
				{
					pText = "Zoom Out";
					amount = "100 Coins";
					// set colour when hover is true
					if (cm.GetCurrency() >= 100 && zoomCount < 2)
					{
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
						
					}
					else
					{
						if (zoomCount >= 2) {
							pText = "Max Zoom";
							amount = nullptr;
						}
						
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
					}

					
				}
				else if (i == HUDUI::WEAPONUI) 
				{
					
					Missle* player = WeaponManager::GetInstance().GetPlayer();
					// set colour when hover is true
					int price = 0;
					if ((player && player->GetMissleType() == Missile_BT))
					{
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
						price = 30;
						pText = "Explosive Missiles";
						amount = "30 Coins";
					}
					else if ((player && player->GetMissleType() == Missile_BT2 && !player->GetSpreadshot())) {
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
						price = 60;
						pText = "Spreadshot";
						amount = "60 Coins";
					}
					else {
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
						pText = "Missile Maxed";
					}

					if (price > 0 && cm.GetCurrency() < price) {
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f); // not enough currency to buy
					}
				}
				else if (i == HUDUI::TURR1UI) 
				{
					pText = "Turret";
					amount = "20 Coins";
					// set colour when hover is true
					if (cm.GetCurrency() >= WeaponManager::GetInstance().GetTurretPrice(WeaponType::TURRET1_WP))
					{
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else 
					{
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
					}
				}
				else if (i == HUDUI::TURR2UI) 
				{
					pText = "Explosive Turret";
					amount = "50 Coins";
					// set colour when hover is true
					if (cm.GetCurrency() >= WeaponManager::GetInstance().GetTurretPrice(WeaponType::TURRET2_WP))
					{
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else 
					{
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
					}
				}
				else if (i == HUDUI::SHIELDUI) 
				{
					pText = "Shield";
					amount = "30 Coins";
					// set colour when hover is true
					if (cm.GetCurrency() >= 30) 
					{
						AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
					}
					else 
					{
						AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
					}
				}
				else if (i == HUDUI::SELLUI) 
				{
					
					AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
					pText = "Sell";
					amount = nullptr;
		
				}

				f32 width, height;
				
				AEGfxGetPrintSize(fontId, pText, 1.f, &width, &height);
				f32 offset = 0.5f * width;
				AEGfxPrint(fontId, pText, -offset + item.pos.x / screenWidth, 0.2f + item.pos.y / screenHeight, 1, 1, 1, 1, 1);
				if (amount != nullptr) {

					AEGfxGetPrintSize(fontId, amount, 1.f, &width, &height);
					f32 amountOffset = 0.5f * width;
					AEGfxPrint(fontId, amount, -amountOffset + item.pos.x / screenWidth, 0.125f + item.pos.y / screenHeight, 1, 1, 1, 1, 1);
				}
			}
			else 
			{
				AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); // reset the colours of the icon
				if (i == HUDUI::TURR1UI) {
					if (WeaponManager::GetInstance().GetSelectedTurret() == WeaponType::TURRET1_WP) {
						if (cm.GetCurrency() >= WeaponManager::GetInstance().GetTurretPrice(WeaponType::TURRET1_WP))
						{
							AEGfxSetColorToAdd(0.384f, 0.529f, 0.973f, 1.0f); // currently selected
						}
						else {
							AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
						}
					}
				}

				else if (i == HUDUI::TURR2UI) {
					if (WeaponManager::GetInstance().GetSelectedTurret() == WeaponType::TURRET2_WP) {
						if (cm.GetCurrency() >= WeaponManager::GetInstance().GetTurretPrice(WeaponType::TURRET2_WP))
						{
							AEGfxSetColorToAdd(0.384f, 0.529f, 0.973f, 1.0f); // currently selected
						}
						else {
							AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
						}
					}
				}
				else if (i == HUDUI::SELLUI) {
					if (WeaponManager::GetInstance().GetSelling()) {
						AEGfxSetColorToAdd(0.6f, 0.25f, 0.0f, 1.0f); // set colour when hover is true
					}
				}
			}

			// Set blend mode to AE_GFX_BM_BLEND, which will allow transparency.
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);

			// Tell Alpha Engine to use the texture stored in pTex
			AEGfxTextureSet(item.pTex, 0, 0);
			AEGfxSetTransform(item.GetUITransform().m);

			AEGfxMeshDraw(item.pMesh, AE_GFX_MDM_TRIANGLES);
		}
		++i;
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);
	}
}

void HUD::Reset()
{
	currentLevel = 0;
	counter = 0;
	zoomCount = 0;       // Reset the counter (for upgrades or zoom)
	hasClick = false;  // Reset click state

	// Reset hover flags
	for (int i = 0; i < TOTALUI; ++i) {
		hoverFlags[i] = false;
	}

	if (fontId > 0) {
		AEGfxDestroyFont(fontId);
		fontId = 0; // Set to invalid ID after destruction
	}
}