//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cDrawing
	{
	public:

		enum eWallHackMode
		{
			WALLHACK_MODE_AXIS,
			WALLHACK_MODE_ALLIES,
			WALLHACK_MODE_ALL,
			WALLHACK_MODE_MAX
		};

		enum ePlayerBoxes
		{
			PLAYER_BOXES_OFF,
			PLAYER_BOXES_BORDER,
			PLAYER_BOXES_CORNER,
			PLAYER_BOXES_BORDER_FILLED,
			PLAYER_BOXES_CORNER_FILLED,
			PLAYER_BOXES_BORDER_3D,
			PLAYER_BOXES_CORNER_3D,
			PLAYER_BOXES_MAX
		};

		enum ePlayerBones
		{
			PLAYER_BONES_OFF,
			PLAYER_BONES_DOTS,
			PLAYER_BONES_LINES,
			PLAYER_BONES_MAX
		};

		enum ePlayerSnapLines
		{
			PLAYER_SNAPLINES_OFF,
			PLAYER_SNAPLINES_TOP,
			PLAYER_SNAPLINES_CENTER,
			PLAYER_SNAPLINES_BOTTOM,
			PLAYER_SNAPLINES_MAX
		};

		std::shared_ptr<sCvar> gWallHackMode = std::make_shared<sCvar>("Mode", std::vector<std::string>({ "Axis", "Allies", "All" }), WALLHACK_MODE_AXIS, WALLHACK_MODE_AXIS, WALLHACK_MODE_MAX);
		std::shared_ptr<sCvar> gPlayerBoxes = std::make_shared<sCvar>("Boxes", std::vector<std::string>({ "Off", "Border", "Corner", "Border Filled", "Corner Filled", "Border 3D", "Corner 3D" }), PLAYER_BOXES_OFF, PLAYER_BOXES_OFF, PLAYER_BOXES_MAX);
		std::shared_ptr<sCvar> gPlayerBones = std::make_shared<sCvar>("Bones", std::vector<std::string>({ "Off", "Dots", "Lines" }), PLAYER_BONES_OFF, PLAYER_BONES_OFF, PLAYER_BONES_MAX);
		std::shared_ptr<sCvar> gPlayerSnapLines = std::make_shared<sCvar>("Snaplines", std::vector<std::string>({ "Off", "Top", "Center", "Bottom" }), PLAYER_SNAPLINES_OFF, PLAYER_SNAPLINES_OFF, PLAYER_SNAPLINES_MAX);
		std::shared_ptr<sCvar> gPlayerInformation = std::make_shared<sCvar>("Information", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerWeapons = std::make_shared<sCvar>("Weapons", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAgents = std::make_shared<sCvar>("Agents", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gMissiles = std::make_shared<sCvar>("Missiles", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gItems = std::make_shared<sCvar>("Items", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerCrossHair = std::make_shared<sCvar>("Crosshair", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerCompass = std::make_shared<sCvar>("Compass", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gPlayerRadar = std::make_shared<sCvar>("Radar", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gColorAxisVisible = std::make_shared<sCvar>("Axis Visible", std::vector<std::string>(), ImVec4(ByteToFloat(0), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorAxisInvisible = std::make_shared<sCvar>("Axis Invisible", std::vector<std::string>(), ImVec4(ByteToFloat(0), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorAlliesVisible = std::make_shared<sCvar>("Allies Visible", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorAlliesInvisible = std::make_shared<sCvar>("Allies Invisible", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(255), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorAccents = std::make_shared<sCvar>("Accents", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(0), ByteToFloat(0), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorCrossHair = std::make_shared<sCvar>("Crosshair", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(0), ByteToFloat(255), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorText = std::make_shared<sCvar>("Text", std::vector<std::string>(), ImVec4(ByteToFloat(255), ByteToFloat(255), ByteToFloat(255), ByteToFloat(255)));
		std::shared_ptr<sCvar> gColorShadow = std::make_shared<sCvar>("Shadow", std::vector<std::string>(), ImVec4(ByteToFloat(0), ByteToFloat(0), ByteToFloat(0), ByteToFloat(255)));

		struct sTracer
		{
			bool bW2SSuccess;
			int iStartTime;
			ImVec2 vHitPos2D, vStartPos2D;
			ImVec3 vHitPos3D, vStartPos3D;
			ImVec4 cColorShadow, cColorHitMarker, cColorTracer;
			float flAlphaShadow, flAlphaHitMarker, flAlphaTracer;
		};

		std::vector<sTracer> vTracers;

		struct sCompass
		{
			float flCompassSize, flArrowSize;
			ImVec2 vCompassPosition, vArrowPosition[MAX_CLIENTS];
		} Compass;

		struct sRadar
		{
			float flScale = 10.0f, flRadarSize = 200.0f, flBlipSize = 8.0f;
			ImVec2 vRadarPosition, vBlipPosition[MAX_CLIENTS];
		} Radar;

		void DrawBorder(ImVec2 position, ImVec2 dimentions, bool border, bool shadow, ImVec4 color);
		void DrawCorners(ImVec2 position, ImVec2 dimentions, float length, bool shadow, ImVec4 color);
		void DrawBorder3D(ImVec2 corners2d[8], bool shadow, ImVec4 color);
		void DrawCorners3D(ImVec2 corners2d[8], float length, bool shadow, ImVec4 color);
		void DrawLine(ImVec2 point1, ImVec2 point2, bool shadow, ImVec4 color);
		void DrawBones(ImVec2 bones2d[BONE_MAX], bool skeleton, bool shadow, ImVec4 color);
		void DrawString(std::string text, ImVec2 position, bool shadow, ImVec4 color);
		void DrawPlayer(sCEntity* entity, ImVec2 bones2d[BONE_MAX], ImVec2 corners2d[8], ImVec2 center, float scale, float distance, std::string name, ImVec4 color);
		void DrawItem(std::string name, ImVec2 center, float distance, ImVec4 color);
		void DrawMissile(std::string name, ImVec2 center, float distance, ImVec4 color);
		void DrawAgent(sCEntity* entity, ImVec2 center, float distance, ImVec4 color);
		bool Calculate2D(ImVec3 bones3d[BONE_MAX], ImVec2 bones2d[BONE_MAX], ImVec2& position, ImVec2& dimentions);
		bool Calculate3D(sCEntity* entity, ImVec3 center, ImVec3 corners3d[8], ImVec2 corners2d[8]);
		void CalculateTracers();
		void DrawESP();
		void DrawCompass();
		void DrawRadar();
		void DrawCrosshair();
		void DrawTracers();
		void ColorPicker(std::string label, ImVec4& color);
	} extern _drawing;
}

//=====================================================================================