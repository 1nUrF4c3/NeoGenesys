//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"
#include "RapidXML/rapidxml_wrap.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cProfiler
	{
	public:

		acut::XmlDoc<char> XML;

		bool SaveProfile(const std::string& path);
		bool LoadProfile(const std::string& path);
		void DisableAll();
	} extern _profiler;
}

//=====================================================================================
