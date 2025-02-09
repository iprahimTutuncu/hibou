#include "pch.h"
#include "thsan/hud/debug_hud/terrain_panel_debug.h"
#include "imgui.h"
#include "thsan/graphics/terrain/terrain.h"

Thsan::TerrainPanelDebug::TerrainPanelDebug(TerrainOptions* pTerrainOptions):
	terrainOptions(pTerrainOptions)
{
}

void Thsan::TerrainPanelDebug::drawContent()
{
	ImGui::Text("Terrain Debug Panel");

	ImGui::DragFloat3("Position", terrainOptions->position, 1.f);
	ImGui::DragFloat2("Normal Blur", terrainOptions->normalBlurr, 0.1f);

	ImGui::DragInt("Terrain Size", &terrainOptions->terrainSize, 1);

	ImGui::DragInt("Normal Blur Iteration", &terrainOptions->normalBlurIteration, 1);
	ImGui::DragFloat("LOD", &terrainOptions->LOD, 0.01f, 0.0f, 4.f);
	ImGui::DragFloat("LOD Start At", &terrainOptions->LODStartAt, 1.f, 0.0f, 1024.f);

	ImGui::DragFloat("Angle", &terrainOptions->angle, 0.01f, -2 * 3.14159265358979f, 2 * 3.14159265358979f);
	ImGui::DragFloat("Horizon", &terrainOptions->horizon, 1.f);
	ImGui::DragFloat("Scale Height", &terrainOptions->scaleHeight, 0.5f);
	ImGui::DragFloat("Sprite Render Distance", &terrainOptions->spriteRenderDistance, 1.0f);
	ImGui::DragFloat("Terrain Render Distance", &terrainOptions->terrainRenderDistance, 1.0f);
	ImGui::DragFloat("Shininess", &terrainOptions->shininess, 0.1f);
	ImGui::DragInt("Blur Type", &terrainOptions->blurType, 1);

	ImGui::InputText("Albedo Texture Path", terrainOptions->albedoTexturePath, sizeof(terrainOptions->albedoTexturePath));
	ImGui::InputText("Height Texture Path", terrainOptions->heightTexturePath, sizeof(terrainOptions->heightTexturePath));
	ImGui::InputText("Collision Height Texture Path", terrainOptions->collisionHeightTexturePath, sizeof(terrainOptions->collisionHeightTexturePath));
	ImGui::InputText("Collision Mask Texture Path", terrainOptions->collisionMaskTexturePath, sizeof(terrainOptions->collisionMaskTexturePath));

}
