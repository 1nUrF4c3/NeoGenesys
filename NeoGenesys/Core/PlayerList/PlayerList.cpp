//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cPlayerList _playerList;

	void cPlayerList::DrawPlayerList()
	{
		if (PlayerList.bWriteLog)
		{
			ImGui::LogToFile();
			PlayerList.bWriteLog = false;
		}

		ImGui::SetNextWindowSize(ImVec2(400.0f, 415.0f));
		ImGui::Begin("PLAYER LIST", &PlayerList.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (CharacterInfo[i].iInfoValid)
			{
				GetPlayerAddr(&PlayerList.NetAddr[i], GetCurrentSession(), ClientInfo[i].iClientNum);

				ImGui::PushID(i);

				if (ImGui::RadioButton("", &_targetList.iRiotShieldTarget, i))
				{
					PlayerList.bWriteLog = true;
				} ImGui::PopID(); ImGui::SameLine();

				ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue);

				if (ImGui::Checkbox("", (bool*)&_targetList.vIsTarget[i]))
				{
					PlayerList.bWriteLog = true;
				} ImGui::PopID(); ImGui::SameLine();

				if (IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
				{
					ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue * 2);

					if (ImGui::Button("Crash", ImVec2(50.0f, 0.0f)))
					{
						Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[i], 0, "\x5E\x01\x3D\x3D\xFF");
						PlayerList.bWriteLog = true;
					} ImGui::PopID(); ImGui::SameLine();
				}

				ImGui::PushItemWidth(150.0f);
				ImGui::InputText(VariadicText("%i: %s", ClientInfo[i].iClientNum, ClientInfo[i].szName).c_str(),
					(LPSTR)VariadicText("%u.%u.%u.%u",
						(BYTE)PlayerList.NetAddr[i].szIP[0],
						(BYTE)PlayerList.NetAddr[i].szIP[1],
						(BYTE)PlayerList.NetAddr[i].szIP[2],
						(BYTE)PlayerList.NetAddr[i].szIP[3]).c_str(),
					1024, ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();

				if (IsSessionHost(GetCurrentSession(), i))
				{
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, 
						IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum) ? 
						ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : 
						ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
					ImGui::Text("[HOST]");
					ImGui::PopStyleColor();
				}
			}
		}

		ImGui::EndChild();
		ImGui::End();
	}
}

//=====================================================================================