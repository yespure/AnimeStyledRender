#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UObject/WeakObjectPtr.h"

class USkeletalMesh;
struct FAssetData;

struct FFaceSDFTriangle
{
	FVector2D UV0;
	FVector2D UV1;
	FVector2D UV2;
};
class SFaceSDFGeneratorWindow : public SCompoundWidget
{
public:

    SLATE_BEGIN_ARGS(SFaceSDFGeneratorWindow) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:

    // 按钮点击
    FReply OnGenerateClicked();

    // 资源选择
    void OnMeshChanged(const FAssetData& AssetData);
    FString GetSelectedMeshAsset() const;

    // ===== LOD =====
    TOptional<int32> GetLODValue() const;
    void OnLODChanged(int32 NewValue);

    // ===== Section =====
    TOptional<int32> GetSectionValue() const;
    void OnSectionChanged(int32 NewValue);

    // ===== UVChannel =====
    TOptional<int32> GetUVChannelValue() const;
    void OnUVChannelChanged(int32 NewValue);

    // ===== Resolution =====
    TOptional<int32> GetResolutionValue() const;
    void OnResolutionChanged(int32 NewValue);

    // ===== OutputName =====
    FText GetOutputNameText() const;
    void OnOutputNameChanged(const FText& NewText);

    int32 LODIndex = 0;
    int32 SectionIndex = 0;
    int32 UVChannel = 0;
    int32 Resolution = 128;
    FString OutputName = TEXT("FaceSDF");
    TWeakObjectPtr<USkeletalMesh> SelectedMesh;
    bool ExtractFaceTriangles(TArray<FFaceSDFTriangle>& OutTriangles);//三角形三坐标数组

    bool RasterizeFaceMask(
        const TArray<FFaceSDFTriangle>& Triangles,
		TArray<uint8>& OutPixels);//光栅化三角形到像素数组
};