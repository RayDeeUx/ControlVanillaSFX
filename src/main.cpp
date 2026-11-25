#include <Geode/modify/FMODAudioEngine.hpp>
#include <asp/fs/std.hpp>

static const std::array<std::string, 30> vanillaSFX = { "achievement_01.ogg", "buyItem01.ogg", "buyItem03.ogg", "chest07.ogg", "chest08.ogg", "chestClick.ogg", "chestLand.ogg", "chestOpen01.ogg", "counter003.ogg", "crystal01.ogg", "door001.ogg", "door01.ogg", "door02.ogg", "endStart_02.ogg", "explode_11.ogg", "gold01.ogg", "gold02.ogg", "grunt01.ogg", "grunt02.ogg", "grunt03.ogg", "highscoreGet02.ogg", "magicExplosion.ogg", "playSound_01.ogg", "quitSound_01.ogg", "reward01.ogg", "secretKey.ogg", "unlockPath.ogg", "unlockGauntlet.ogg" };
#ifdef GEODE_IS_WINDOWS
const std::filesystem::path resourcesPath = (std::filesystem::current_path() / "Resources");
#elif defined(GEODE_IS_MACOS)
#include <Geode/loader/Dirs.hpp>
const std::filesystem::path resourcesPath = (geode::dirs::getResourcesDir()).toStdPath().unwrapOr(std::filesystem::path{});
#elif defined(GEODE_IS_ANDROID)
const std::filesystem::path resourcesPath = "file:///android_asset"; // THANK YOU WEEBIFY
#elif defined(GEODE_IS_IOS)
std::string iosResourcePath();
const std::filesystem::path resourcesPath = iosResourcePath();
#endif

using namespace geode::prelude;

class $modify(MyFMODAudioEngine, FMODAudioEngine) {
	int playEffect(std::string p0, float p1, float p2, float p3) { // , float p4, bool p5, bool p6, int p7, int p8, int p9, int p10, bool p11, int p12, bool p13, bool p14, int p15, int p16, float p17, int p18
		const std::string& p0AsString = std::string(p0);
		if (!Mod::get()->getSettingValue<bool>("enabled") || !geode::utils::string::endsWith(p0AsString, ".ogg")) return FMODAudioEngine::sharedEngine()->playEffect(p0, p1, p2, p3); // , p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18
		if (std::ranges::find(vanillaSFX, p0AsString) == vanillaSFX.end()) return FMODAudioEngine::sharedEngine()->playEffect(p0, p1, p2, p3); // , p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18
		auto volume = (Mod::get()->getSettingValue<int64_t>("volume") / 100.0f);
		if (volume < 0.009f) return 0;
		if (Mod::get()->getSettingValue<double>("volumeBoost") != 1.0) volume *= Mod::get()->getSettingValue<double>("volumeBoost");

		auto system = FMODAudioEngine::sharedEngine()->m_system;

		FMOD::Channel* channel;
		FMOD::Sound* sound;

		std::filesystem::path sfxPath = (resourcesPath / p0AsString);

		std::error_code ec;
		for (auto& searchPaths : CCFileUtils::sharedFileUtils()->getSearchPaths()) {
			const std::filesystem::path& path = std::filesystem::path(searchPaths).parent_path();
			std::string grandparentPath = asp::fs::Path::fromStdPath(path.parent_path()).filename().value_or(asp::fs::PathView{}).string();
			if (grandparentPath != "packs" && grandparentPath != "unzipped") continue;
			const std::filesystem::path& proposedPath = (path / p0AsString);
			if (!std::filesystem::exists(proposedPath, ec)) continue;
			sfxPath = proposedPath;
			break;
		}

		system->createSound(asp::fs::Path::fromStdPath(sfxPath).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
		system->playSound(sound, nullptr, false, &channel);
		channel->setVolume(volume);

		return 0;
	}
};
