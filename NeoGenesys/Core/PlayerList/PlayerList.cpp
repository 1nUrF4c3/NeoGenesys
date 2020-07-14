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

		ImGui::SetNextWindowSize(ImVec2(400.0f, 440.0f));
		ImGui::Begin("PLAYER LIST", &gPlayerList->Current.bValue, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);

		ImGui::Columns(3, "PlayerList");
		ImGui::Separator();

		ImGui::Text("ID"); ImGui::NextColumn();
		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("IP Address"); ImGui::NextColumn();

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (CharacterInfo[i].iInfoValid)
			{
				GetPlayerAddr(&PlayerList.NetAddr[i], GetCurrentSession(), ClientInfo[i].iClientNum);

				ImGui::Separator();
				ImGui::PushID(i);

				if (ImGui::Selectable(std::to_string(i).c_str(), &_targetList.Priorities[i].bIsPrioritized, ImGuiSelectableFlags_SpanAllColumns))
				{
					PlayerList.bWriteLog = true;
				}

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

				if (ImGui::BeginPopupContextItem(std::to_string(i).c_str()))
				{
					if (IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
					{
						if (ImGui::Selectable("Crash"))
						{
							Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[i], 0, "\x5E\x01\x3D\x3D\xFF");

							PlayerList.bWriteLog = true;
						}
					}

					if (ImGui::Selectable("Ignore"))
					{
						_targetList.Priorities[i].bIsIgnored = !_targetList.Priorities[i].bIsIgnored;

						PlayerList.bWriteLog = true;
					}

					if (_targetList.Priorities[i].bIsIgnored)
					{
						ImGui::SameLine();
						ImGui::RenderCheckMark(ImGui::GetCurrentWindow()->DrawList, ImGui::GetCurrentWindow()->DC.CursorPos, ImGui::GetColorU32(ImGuiCol_CheckMark), ImGui::GetCurrentContext()->FontSize);
						ImGui::NewLine();
					}

					ImGui::Separator();

					if (ImGui::Selectable("Copy ID"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(std::to_string(i).c_str());
						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					if (ImGui::Selectable("Copy Name"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(ClientInfo[i].szName);
						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					if (ImGui::Selectable("Copy IP Address"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(VariadicText("%u.%u.%u.%u",
							(BYTE)PlayerList.NetAddr[i].szIP[0],
							(BYTE)PlayerList.NetAddr[i].szIP[1],
							(BYTE)PlayerList.NetAddr[i].szIP[2],
							(BYTE)PlayerList.NetAddr[i].szIP[3]));

						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					ImGui::EndPopup();
				} ImGui::NextColumn();

				ImGui::Text(ClientInfo[i].szName);

				if (ImGui::OpenPopupOnItemClick(std::to_string(i).c_str()))
				{
					PlayerList.bWriteLog = true;
				} ImGui::NextColumn();

				ImGui::Text(VariadicText("%u.%u.%u.%u",
					(BYTE)PlayerList.NetAddr[i].szIP[0],
					(BYTE)PlayerList.NetAddr[i].szIP[1],
					(BYTE)PlayerList.NetAddr[i].szIP[2],
					(BYTE)PlayerList.NetAddr[i].szIP[3]));

				if (ImGui::OpenPopupOnItemClick(std::to_string(i).c_str()))
				{
					PlayerList.bWriteLog = true;
				} ImGui::NextColumn();

				ImGui::PopID();
			}
		}

		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::EndChild();
		ImGui::End();
	}
}

//=====================================================================================