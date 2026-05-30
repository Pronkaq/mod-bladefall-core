/*
 * BladeFall core module
 */

#include "BladeFallStarterGear.h"

#include "Player.h"
#include "SharedDefines.h"

#include <initializer_list>

namespace
{
enum BladeFallArmorProficiencySpells : uint32
{
    SPELL_PLATE_MAIL = 750,
    SPELL_MAIL_ARMOR = 8737
};
enum BladeFallStarterItems : uint32
{
    // Cloth armor
    ITEM_CLOTH_HEAD      = 31220,
    ITEM_CLOTH_SHOULDERS = 31162,
    ITEM_CLOTH_CHEST     = 31158,
    ITEM_CLOTH_LEGS      = 31212,
    ITEM_CLOTH_WRISTS    = 24668,
    ITEM_CLOTH_HANDS     = 24664,

    // Leather armor
    ITEM_LEATHER_HEAD      = 31219,
    ITEM_LEATHER_SHOULDERS = 31161,
    ITEM_LEATHER_CHEST     = 31157,
    ITEM_LEATHER_WAIST     = 24773,
    ITEM_LEATHER_LEGS      = 31211,
    ITEM_LEATHER_FEET      = 31215,
    ITEM_LEATHER_WRISTS    = 24780,
    ITEM_LEATHER_HANDS     = 24776,

    // Mail armor
    ITEM_MAIL_HEAD      = 31218,
    ITEM_MAIL_SHOULDERS = 31160,
    ITEM_MAIL_CHEST     = 31156,
    ITEM_MAIL_WAIST     = 24886,
    ITEM_MAIL_LEGS      = 31210,
    ITEM_MAIL_FEET      = 31214,
    ITEM_MAIL_WRISTS    = 24893,
    ITEM_MAIL_HANDS     = 24889,

    // Plate armor
    ITEM_PLATE_HEAD      = 31217,
    ITEM_PLATE_SHOULDERS = 31159,
    ITEM_PLATE_CHEST     = 31155,
    ITEM_PLATE_WAIST     = 24998,
    ITEM_PLATE_LEGS      = 31209,
    ITEM_PLATE_FEET      = 31213,
    ITEM_PLATE_WRISTS    = 31284,
    ITEM_PLATE_HANDS     = 25001,

    // Weapons and shields
    ITEM_ONE_HAND_AXE      = 25208,
    ITEM_TWO_HAND_AXE      = 25222,
    ITEM_BOW               = 25250,
    ITEM_ONE_HAND_MACE     = 25124,
    ITEM_TWO_HAND_MACE     = 25138,
    ITEM_TWO_HAND_POLEARM  = 25236,
    ITEM_ONE_HAND_SWORD    = 25152,
    ITEM_TWO_HAND_SWORD    = 25166,
    ITEM_STAFF             = 31289,
    ITEM_MAIN_HAND_FIST    = 29371,
    ITEM_OFF_HAND_FIST     = 29372,
    ITEM_WAND              = 25292,
    ITEM_SHIELD            = 31287
};

void GiveItems(Player* player, std::initializer_list<uint32> itemIds)
{
    for (uint32 itemId : itemIds)
        player->StoreNewItemInBestSlots(itemId, 1);
}

void LearnSpellIfMissing(Player* player, uint32 spellId)
{
    if (!player->HasSpell(spellId))
        player->learnSpell(spellId);
}

void LearnStarterArmorProficiency(Player* player)
{
    switch (player->getClass())
    {
        case CLASS_WARRIOR:
        case CLASS_PALADIN:
        case CLASS_DEATH_KNIGHT:
            LearnSpellIfMissing(player, SPELL_PLATE_MAIL);
            break;
        case CLASS_HUNTER:
        case CLASS_SHAMAN:
            LearnSpellIfMissing(player, SPELL_MAIL_ARMOR);
            break;
        default:
            break;
    }
}

void RemoveExistingEquipment(Player* player)
{
    for (uint8 slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; ++slot)
    {
        if (player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
            player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
    }
}

void GiveClothArmor(Player* player)
{
    GiveItems(
        player,
        {
            ITEM_CLOTH_HEAD,
            ITEM_CLOTH_SHOULDERS,
            ITEM_CLOTH_CHEST,
            ITEM_CLOTH_LEGS,
            ITEM_CLOTH_WRISTS,
            ITEM_CLOTH_HANDS
        });
}

void GiveLeatherArmor(Player* player)
{
    GiveItems(
        player,
        {
            ITEM_LEATHER_HEAD,
            ITEM_LEATHER_SHOULDERS,
            ITEM_LEATHER_CHEST,
            ITEM_LEATHER_WAIST,
            ITEM_LEATHER_LEGS,
            ITEM_LEATHER_FEET,
            ITEM_LEATHER_WRISTS,
            ITEM_LEATHER_HANDS
        });
}

void GiveMailArmor(Player* player)
{
    GiveItems(
        player,
        {
            ITEM_MAIL_HEAD,
            ITEM_MAIL_SHOULDERS,
            ITEM_MAIL_CHEST,
            ITEM_MAIL_WAIST,
            ITEM_MAIL_LEGS,
            ITEM_MAIL_FEET,
            ITEM_MAIL_WRISTS,
            ITEM_MAIL_HANDS
        });
}

void GivePlateArmor(Player* player)
{
    GiveItems(
        player,
        {
            ITEM_PLATE_HEAD,
            ITEM_PLATE_SHOULDERS,
            ITEM_PLATE_CHEST,
            ITEM_PLATE_WAIST,
            ITEM_PLATE_LEGS,
            ITEM_PLATE_FEET,
            ITEM_PLATE_WRISTS,
            ITEM_PLATE_HANDS
        });
}
}

void GiveBladeFallStarterGear(Player* player)
{
    LearnStarterArmorProficiency(player);
    RemoveExistingEquipment(player);
	
    switch (player->getClass())
    {
        case CLASS_WARRIOR:
            GivePlateArmor(player);
            GiveItems(player, { ITEM_ONE_HAND_AXE, ITEM_SHIELD, ITEM_TWO_HAND_AXE });
            break;
        case CLASS_PALADIN:
            GivePlateArmor(player);
            GiveItems(player, { ITEM_ONE_HAND_MACE, ITEM_SHIELD, ITEM_TWO_HAND_MACE });
            break;
        case CLASS_HUNTER:
            GiveMailArmor(player);
            GiveItems(player, { ITEM_TWO_HAND_POLEARM, ITEM_BOW });
            break;
        case CLASS_ROGUE:
            GiveLeatherArmor(player);
            GiveItems(player, { ITEM_MAIN_HAND_FIST, ITEM_OFF_HAND_FIST });
            break;
        case CLASS_PRIEST:
            GiveClothArmor(player);
            GiveItems(player, { ITEM_STAFF, ITEM_WAND });
            break;
        case CLASS_DEATH_KNIGHT:
            GivePlateArmor(player);
            GiveItems(player, { ITEM_TWO_HAND_AXE });
            break;
        case CLASS_SHAMAN:
            GiveMailArmor(player);
            GiveItems(player, { ITEM_ONE_HAND_MACE, ITEM_SHIELD, ITEM_STAFF });
            break;
        case CLASS_MAGE:
            GiveClothArmor(player);
            GiveItems(player, { ITEM_STAFF, ITEM_WAND });
            break;
        case CLASS_WARLOCK:
            GiveClothArmor(player);
            GiveItems(player, { ITEM_STAFF, ITEM_WAND });
            break;
        case CLASS_DRUID:
            GiveLeatherArmor(player);
            GiveItems(player, { ITEM_STAFF });
            break;
        default:
            break;
    }
}
