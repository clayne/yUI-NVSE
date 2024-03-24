#pragma once

#include "TESBoundObject.hpp"
#include "TESModel.hpp"

class TESWaterForm;

class BGSPlaceableWater : public TESBoundObject {
public:
	enum WaterFlags {
		kWtrFlag_Reflects = 0x1,
		kWtrFlag_ReflectsActors = 0x2,
		kWtrFlag_ReflectsLand = 0x4,
		kWtrFlag_ReflectsLODLand = 0x8,
		kWtrFlag_ReflectsLODBuildings = 0x10,
		kWtrFlag_ReflectsLODTrees = 0x20,
		kWtrFlag_ReflectsSky = 0x40,
		kWtrFlag_ReflectsDynamicObjects = 0x80,
		kWtrFlag_ReflectsDeadBodies = 0x100,
		kWtrFlag_Refracts = 0x200,
		kWtrFlag_RefractsActors = 0x400,
		kWtrFlag_RefractsLand = 0x800,
		kWtrFlag_RefractsDeadBodies = 0x20000,
		kWtrFlag_RefractsDynamicObjects = 0x10000,
		kWtrFlag_SilhouetteReflections = 0x40000,
		kWtrFlag_Unk8000000 = 0x8000000,
		kWtrFlag_Depth = 0x10000000,
		kWtrFlag_ObjectTextureCoords = 0x20000000,
		kWtrFlag_AutoGenerated = 0x40000000,
		kWtrFlag_NoUnderwaterFog = 0x80000000,
	};

	TESModel		model;
	Bitfield32		uiWaterFlags;
	TESWaterForm*	pWaterType;

	static BGSPlaceableWater* CreateObject();

	// Reflections

	bool HasReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_Reflects);
	}

	void SetReflects(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_Reflects, abSet);
	}

	bool HasActorReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsActors);
	}

	void SetReflectsActors(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsActors, abSet);
	}

	bool HasLandReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsLand);
	}

	void SetReflectsLand(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsLand, abSet);
	}

	bool HasSkyReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsSky);
	}

	void SetReflectsSky(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsSky, abSet);
	}

	bool HasLODLandReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsLODLand);
	}

	void SetReflectsLODLand(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsLODLand, abSet);
	}

	bool HasLODBuildingReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsLODBuildings);
	}

	void SetReflectsLODBuildings(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsLODBuildings, abSet);
	}

	bool HasLODTreeReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsLODTrees);
	}

	void SetReflectsLODTrees(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsLODTrees, abSet);
	}

	bool HasLODReflection() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsLODLand) || uiWaterFlags.GetBit(kWtrFlag_ReflectsLODBuildings) || uiWaterFlags.GetBit(kWtrFlag_ReflectsLODTrees);
	}

	void SetLODReflection(bool abSet) {
		SetReflectsLODLand(abSet);
		SetReflectsLODBuildings(abSet);
		SetReflectsLODTrees(abSet);
	}

	bool ReflectsDynamicObjects() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsDynamicObjects);
	}

	void SetReflectsDynamicObjects(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsDynamicObjects, abSet);
	}

	bool ReflectsDeadBodies() const {
		return uiWaterFlags.GetBit(kWtrFlag_ReflectsDeadBodies);
	}

	void SetReflectsDeadBodies(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_ReflectsDeadBodies, abSet);
	}


	bool GetUnk8000000() const {
		return uiWaterFlags.GetBit(kWtrFlag_Unk8000000);
	}

	void SetUnk8000000(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_Unk8000000, abSet);
	}

	bool HasRefraction() const {
		return uiWaterFlags.GetBit(kWtrFlag_Refracts);
	}

	void SetRefracts(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_Refracts, abSet);
	}

	bool HasActorRefraction() const {
		return uiWaterFlags.GetBit(kWtrFlag_RefractsActors);
	}

	void SetRefractsActors(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_RefractsActors, abSet);
	}

	bool HasLandRefraction() const {
		return uiWaterFlags.GetBit(kWtrFlag_RefractsLand);
	}

	void SetRefractsLand(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_RefractsLand, abSet);
	}

	bool HasSilhouetteReflections() const {
		return uiWaterFlags.GetBit(kWtrFlag_SilhouetteReflections);
	}

	void SetSilhouetteReflections(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_SilhouetteReflections, abSet);
	}

	bool HasDepth() const {
		return uiWaterFlags.GetBit(kWtrFlag_Depth);
	}

	void SetDepth(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_Depth, abSet);
	}

	bool IsProceduralWater() const {
		return uiWaterFlags.GetBit(kWtrFlag_AutoGenerated);
	}

	void SetProceduralWater(bool abSet) {
		uiWaterFlags.SetBit(kWtrFlag_AutoGenerated, abSet);
	}

	void SetWaterType(TESWaterForm* apWaterType) {
		pWaterType = apWaterType;
	}
};

ASSERT_SIZE(BGSPlaceableWater, 0x50);