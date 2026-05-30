/*
 * BladeFall core module
 */

#include "Chat.h"
#include "ConfigValueCache.h"
#include "Player.h"
#include "ScriptMgr.h"

namespace
{
enum class BladeFallConfig
{
    ENABLED,
    LOGIN_MESSAGE,
    NUM_CONFIGS
};

enum BladeFallItems : uint32
{
    ITEM_NETHERWEAVE_BAG = 21841
};

enum BladeFallSpells : uint32
{
    SPELL_APPRENTICE_RIDING = 33388,
    SPELL_JOURNEYMAN_RIDING = 33391,
    SPELL_EXPERT_RIDING     = 34090,

    SPELL_GOLDEN_GRYPHON    = 32235,
    SPELL_TAWNY_WIND_RIDER  = 32243
};

struct BladeFallTeleportLocation
{
    uint32 Map;
    float X;
    float Y;
    float Z;
    float Orientation;
};

BladeFallTeleportLocation const StormwindLocation =
{
    0,
    -8833.38f,
    628.62f,
    94.00f,
    1.06f
};

BladeFallTeleportLocation const OrgrimmarLocation =
{
    1,
    1569.59f,
    -4397.63f,
    16.04f,
    0.54f
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

BladeFallConfigData bladeFallConfigData;

void LearnSpellIfMissing(Player* player, uint32 spellId)
{
    if (!player->HasSpell(spellId))
        player->learnSpell(spellId);
}

void GiveStarterBags(Player* player)
{
    player->StoreNewItemInBestSlots(ITEM_NETHERWEAVE_BAG, 4);
}

void LearnRidingAndFactionMount(Player* player)
{
    LearnSpellIfMissing(player, SPELL_APPRENTICE_RIDING);
    LearnSpellIfMissing(player, SPELL_JOURNEYMAN_RIDING);
    LearnSpellIfMissing(player, SPELL_EXPERT_RIDING);

    if (player->GetTeamId() == TEAM_ALLIANCE)
        LearnSpellIfMissing(player, SPELL_GOLDEN_GRYPHON);
    else
        LearnSpellIfMissing(player, SPELL_TAWNY_WIND_RIDER);
}

void TeleportToFactionCapital(Player* player)
{
    // Death Knights should complete their original introductory storyline.
    if (player->getClass() == CLASS_DEATH_KNIGHT)
        return;

    BladeFallTeleportLocation const& location =
        player->GetTeamId() == TEAM_ALLIANCE
            ? StormwindLocation
            : OrgrimmarLocation;

    player->TeleportTo(
        location.Map,
        location.X,
        location.Y,
        location.Z,
        location.Orientation);
}
}

class BladeFallPlayerScript : public PlayerScript
{
public:
    BladeFallPlayerScript()
        : PlayerScript(
            "BladeFallPlayerScript",
            {
                PLAYERHOOK_ON_LOGIN,
                PLAYERHOOK_ON_FIRST_LOGIN
            })
    {
    }

    void OnPlayerLogin(Player* player) override
    {
        if (!bladeFallConfigData.GetConfigValue<bool>(BladeFallConfig::ENABLED))
            return;

        ChatHandler(player->GetSession()).SendSysMessage(
            bladeFallConfigData.GetConfigValue(BladeFallConfig::LOGIN_MESSAGE));
    }

    void OnPlayerFirstLogin(Player* player) override
    {
        if (!bladeFallConfigData.GetConfigValue<bool>(BladeFallConfig::ENABLED))
            return;

        GiveStarterBags(player);
        LearnRidingAndFactionMount(player);
        TeleportToFactionCapital(player);

        ChatHandler(player->GetSession()).SendSysMessage(
            "BladeFall starter bonuses have been applied.");
    }
};

class BladeFallWorldScript : public WorldScript
{
public:
    BladeFallWorldScript()
        : WorldScript(
            "BladeFallWorldScript",
            {
                WORLDHOOK_ON_BEFORE_CONFIG_LOAD
            })
    {
    }

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
