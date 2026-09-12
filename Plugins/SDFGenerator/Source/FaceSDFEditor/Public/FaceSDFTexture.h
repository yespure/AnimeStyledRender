#pragma once

#include "CoreMinimal.h"

class FFaceSDFTexture
{
public:

    //Mask保存逻辑
    static bool SaveFaceMaskTexture(
        const TArray<uint8>& Pixels,
        int32 Resolution,
        const FString& AssetName);

    //灰度图保存函数与上方保存函数同理,使用Chatgpt生成实现
    static bool SaveFaceSDFTexture(
        const TArray<uint8>& Pixels,
        int32 Resolution,
        const FString& AssetName);
};