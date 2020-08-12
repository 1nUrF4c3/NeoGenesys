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
			if (IsUserRegistered(GetCurrentSession(), i) || CharacterInformation[i].iInfoValid)
			{
				GetPlayerAddr(&PlayerList.NetAddr[i], GetCurrentSession(), i);

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
					if (ImGui::BeginMenu("Crash", crash_exploit::send_connectivity_test(i)))
					{
						ImGui::PushItemWidth(100.0f);
						ImGui::InputText("", PlayerList.szCrashMessage, 1024);
						ImGui::PopItemWidth();

						ImGui::SameLine();
						if (ImGui::Button("Execute"))
						{
							std::string szCrashMessage(PlayerList.szCrashMessage);

							if (!szCrashMessage.empty())
							{
								sNetAddr NetAddr;
								GetPlayerAddr(&NetAddr, GetCurrentSession(), i);

								szCrashMessage = acut::FindAndReplaceString(szCrashMessage, "%attacker", PlayerName[t6::Party_FindMemberByXUID((SessionData*)NeoGenesys::GetCurrentSession(), t6::Live_GetXuid((ControllerIndex_t)0))].szName);
								szCrashMessage = acut::FindAndReplaceString(szCrashMessage, "%victim", PlayerName[i].szName);
								szCrashMessage = acut::FindAndReplaceString(szCrashMessage, "%ip", VariadicText("%u.%u.%u.%u",
									(BYTE)NetAddr.szIPAddress[0],
									(BYTE)NetAddr.szIPAddress[1],
									(BYTE)NetAddr.szIPAddress[2],
									(BYTE)NetAddr.szIPAddress[3]));

								Cbuf_AddText(VariadicText("say \"%s\"\n", szCrashMessage.c_str()));
							}

							std::thread(crash_exploit::send_crash, i).detach();
							PlayerList.bWriteLog = true;
						}
						ImGui::EndMenu();
					}

					if (ImGui::MenuItem("Ignore", (const char*)0, &_targetList.Priorities[i].bIsIgnored))
					{
						PlayerList.bWriteLog = true;
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Copy ID"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(std::to_string(i).c_str());
						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					if (ImGui::MenuItem("Copy Name"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(PlayerName[i].szName);
						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					if (ImGui::MenuItem("Copy IP Address"))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(VariadicText("%u.%u.%u.%u",
							(BYTE)PlayerList.NetAddr[i].szIPAddress[0],
							(BYTE)PlayerList.NetAddr[i].szIPAddress[1],
							(BYTE)PlayerList.NetAddr[i].szIPAddress[2],
							(BYTE)PlayerList.NetAddr[i].szIPAddress[3]).c_str());

						ImGui::LogFinish();

						PlayerList.bWriteLog = true;
					}

					ImGui::EndPopup();
				} ImGui::NextColumn();

				ImGui::Text(PlayerName[i].szName);

				if (ImGui::OpenPopupOnItemClick(std::to_string(i).c_str()))
				{
					PlayerList.bWriteLog = true;
				} ImGui::NextColumn();

				ImGui::Text(VariadicText("%u.%u.%u.%u",
					(BYTE)PlayerList.NetAddr[i].szIPAddress[0],
					(BYTE)PlayerList.NetAddr[i].szIPAddress[1],
					(BYTE)PlayerList.NetAddr[i].szIPAddress[2],
					(BYTE)PlayerList.NetAddr[i].szIPAddress[3]).c_str());

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