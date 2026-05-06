// Zhipei Liang


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelFromXP(const int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	
	while (bSearching)
	{
		if (LevelUpInfos.Num() <= Level)
		{
			return Level;
		}
		
		if (XP >= LevelUpInfos[Level].LevelUpRequirementXP)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
