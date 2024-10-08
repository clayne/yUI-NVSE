#pragma once

enum Context {
	MC_UNK0						= 0,
	MC_UNK1						= 1,
	MC_PERF_TIMERS				= 2,
	MC_MEMPOOLS					= 3,
	MC_RENDERER					= 4,
	MC_SHADERS					= 5,
	MC_THREAD_SAFE_STRUCT		= 6,
	MC_EFFECTS					= 7,
	MC_STRING					= 8,
	MC_SETTINGS					= 9,
	MC_SYSTEM					= 10,
	MC_AUDIO					= 11,
	MC_FONTS					= 12,
	MC_MENU						= 13,
	MC_LOCAL_MAP				= 14,
	MC_UNK_15					= 15, // Texture palettes, NiStaticDataManager
	MC_HAVOK					= 16,
	MC_SAVE_LOAD				= 17,
	MC_UNK_18					= 18,
	MC_ARCHIVE_MANAGER			= 19,
	MC_MOVIE_PLAYER				= 20,
	MC_SCRIPT					= 21,
	MC_FILE_TES					= 22,
	MC_FILE_BUFFER				= 23,
	MC_FILE_CACHE				= 24,
	MC_SCENEGRAPH				= 25,
	MC_CELL						= 26,
	MC_TERRAIN_LAND				= 27,
	MC_TERRAIN_LOD_MANAGER		= 28,
	MC_WATER					= 29,
	MC_TREES_MODELS				= 30,
	MC_TREES_SPEEDTREE			= 31,
	MC_GRIDCELL					= 32,
	MC_SKY						= 33,
	MC_LOD_SYSTEM				= 34,
	MC_LOD_LAND					= 35,
	MC_LOD_OBJECTS				= 36,
	MC_LOD_TREES				= 37,
	MC_RENDER_38				= 38,
	MC_IMAGESPACE				= 39,
	MC_HAIR_SHADER				= 40,
	MC_RENDER_41				= 41, // Unused
	MC_PROCESS_MANAGER			= 42,
	MC_COMBAT_SYSTEM			= 43,
	MC_LOADED_REF_COLLECTION	= 44,
	MC_PATHING					= 45,
	MC_NAVMESH					= 46,
	MC_DATAHANDLER				= 47,
	MC_FORMS					= 48,
	MC_REFERENCES				= 49,
	MC_ACTORS					= 50, // Models, queued models, particles, blood
	MC_ANIMATION				= 51,
	MC_PLAYER					= 52,
	MC_DIALOGUE					= 53,
	MC_INVENTORY				= 54,
	MC_FACEGEN_0				= 55,
	MC_FACEGEN_1				= 56,
	MC_FACEGEN_2				= 57,
	MC_UNK_58					= 58,
	MC_UNK_59					= 59,
	MC_UNK_60					= 60,
	MC_UNK_61					= 61,
	MC_UNK_62					= 62,
	MC_UNK_63					= 63,
	MC_UNK_64					= 64,
	MC_DEFAULT					= 65,
};