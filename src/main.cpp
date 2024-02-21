#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <regex>

const std::regex sfxRegex("s(?:\\d+)\\.ogg", std::regex_constants::icase);
const std::regex mp3Regex(".*\\.mp3", std::regex_constants::icase);

using namespace geode::prelude;

class $modify(MyFMODAudioEngine, FMODAudioEngine) {
	void playEffect(gd::string p0, float p1, float p2, float p3) {
		if (Mod::get()->getSettingValue<bool>("enabled") && !std::regex_match(std::string(p0), mp3Regex)) {
			if ((std::regex_match(std::string(p0), sfxRegex))) {
				FMODAudioEngine::playEffect(p0, p1, p2, p3);
			} else {
				auto volume = (Mod::get()->getSettingValue<int64_t>("volume") / (float)100.0);
				auto speed = (Mod::get()->getSettingValue<int64_t>("speed") / (float)100.0);
				if (m_sfxVolume < 0.11 && Mod::get()->getSettingValue<double>("volumeBoost") != 1.0) {
					volume *= Mod::get()->getSettingValue<double>("volumeBoost");
				}
				FMODAudioEngine::playEffect(p0, speed, p2, volume);
			}
		} else {
			FMODAudioEngine::playEffect(p0, p1, p2, p3);
		}
	}
};