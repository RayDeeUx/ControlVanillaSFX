#include <Geode/Geode.hpp>
#include <Geode/modify/FMODAudioEngine.hpp>
#include <regex>

// const std::regex sfxRegex("s(?:\\d+)\\.ogg", std::regex_constants::icase);
const std::regex mp3Regex(".*\\.mp3", std::regex_constants::icase);
const std::list<std::string> vanillaSFX = {
	"achievement_01.ogg",
	"buyItem01.ogg",
	"buyItem03.ogg",
	"chest07.ogg",
	"chest08.ogg",
	"chestClick.ogg",
	"chestLand.ogg",
	"chestOpen01.ogg",
	"counter003.ogg",
	"crystal01.ogg",
	"door001.ogg",
	"door01.ogg",
	"door02.ogg",
	"endStart_02.ogg",
	"explode_11.ogg",
	"gold01.ogg",
	"gold02.ogg",
	"grunt01.ogg",
	"grunt02.ogg",
	"grunt03.ogg",
	"highscoreGet02.ogg",
	"magicExplosion.ogg",
	"playSound_01.ogg",
	"quitSound_01.ogg",
	"reward01.ogg",
	"secretKey.ogg",
	"unlockPath.ogg"
};
const std::filesystem::path resourcesPath = (std::filesystem::current_path() / "Resources");

using namespace geode::prelude;

class $modify(MyFMODAudioEngine, FMODAudioEngine) {
	void playEffect(gd::string p0, float p1, float p2, float p3) {
		if (Mod::get()->getSettingValue<bool>("enabled") && !std::regex_match(std::string(p0), mp3Regex)) {
			if (std::find(vanillaSFX.begin(), vanillaSFX.end(), std::string(p0)) == vanillaSFX.end()) {
				FMODAudioEngine::playEffect(p0, p1, p2, p3);
			} else {
				auto volume = (Mod::get()->getSettingValue<int64_t>("volume") / 100.0f);
				if (Mod::get()->getSettingValue<double>("volumeBoost") != 1.0) { volume *= Mod::get()->getSettingValue<double>("volumeBoost"); }

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
			FMODAudioEngine::playEffect(p0, p1, p2, p3);
		}
	}
};
