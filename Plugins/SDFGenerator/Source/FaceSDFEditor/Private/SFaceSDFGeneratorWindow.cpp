#include "SFaceSDFGeneratorWindow.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "PropertyCustomizationHelpers.h"

#include "Engine/SkeletalMesh.h"

#include "FaceSDFGenerator.h"
#include "FaceSDFTexture.h"

#include "Framework/Application/SlateApplication.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "SFaceSDFGeneratorWindow"


void SFaceSDFGeneratorWindow::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)

                // 标题
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(10.0f)
                [
                    SNew(STextBlock)
                        .Text(LOCTEXT("WindowTitle", "Face SDF Generator V1"))
                ]

                //Mesh部分
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock)
                                .Text(LOCTEXT("SelectMeshLabel", "Select Mesh:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SObjectPropertyEntryBox)
                                .AllowedClass(USkeletalMesh::StaticClass())
                                // skeletal mesh only
                                .ObjectPath(this, &SFaceSDFGeneratorWindow::GetSelectedMeshAsset)
                                // 获取当前选中的资源
                                .OnObjectChanged(this, &SFaceSDFGeneratorWindow::OnMeshChanged)
                                // 改变时的回调
                        ]
                ]

            //LOD部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            // ✅修复：LOCTEXT
                            SNew(STextBlock).Text(LOCTEXT("LODLabel", "LOD:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetLODValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnLODChanged)
                                .MinValue(0)
                        ]
                ]

            //Section部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("SectionLabel", "Section:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetSectionValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnSectionChanged)
                                .MinValue(0)
                        ]
                ]

            //UVChannel部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("UVChannelLabel", "UV Channel:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetUVChannelValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnUVChannelChanged)
                                .MinValue(0)
                        ]
                ]

            //Resolution部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("ResolutionLabel", "Resolution:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetResolutionValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnResolutionChanged)
                                .MinValue(64)
                        ]
                ]

            //OutputName部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("OutputNameLabel", "Output Name:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SEditableTextBox)
                                .Text(this, &SFaceSDFGeneratorWindow::GetOutputNameText)
                                .OnTextChanged(this, &SFaceSDFGeneratorWindow::OnOutputNameChanged)
                        ]
                ]

            // ===== Shadow Mask 批量导入 =====
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(SButton)
                                .Text(LOCTEXT("SelectShadowMasksBtn", "Select Shadow Mask PNGs"))
                                .OnClicked(
                                    FOnClicked::CreateSP(
                                        this,
                                        &SFaceSDFGeneratorWindow::OnSelectShadowMasksClicked))
                        ]

                    + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(
                                    this,
                                    &SFaceSDFGeneratorWindow::GetShadowMaskStatusText)
                        ]
                ]

            // ===== 批量生成SDF =====
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SButton)
                        .Text(LOCTEXT("GenerateAllSDFBtn", "Generate All SDF"))
                        .OnClicked(
                            FOnClicked::CreateSP(
                                this,
                                &SFaceSDFGeneratorWindow::OnGenerateAllSDFClicked))
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(20.0f)
                .HAlign(HAlign_Center)
                [
                    SNew(SButton)
                        .Text(LOCTEXT("GenerateBtn", "Generate SDF"))
                        .OnClicked(
                            FOnClicked::CreateSP(
                                this,
                                &SFaceSDFGeneratorWindow::OnGenerateClicked))
                ]
        ];
}


FReply SFaceSDFGeneratorWindow::OnGenerateClicked()
{
    UE_LOG(LogTemp, Warning,
        TEXT("Face SDF: Generate Clicked. Mesh: %s, LOD: %d, Section: %d, UV: %d, Res: %d"),
        *GetSelectedMeshAsset(),
        LODIndex,
        SectionIndex,
        UVChannel,
        Resolution);

    TArray<FFaceSDFTriangle> FaceTriangles;

    if (!FFaceSDFGenerator::ExtractFaceTriangles(
        SelectedMesh.Get(),
        LODIndex,
        SectionIndex,
        UVChannel,
        FaceTriangles))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to extract face triangles."));

        return FReply::Handled();
    }

    TArray<uint8> FaceMaskPixels;

    if (!FFaceSDFGenerator::RasterizeFaceMask(
        FaceTriangles,
        Resolution,
        FaceMaskPixels))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to rasterize face mask."));

        return FReply::Handled();
    }

    TArray<uint8> SDFPixels;

    if (!FFaceSDFGenerator::GenerateGrayscaleSDF(
        FaceMaskPixels,
        Resolution,
        SDFPixels))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to generate grayscale SDF."));

        return FReply::Handled();
    }

    const FString SDFAssetName =
        OutputName.IsEmpty()
        ? TEXT("FaceSDF")
        : OutputName;

    if (!FFaceSDFTexture::SaveFaceSDFTexture(
        SDFPixels,
        Resolution,
        SDFAssetName))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to save grayscale SDF texture."));

        return FReply::Handled();
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Face SDF: Generation completed successfully."));

    return FReply::Handled();
}


void SFaceSDFGeneratorWindow::OnMeshChanged(const FAssetData& AssetData)
{
    // 当用户选择了一个 Mesh 时触发
    SelectedMesh = Cast<USkeletalMesh>(AssetData.GetAsset());

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Mesh changed to: %s"),
        SelectedMesh.IsValid()
        ? *SelectedMesh->GetName()
        : TEXT("None"));
}


FString SFaceSDFGeneratorWindow::GetSelectedMeshAsset() const
{
    if (SelectedMesh.IsValid())
    {
        // 返回资源的路径字符串，UI 就会显示它
        return SelectedMesh->GetPathName();
    }

    return FString(); // 空字符串，UI 就会显示 None
}


//各种数值的获取和设置

TOptional<int32> SFaceSDFGeneratorWindow::GetLODValue() const
{
    return LODIndex;
}


void SFaceSDFGeneratorWindow::OnLODChanged(int32 NewValue)
{
    LODIndex = NewValue;
}


TOptional<int32> SFaceSDFGeneratorWindow::GetSectionValue() const
{
    return SectionIndex;
}


void SFaceSDFGeneratorWindow::OnSectionChanged(int32 NewValue)
{
    SectionIndex = NewValue;
}


TOptional<int32> SFaceSDFGeneratorWindow::GetUVChannelValue() const
{
    return UVChannel;
}


void SFaceSDFGeneratorWindow::OnUVChannelChanged(int32 NewValue)
{
    UVChannel = NewValue;
}


TOptional<int32> SFaceSDFGeneratorWindow::GetResolutionValue() const
{
    return Resolution;
}


void SFaceSDFGeneratorWindow::OnResolutionChanged(int32 NewValue)
{
    Resolution = NewValue;
}


// OutputName 文本双向绑定
FText SFaceSDFGeneratorWindow::GetOutputNameText() const
{
    return FText::FromString(OutputName);
}


void SFaceSDFGeneratorWindow::OnOutputNameChanged(const FText& NewText)
{
    OutputName = NewText.ToString();
}


// ===== Shadow Mask 批量导入 =====

FReply SFaceSDFGeneratorWindow::OnSelectShadowMasksClicked()
{
    IDesktopPlatform* DesktopPlatform =
        FDesktopPlatformModule::Get();

    if (!DesktopPlatform)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to get Desktop Platform."));

        return FReply::Handled();
    }

   const void* ParentWindowHandle =
        FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

    TArray<FString> SelectedFiles;

    const bool bOpened =
        DesktopPlatform->OpenFileDialog(
            ParentWindowHandle,
            TEXT("Select Shadow Mask PNGs"),
            FPaths::ProjectDir(),
            TEXT(""),
            TEXT("PNG Files (*.png)|*.png"),
            EFileDialogFlags::Multiple,
            SelectedFiles);

    if (!bOpened || SelectedFiles.Num() == 0)
    {
        return FReply::Handled();
    }

    // 按文件名排序，保证 Shadow_00 ~ Shadow_64 顺序稳定
    SelectedFiles.Sort();

    SelectedShadowMaskFiles = MoveTemp(SelectedFiles);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Face SDF: Selected %d Shadow Mask files."),
        SelectedShadowMaskFiles.Num());

    return FReply::Handled();
}

FText SFaceSDFGeneratorWindow::GetShadowMaskStatusText() const
{
    return FText::Format(
        LOCTEXT(
            "ShadowMaskStatus",
            "{0} Shadow Mask files selected"),
        SelectedShadowMaskFiles.Num());
}


bool SFaceSDFGeneratorWindow::ReadShadowMaskPNG(
    const FString& FilePath,
    TArray<uint8>& OutPixels,
    int32& OutWidth,
    int32& OutHeight)
{
    TArray<uint8> CompressedData;

    if (!FFileHelper::LoadFileToArray(
        CompressedData,
        *FilePath))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to read PNG file: %s"),
            *FilePath);

        return false;
    }

    IImageWrapperModule& ImageWrapperModule =
        FModuleManager::LoadModuleChecked<IImageWrapperModule>(
            TEXT("ImageWrapper"));

    TSharedPtr<IImageWrapper> ImageWrapper =
        ImageWrapperModule.CreateImageWrapper(
            EImageFormat::PNG);

    if (!ImageWrapper.IsValid())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to create PNG image wrapper."));

        return false;
    }

    if (!ImageWrapper->SetCompressed(
        CompressedData.GetData(),
        CompressedData.Num()))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to decompress PNG: %s"),
            *FilePath);

        return false;
    }

    OutWidth = ImageWrapper->GetWidth();
    OutHeight = ImageWrapper->GetHeight();

    TArray64<uint8> RawData;

    if (!ImageWrapper->GetRaw(
        ERGBFormat::Gray,
        8,
        RawData))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to convert PNG to grayscale: %s"),
            *FilePath);

        return false;
    }

    if (RawData.Num() == 0)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: PNG raw data is invalid: %s"),
            *FilePath);

        return false;
    }

    OutPixels.Reset();

    OutPixels.Append(
        RawData.GetData(),
        RawData.Num());

    return true;
}


FReply SFaceSDFGeneratorWindow::OnGenerateAllSDFClicked()
{
    if (SelectedShadowMaskFiles.Num() == 0)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: No Shadow Mask files selected."));

        return FReply::Handled();
    }

    int32 GeneratedCount = 0;

    for (int32 FileIndex = 0;
        FileIndex < SelectedShadowMaskFiles.Num();
        ++FileIndex)
    {
        const FString& FilePath =
            SelectedShadowMaskFiles[FileIndex];

        TArray<uint8> MaskPixels;

        int32 Width = 0;
        int32 Height = 0;

        if (!ReadShadowMaskPNG(
            FilePath,
            MaskPixels,
            Width,
            Height))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Failed to read Shadow Mask: %s"),
                *FilePath);

            continue;
        }

        if (Width != Height)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Shadow Mask is not square: %s"),
                *FilePath);

            continue;
        }

        // 使用第一张图片决定SDF分辨率
        if (FileIndex == 0)
        {
            Resolution = Width;
        }

        if (Width != Resolution ||
            Height != Resolution)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Resolution mismatch: %s"),
                *FilePath);

            continue;
        }

        TArray<uint8> SDFPixels;

        if (!FFaceSDFGenerator::GenerateGrayscaleSDF(
            MaskPixels,
            Resolution,
            SDFPixels))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Failed to generate SDF: %s"),
                *FilePath);

            continue;
        }

        FString BaseName =
            FPaths::GetBaseFilename(FilePath);

        FString SDFAssetName;

        if (BaseName.StartsWith(TEXT("Shadow_")))
        {
            const FString IndexString =
                BaseName.RightChop(7);

            SDFAssetName =
                FString::Printf(
                    TEXT("SDF_%s"),
                    *IndexString);
        }
        else
        {
            SDFAssetName =
                FString::Printf(
                    TEXT("SDF_%s"),
                    *BaseName);
        }

        if (!FFaceSDFTexture::SaveFaceSDFTexture(
            SDFPixels,
            Resolution,
            SDFAssetName))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Failed to save SDF: %s"),
                *FilePath);

            continue;
        }

        ++GeneratedCount;

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Generated %s"),
            *SDFAssetName);
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Face SDF: Batch generation completed. %d / %d files generated."),
        GeneratedCount,
        SelectedShadowMaskFiles.Num());

    return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE