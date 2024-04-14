#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <regex>

// const std::regex sfxRegex("s(?:\\d+)\\.ogg", std::regex_constants::icase);
const std::regex mp3Regex(".*\\.mp3", std::regex_constants::icase);
const std::list<std::string> vanillaSFX = { "achievement_01.ogg", "buyItem01.ogg", "buyItem03.ogg", "chest07.ogg", "chest08.ogg", "chestClick.ogg", "chestLand.ogg", "chestOpen01.ogg", "counter003.ogg", "crystal01.ogg", "door001.ogg", "door01.ogg", "door02.ogg", "endStart_02.ogg", "explode_11.ogg", "gold01.ogg", "gold02.ogg", "grunt01.ogg", "grunt02.ogg", "grunt03.ogg", "highscoreGet02.ogg", "magicExplosion.ogg", "playSound_01.ogg", "quitSound_01.ogg", "reward01.ogg", "secretKey.ogg", "unlockPath.ogg" };
const std::filesystem::path resourcesPath = (std::filesystem::current_path() / "Resources");
// #ifdef GEODE_IS_ANDROID
// resourcesPath = (std::filesystem::current_path() / "resources");
// #endif
using namespace geode::prelude;

class $modify(MyFMODAudioEngine, FMODAudioEngine) {
	void playEffectAdvanced(gd::string p0, float p1, float p2, float p3, float p4, bool p5, bool p6, int p7, int p8, int p9, int p10, bool p11, int p12, bool p13, bool p14, int p15, int p16, float p17, int p18) {
		if (Mod::get()->getSettingValue<bool>("enabled") && !std::regex_match(std::string(p0), mp3Regex)) {
			if (std::find(vanillaSFX.begin(), vanillaSFX.end(), std::string(p0)) == vanillaSFX.end()) {
				FMODAudioEngine::playEffectAdvanced(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18);
			} else {
				log::info("std::filesystem::current_path(): {}", std::filesystem::current_path());
				auto volume = (Mod::get()->getSettingValue<int64_t>("volume") / 100.0f);
				if (Mod::get()->getSettingValue<double>("volumeBoost") != 1.0) { volume *= Mod::get()->getSettingValue<double>("volumeBoost"); }
				if (volume < 0.001f) { volume = 0.0001f; }

				auto system = FMODAudioEngine::sharedEngine()->m_system;

				FMOD::Channel* channel;
				FMOD::Sound* sound;

				std::filesystem::path sfxPath = (resourcesPath / p0.c_str());

				for (auto& p : CCFileUtils::sharedFileUtils()->getSearchPaths()) {
					auto path = std::filesystem::path(p.c_str()).parent_path();
					if (!strcmp(path.parent_path().filename().string().c_str(), "packs") || !strcmp(path.parent_path().filename().string().c_str(), "unzipped")) {
						if (std::filesystem::exists(path / std::string(p0))) {
							sfxPath = path / std::string(p0);
							break;
						}
					}
				}

				system->createSound(sfxPath.string().c_str(), FMOD_DEFAULT, nullptr, &sound);
				system->playSound(sound, nullptr, false, &channel);
				channel->setVolume(volume);
			}
		} else {
			FMODAudioEngine::playEffectAdvanced(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18);
		}
	}
};