#include "BaseModels/ModFeature.hpp"

#include <cstdint>

class GameState : public ModFeature {
public:
	bool Init(uintptr_t moduleBase);
	bool Deinit();

#pragma pack(push, 1)
	struct GameOptions {
		uintptr_t vTable;
		int32_t someFlag;
		float someFloat;
		int32_t unk1;
		int32_t unk2;
		int32_t hidden;
		int32_t hiddenHeight;
		int32_t sudden;
		int32_t unk3;
		int32_t suddenHeight;
		int32_t lift;
		int32_t liftHeight;
		int32_t unk4;
		int32_t randomModeL;
		int32_t randomModeR;
		int32_t mirrorL;
		int32_t mirrorR;
		int32_t rRan;
		int32_t gaugeType;
		int32_t autoScratch;
		int32_t legacyNote;
		int32_t keyAssist;
		int32_t anyKey;
		int32_t unk5;
		int32_t targetDeltaPosition;
		int32_t targetDeltaStyle;
		int32_t fastSlowPosition;
		int32_t fastSlowSeparateDisplay;
		int32_t judgementPosition;
		int32_t adjust;
		bool judgementCountDisplay;
		bool autoadjust;
		int16_t unk6;
		int32_t bgaDisplay;
		int32_t bestDeltaDisplay;
		int32_t autoscratchDisplay;
		int32_t gaugeStyle;
		int32_t laneBrightness;
		int32_t classicHispeed;
	};
	struct GeneralOptions {
		GameOptions optionsP1;
		GameOptions optionsP2;
		GameOptions optionsDP;
	};
#pragma pack(pop)
	static_assert(sizeof(GameOptions) == 0x98);
	const GeneralOptions& GetOptions();

#pragma pack(push, 1)
	struct State {
		int32_t gameType;
		int32_t isDP;
		int32_t unk1;
		int32_t difficultyP1;
		int32_t difficultyP2;
		int32_t isP1;
		int32_t isP2;
		uint32_t unk2;
		void* musicPtr;
	};
#pragma pack(pop)
	static_assert(sizeof(State) == 0x28);
	const State& GetState();
private:
	GeneralOptions** options;
	State* state;
};