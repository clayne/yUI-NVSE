#include <functions.h>
#include <unordered_set>
#include <utility>
#include <PluginAPI.h>
#include <GameAPI.h>
#include <settings.h>
#include <GameSettings.h>

void PrintAndClearQueuedConsoleMessages()
{
	for (auto iter = queuedConsoleMessages.Begin(); !iter.End(); ++iter)
		Console_Print("%s", iter.Get());
	queuedConsoleMessages.DeleteAll();
}

void ConsoleQueueOrPrint(const char* str, const int len)
{
	if (*reinterpret_cast<ConsoleManager**>(0x11D8CE8) || g_dataHandler) { // g_dataHandler will be non-null if Deferred init has been called
		Console_Print("%s", str);
	}
	else {
		const auto errorStr = static_cast<char*>(GameHeapAlloc(len + 1));
		// ReSharper disable once CppDeprecatedEntity
		strcpy(errorStr, str);    // NOLINT(clang-diagnostic-deprecated-declarations)
		queuedConsoleMessages.Append(errorStr);
	}
}

bool Menu::GetTemplateExists(const char* templateName)
{
	for (ListNode<TemplateData>* node = menuTemplates.Head(); node; node = node->next) {
		if (node->data && (!_strcmpi(node->data->templateName, templateName))) { return true; }
	}
	return false;
}

__declspec(naked) void UIWidth()
{
	static const UInt32 retnAddr = 0x715D8D;
	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight()
{
	static const UInt32 retnAddr = 0x715DED;
	__asm
	{
		fld[ebp - 0x4]
		jmp		retnAddr
	}
}

double f1920 = 1920.0;
double f1080 = 1080.0;

__declspec(naked) void UIWidth2()
{
	static const UInt32 retnAddr = 0x71303D;
	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight2()
{
	static const UInt32 retnAddr = 0x713046;
	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}

__declspec(naked) void UIWidth3()
{
	static const UInt32 retnAddr = 0x7FBBE2;
	__asm
	{
		fld		ds : f1920
		jmp		retnAddr
	}
}

__declspec(naked) void UIHeight3()
{
	static const UInt32 retnAddr = 0x7FBBEB;
	__asm
	{
		fld		ds : f1080
		jmp		retnAddr
	}
}

StartMenuOption* yCM;

void __cdecl yCMCallback()
{
	MenuButton_DownloadsClick();
	//	ThisCall(0x7D74F0, (void *)( 0x11DAAC0 + 0x0B4), 0);
	//	static StartMenu* g_StartMenu = StartMenu::GetSingleton();
	//	((Tile*)(&(g_StartMenu->settingsSubMenu) + 0xC))->SetFloat(TraitNameToID("_enabled"), 1);
	//	g_StartMenu->GetChild("yCM")->GetChild("yCM")->SetFloat(kTileValue_visible, 0, 0);
}

void __fastcall AddyCMToSettingsMenu(BSSimpleArray<StartMenuOption*>* settingsMenuOptions, void* edx, StartMenuOption** menuDownloads)
{
	//		(*HUDColorSetting)->onSelection = HUDColorSettingCallback;
	//	(*menuDownloads)->displayString = "Mods";
	//	(*menuDownloads)->callback = yCMCallback;
	//	(*menuDownloads)->data = StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu;

	yCM = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu);
	settingsMenuOptions->Append(&yCM);
	settingsMenuOptions->Append(menuDownloads);


	//yCM = StartMenuOption::Create("Mods", yCMCallback, StartMenuOption::kMainMenu);

}

typedef Vector<ArrayElementL> TempElements;
__declspec(noinline) TempElements* GetTempElements()
{
	thread_local TempElements s_tempElements(0x100);
	return &s_tempElements;
}

const UInt8 kTypeListJmpTbl[] =
{
	255, 255, 255, 255, 128, 50, 28, 15, 16, 8, 9, 10, 11, 25, 26, 255, 255, 24, 13, 6, 7, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 48, 128, 39, 128, 5, 2, 58, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 1, 255, 255, 255, 32, 33, 34, 255, 0, 35, 36, 128, 51, 37, 38, 255, 40, 45, 3, 4, 49, 46, 47, 128, 255, 41, 14,
	255, 29, 30, 31, 128, 12, 52, 27, 255, 53, 54, 128, 17, 255, 19, 20, 128, 22, 55, 56, 57, 18, 21, 128, 128, 23, 42, 43, 44
};

tList<TESForm>* GetSourceList(UInt8 formType)
{
	if (formType == kFormType_TESRegion)
		return (tList<TESForm>*) & g_dataHandler->regionList->list;
	UInt8 listIdx = kTypeListJmpTbl[formType];
	if (listIdx >= 0x80) return NULL;
	return &((tList<TESForm>*) & g_dataHandler->packageList)[listIdx];
}

void GetLoadedType(UInt32 formType, int index, tList<TESForm>* outList, TempElements* tmpElements)
{
	if (formType == kFormType_TESObjectCELL)
	{
		TESObjectCELL** cells = g_dataHandler->cellArray.data;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			if (index != -1 && index != (*cells)->modIndex) continue;
			if (outList) outList->Insert(*cells);
			else tmpElements->Append(*cells);
		}
	}
	else if (formType == 301)
	{
		ListNode<TESWorldSpace>* wspcIter = g_dataHandler->worldSpaceList.Head();
		TESWorldSpace* wspc;
		do
		{
			if (!(wspc = wspcIter->data)) continue;
			ListNode<TESObjectREFR>* refrIter = wspc->cell->objectList.Head();
			do
			{
				auto refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kExtraData_MapMarker) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Insert(refr);
				else tmpElements->Append(refr);
			} while (refrIter = refrIter->next);
		} while (wspcIter = wspcIter->next);
	}
	else if (formType == 302)
	{
		TESObjectCELL** cells = g_dataHandler->cellArray.data;
		ListNode<TESObjectREFR>* refrIter;
		TESObjectREFR* refr;
		for (UInt32 count = g_dataHandler->cellArray.Length(); count; count--, cells++)
		{
			refrIter = (*cells)->objectList.Head();
			do
			{
				refr = refrIter->data;
				if (!refr || !refr->extraDataList.HasType(kExtraData_RadioData) || ((index != -1) && (index != refr->modIndex))) continue;
				if (outList) outList->Insert(refr);
				else tmpElements->Append(refr);
			} while (refrIter = refrIter->next);
		}
	}
	else if (formType < kFormType_Max)
	{
		tList<TESForm>* sourceList = GetSourceList(formType);
		TESForm* form;
		if (sourceList)
		{
			ListNode<TESForm>* iter = sourceList->Head();
			do
			{
				form = iter->data;
				if (!form || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Insert(form);
				else tmpElements->Append(form);
			} while (iter = iter->next);
		}
		else if (kTypeListJmpTbl[formType] == 0x80)
		{
			for (TESBoundObject* object = g_dataHandler->boundObjectList->first; object; object = object->next)
			{
				if ((object->typeID != formType) || ((index != -1) && (index != object->modIndex))) continue;
				if (outList) outList->Insert(object);
				else tmpElements->Append(object);
			}
		}
		else
		{
			for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter)
			{
				form = mIter.Get();
				if (!form || (form->typeID != formType) || ((index != -1) && (index != form->modIndex))) continue;
				if (outList) outList->Insert(form);
				else tmpElements->Append(form);
			}
		}
	}
}

void FillCraftingComponents()
{
	for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter)
	{
		if (mIter.Get()->typeID != 106) continue;
		const auto list = DYNAMIC_CAST(mIter.Get(), TESForm, TESRecipe);
		if (!list) continue;
		for (ListNode<RecipeComponent>* node = list->inputs.Head(); node; node = node->next)
			if (node->data && node->data->item) g_CraftingComponents.emplace(node->data->item);
	}
}

float TESObjectWEAP::GetWeaponValue(UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case eWeap_Type:				return this->eWeaponType;
	case eWeap_MinSpread:			return this->minSpread;
	case eWeap_Spread:				return this->spread;
	case eWeap_Proj:
	{
		if (BGSProjectile* pProj = this->projectile; pProj) return pProj->refID;
		break;
	}
	case eWeap_SightFOV:			return this->sightFOV;
	case eWeap_MinRange:			return this->minRange;
	case eWeap_Range:				return this->maxRange;
	case eWeap_AmmoUse:				return this->ammoUse;
	case eWeap_APCost:				return this->AP;
	case eWeap_CritDam:				return this->criticalDamage;
	case eWeap_CritChance:			return this->criticalPercent;
	case eWeap_CritEffect:
	{
		if (SpellItem* pSpell = this->criticalEffect; pSpell) return pSpell->refID;
		break;
	}
	case eWeap_FireRate:			return this->fireRate;
	case eWeap_AnimAttackMult:		return this->animAttackMult;
	case eWeap_RumbleLeft:			return this->rumbleLeftMotor;
	case eWeap_RumbleRight:			return this->rumbleRightMotor;
	case eWeap_RumbleDuration:		return this->rumbleDuration;
	case eWeap_RumbleWaveLength:	return this->rumbleWavelength;
	case eWeap_AnimShotsPerSec:		return this->animShotsPerSec;
	case eWeap_AnimReloadTime:		return this->animReloadTime;
	case eWeap_AnimJamTime:			return this->animJamTime;
	case eWeap_Skill:				return this->weaponSkill;
	case eWeap_ResistType:			return this->resistType;
	case eWeap_FireDelayMin:		return this->semiAutoFireDelayMin;
	case eWeap_FireDelayMax:		return this->semiAutoFireDelayMax;
	case eWeap_AnimMult:			return this->animMult;
	case eWeap_Reach:				return this->reach;
	case eWeap_IsAutomatic:			return this->IsAutomatic();
	case eWeap_HandGrip:			return this->HandGrip();
	case eWeap_ReloadAnim:			return this->reloadAnim;
	case eWeap_VATSChance:			return this->baseVATSChance;
	case eWeap_AttackAnim:			return this->AttackAnimation();
	case eWeap_NumProj:				return this->numProjectiles;
	case eWeap_AimArc:				return this->aimArc;
	case eWeap_LimbDamageMult:		return this->limbDamageMult;
	case eWeap_SightUsage:			return this->sightUsage;
	case eWeap_ReqStr:				return this->strRequired;
	case eWeap_ReqSkill:			return this->skillRequirement;
	case eWeap_LongBursts:			return this->weaponFlags2.IsSet(TESObjectWEAP::eFlag_LongBurst);
	case eWeap_Flags1:				return this->weaponFlags1.Get();
	case eWeap_Flags2:				return this->weaponFlags2.Get();
	case eWeap_HasScope:			return this->HasScope();
	case eWeap_IgnoresDTDR:			return this->IgnoresDTDR();
	case eWeap_SoundLevel:			return this->soundLevel;
	case eWeap_ClipSize:
	{
		if (const auto pClipRounds = DYNAMIC_CAST(this, TESForm, BGSClipRoundsForm); pClipRounds) return pClipRounds->clipRounds;
		break;
	}
	default: HALT("unknown weapon value"); break;
	}
	return false;
}

enum EBipedFlags {
	eBipedFlag_HasBackPack = 0x4,
	eBipedFlag_MediumArmor = 0x8,
	eBipedFlag_PowerArmor = 0x20,
	eBipedFlag_NonPlayable = 0x40,
	eBipedFlag_HeavyArmor = 0x80,
};

UInt32 TESObjectARMO::GetArmorValue(const UInt32 whichVal)
{
	if (!this) return false;
	switch (whichVal) {
	case 1:	return (&this->bipedModel)->bipedFlags & eBipedFlag_HeavyArmor ? 3 : (&this->bipedModel)->bipedFlags & eBipedFlag_MediumArmor ? 2 : 1;
	case 2:	return (&this->bipedModel)->bipedFlags & eBipedFlag_PowerArmor ? 1 : 0;
	case 3:	return (&this->bipedModel)->bipedFlags & eBipedFlag_HasBackPack ? 1 : 0;
	case 4:	return this->armorRating;
	case 5:	return static_cast<UInt32>(this->damageThreshold);
	case 6: return (&this->bipedModel)->partMask;
	default: HALT("unknown armor value"); break;
	}
	return false;
}

ExtraDataList* ExtraContainerChanges::EntryData::GetCustomExtra(UInt32 whichVal)
{
	if (extendData)
	{
		ListNode<ExtraDataList>* xdlIter = extendData->Head();
		do if (const auto xData = xdlIter->data; xData && xData->HasType(whichVal)) return xData;
		while (xdlIter = xdlIter->next);
	}
	return nullptr;
}

UInt8 ContWeaponHasAnyMod(ContChangesEntry* weaponInfo)
{
	const auto xData = weaponInfo->GetCustomExtra(kExtraData_WeaponModFlags);
	if (!xData) return 0;
	const auto xModFlags = GetExtraType((*xData), WeaponModFlags);
	if (!xModFlags) return 0;
	return xModFlags->flags;
}

float ContGetHealthPercent(ContChangesEntry* itemInfo)
{
	ExtraDataList* xData = itemInfo->GetCustomExtra(kExtraData_Health);
	if (!xData) return 0;
	auto* healthForm = DYNAMIC_CAST(itemInfo->type, TESForm, TESHealthForm);
	ExtraHealth* xHealth = nullptr;
	if (healthForm) xHealth = GetExtraType((*xData), Health);
	const float health = xHealth ? xHealth->health : static_cast<int>(healthForm->health);
	return (health / healthForm->health);
}

bool ContGetEquipped(ContChangesEntry* weaponInfo)
{
	auto xData = weaponInfo->GetCustomExtra(kExtraData_Worn);
	if (!xData) return false;
	auto* xWorn = GetExtraType((*xData), Worn);
	if (xWorn) return true;
	auto* xWornLeft = GetExtraType((*xData), WornLeft);
	if (xWornLeft) return true;
	return false;
}

UInt32 AlchemyItem::HasBaseEffectRestoresAV(int avCode)
{
	for (auto iter = this->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && !(setting->effectFlags & EffectSetting::kDetrimental))
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

UInt32 AlchemyItem::HasBaseEffectDamagesAV(const int avCode)
{
	for (auto iter = this->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && setting->effectFlags & EffectSetting::kDetrimental)
				return effect->magnitude;
		//				if (effect->conditions.Evaluate(g_player, nullptr, &eval, false)) return true;
	return 0;
}

bool AlchemyItem::IsAddictive()
{
	return this->withdrawalEffect && this->withdrawalEffect->refID ? true : false;
}

bool AlchemyItem::IsFood()
{
	return (this->alchFlags & 2) ? true : false;
}

bool AlchemyItem::IsMedicine()
{
	return (this->alchFlags & 4) ? true : false;
}

bool AlchemyItem::IsPoison()
{
	EffectItem* effItem;
	EffectSetting* effSetting = nullptr;
	ListNode<EffectItem>* iter = magicItem.list.list.Head();
	do
	{
		if (!(effItem = iter->data)) continue;
		effSetting = effItem->setting;
		if (effSetting && !(effSetting->effectFlags & 4)) return false;
	} while (iter = iter->next);
	return effSetting != nullptr;
}

bool AlchemyItem::IsFoodAlt()
{
	return this->HasBaseEffectRestoresAV(kAVCode_Hunger) && this->HasBaseEffectRestoresAV(kAVCode_Hunger) > this->HasBaseEffectRestoresAV(kAVCode_Dehydration) ? true : false;
}

bool AlchemyItem::IsWaterAlt()
{
	return this->HasBaseEffectRestoresAV(kAVCode_Dehydration) && this->HasBaseEffectRestoresAV(kAVCode_Hunger) == 0 ? true : false;
}

bool HasBaseEffectChangesAV(TESForm* form, const int avCode)
{
	auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
	if (!armor) return false;
	auto enchantable = &armor->enchantable;
	if (!enchantable) return false;
	auto enchantment = enchantable->enchantItem;
	if (!enchantment) return false;
	for (auto iter = enchantment->magicItem.list.list.Begin(); !iter.End(); ++iter)
		if (auto effect = iter.Get(); effect->GetSkillCode() == avCode)
			if (const auto setting = effect->setting; setting && setting->effectFlags & EffectSetting::kRecover)
				return true;
	return false;
}


__declspec(naked) TESForm* __stdcall LookupFormByRefID(UInt32 refID)
{
	__asm
	{
		mov		ecx, ds: [0x11C54C0]
		mov		eax, [esp + 4]
		xor edx, edx
		div		dword ptr[ecx + 4]
		mov		eax, [ecx + 8]
		mov		eax, [eax + edx * 4]
		test	eax, eax
		jz		done
		mov		edx, [esp + 4]
		ALIGN 16
	iterHead:
		cmp[eax + 4], edx
		jz		found
		mov		eax, [eax]
		test	eax, eax
		jnz		iterHead
		retn	4
		found:
		mov		eax, [eax + 8]
	done :
		retn	4
	}
}

TESForm* GetRefFromString(char* mod, char* id)
{
	const auto itemID = (g_dataHandler->GetModIndex(mod) << 24) | strtoul(id, nullptr, 0);
	return LookupFormByRefID(itemID);
}

bool IsInListRecursive(TESForm* item, TESForm* list)
{
	if (!item || !list) return false;
	if (item->refID == list->refID) return true;
	if (list->typeID != 85) return false;
//	if (dynamic_cast<BGSListForm*>(list)->GetIndexOf(item) >= 0) return true;
	const auto bgslist = DYNAMIC_CAST(list, TESForm, BGSListForm);
	if (!bgslist) return false;
	for (auto iter = bgslist->list.Begin(); !iter.End(); ++iter) {
		if (iter.Get() && IsInListRecursive(item, iter.Get())) return true;
	}
	return false;
}

bool IsCraftingComponent(TESForm* form)
{
	return (g_CraftingComponents.find(form) != g_CraftingComponents.end());
}

bool FindStringCI(const std::string& strHaystack, const std::string& strNeedle)
{
	const auto it = ra::search(strHaystack, strNeedle, [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }).begin();
	return it != strHaystack.end();
}

void Log(const std::string& msg)
{
	_MESSAGE("%s", msg.c_str());
	if (g_logLevel == 2)
		Console_Print("yUI: %s", msg.c_str());
}

void DebugLog(const std::string& msg)
{
	_MESSAGE("%s", msg.c_str());
	if (g_logLevel == 2)
		Console_Print("yUI: %s", msg.c_str());
}

int HexStringToInt(const std::string& str)
{
	char* p;
	const auto id = strtoul(str.c_str(), &p, 16);
	if (*p == 0)
		return id;
	return -1;
}

void DebugPrint(const std::string& str)
{
	if (g_logLevel == 1)
		Console_Print("yUI: %s", str.c_str());
	Log(str);
}

bool IsPlayersOtherAnimData(AnimData* animData)
{
	if (g_thePlayer->IsThirdPerson() && animData == g_thePlayer->firstPersonAnimData)
		return true;
	if (!g_thePlayer->IsThirdPerson() && animData == g_thePlayer->baseProcess->GetAnimData())
		return true;
	return false;
}

AnimData* GetThirdPersonAnimData(AnimData* animData)
{
	if (animData == g_thePlayer->firstPersonAnimData)
		return g_thePlayer->baseProcess->GetAnimData();
	return animData;
}

void PatchPause(UInt32 ptr)
{
	SafeWriteBuf(ptr, "\xEB\xFE", 2);
}

void SetUIStringFull(char* tochange, char* changeto, UInt32 tileValue)
{
	TileMenu::GetTileMenu(kMenuType_HUDMain)->SetStringRecursive(tileValue, changeto, tochange);
}

void Tile::SetStringRecursive(const UInt32 tileValue, const char* changeto, const char* tochange) {
//	if (!tochange || !_strcmpi(this->GetValue(tileValue)->str, tochange))
		this->SetString(tileValue, changeto, false);
		this->HandleChange(tileValue);
	for (auto iter = this->children.Head(); iter; iter = iter->next)
		iter->data->SetStringRecursive(tileValue, changeto, tochange);
}

extern TileMenu** g_tileMenuArray;


TileMenu* TileMenu::GetTileMenu(const UInt32 menuID)
{
	return menuID ? g_tileMenuArray[menuID - kMenuType_Min] : nullptr;
}


Tile* Tile::InjectUIXML(const char* str)
{
	return FileExists(str) ? this->ReadXML(str) : nullptr;
}

void __fastcall CursorTileSetStringValue(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* src, char propagate)
{
	tile->SetFloat(kTileValue_zoom, -1, propagate);
	tile->SetFloat(kTileValue_systemcolor, 1, propagate);
}

void __fastcall CursorTileSetIntValue(Tile* tile, void* dummyEDX, enum TileValues tilevalue, int value)
{
	tile->SetFloat(kTileValue_visible, value, 1);
	ThisCall(0xA0B350, InterfaceManager::GetSingleton()->cursor, 1, 0);
}

bool __fastcall FileExists(const char* path)
{
	const auto attr = GetFileAttributes(path);
	return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

char* __fastcall StrFromINI(DWORD *address)
{
	return address ? reinterpret_cast<char*>(address[1]) : nullptr;
}

Setting* GameSettingFromString(const std::string& settingName)
{
	Setting* setting = nullptr;
	if (auto gmsts = GameSettingCollection::GetSingleton(); gmsts && gmsts->GetGameSetting(settingName.c_str(), &setting) && setting) return setting;
}

std::string GetStringGameSetting(Setting* setting)
{
	if (setting->GetType() == Setting::kSetting_String)	return std::string(setting->data.str);
}

std::string GetStringFromGameSettingFromString(const std::string& settingName)
{
	if (const auto setting = GameSettingFromString(settingName); setting)
		return GetStringGameSetting(setting);
}

void* __fastcall FixGetDroppedWeapon(ExtraDataList* extradatalist)
{
	auto* xDropped = (ExtraDroppedItemList*)(extradatalist->GetByType(kExtraData_DroppedItemList));
	if (!xDropped) return nullptr;
	for (auto iter = xDropped->itemRefs.Head(); iter; iter = iter->next)
	{
		auto worldref = iter->data;
		auto entry = ContChangesEntry::Create(worldref->baseForm, 1);
		ContainerMenu::GetSingleton()->rightItems.Insert(entry, worldref->GetTheName());
		
//		entry->extendData = tList::Init(8)
//		tList::Init()
//		ThisCall(0x41DE40, *&entry, worldref);
		
/*		TESHealthForm* healthForm = DYNAMIC_CAST(worldref->baseForm, TESForm, TESHealthForm);
		ExtraHealth* xHealth = nullptr;
		if (healthForm)	xHealth = (ExtraHealth*)worldref->extraDataList.GetByType(kExtraData_Health);
		//(float) entry->GetCustomExtra(kExtraData_Health)-> = xHealth->health;
		ThisCall(0x419970, *&entry, xHealth->health);*/
		
	}
	return nullptr;
}