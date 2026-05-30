/*
 * BladeFall core module
 */

#include "Chat.h"
#include "ConfigValueCache.h"
#include "Player.h"
#include "ScriptMgr.h"

enum class BladeFallConfig
{
    ENABLED,
    LOGIN_MESSAGE,
    NUM_CONFIGS
};

class BladeFallConfigData : public ConfigValueCache<BladeFallConfig>
{
public:
    BladeFallConfigData() : ConfigValueCache(BladeFallConfig::NUM_CONFIGS) { }

    void BuildConfigCache() override
    {
        SetConfigValue<bool>(BladeFallConfig::ENABLED, "BladeFall.Enable", true);
        SetConfigValue<std::string>(
            BladeFallConfig::LOGIN_MESSAGE,
            "BladeFall.LoginMessage",
            "Welcome to BladeFall!");
    }
};

static BladeFallConfigData bladeFallConfigData;

class BladeFallPlayerScript : public PlayerScript
{
public:
    BladeFallPlayerScript() : PlayerScript("BladeFallPlayerScript", { PLAYERHOOK_ON_LOGIN }) { }

    void OnPlayerLogin(Player* player) override
    {
        if (!bladeFallConfigData.GetConfigValue<bool>(BladeFallConfig::ENABLED))
            return;

        ChatHandler(player->GetSession()).SendSysMessage(
            bladeFallConfigData.GetConfigValue(BladeFallConfig::LOGIN_MESSAGE));
    }
};

class BladeFallWorldScript : public WorldScript
{
public:
    BladeFallWorldScript() : WorldScript("BladeFallWorldScript", { WORLDHOOK_ON_BEFORE_CONFIG_LOAD }) { }

    void OnBeforeConfigLoad(bool reload) override
    {
        bladeFallConfigData.Initialize(reload);
    }
};

void AddBladeFallCoreScripts()
{
    new BladeFallPlayerScript();
    new BladeFallWorldScript();
}
