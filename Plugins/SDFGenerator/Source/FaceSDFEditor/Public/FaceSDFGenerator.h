#pragma once

#include "CoreMinimal.h"
#include "FaceSDFTypes.h"

class USkeletalMesh;

class FFaceSDFGenerator
{
public:

    //获取模型三角面数量
    static bool ExtractFaceTriangles(
        USkeletalMesh* Mesh,
        int32 LODIndex,
        int32 SectionIndex,
        int32 UVChannel,
        TArray<FFaceSDFTriangle>& OutTriangles);

    //模型光栅化
    static bool RasterizeFaceMask(
        const TArray<FFaceSDFTriangle>& Triangles,
        int32 Resolution,
        TArray<uint8>& OutPixels);

    //灰度SDF
    static bool GenerateGrayscaleSDF(
        const TArray<uint8>& MaskPixels,
        int32 Resolution,
        TArray<uint8>& OutPixels);
};