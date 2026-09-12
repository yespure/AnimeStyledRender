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

#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "ObjectTools.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

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
            //生成Atlas
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

FReply SFaceSDFGeneratorWindow::OnGenerateAtlasClicked()
{
    if (SelectedShadowMaskFiles.Num() != 65)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Please select exactly 65 files for the 9x9 Atlas."));

        return FReply::Handled();
    }

    TArray<FString> SDFFiles;

    /*
     * 这里暂时直接使用已经选择的文件。
     *
     * 如果你现在SelectedShadowMaskFiles保存的是
     * Shadow Mask PNG，那么这里需要先生成SDF，
     * 再把生成的SDF像素直接放入Atlas。
     *
     * 第一版为了跑通流程，我们先把当前选中的
     * 65张SDF PNG作为输入。
     */

    SDFFiles = SelectedShadowMaskFiles;

    TArray<uint8> AtlasPixels;
    int32 AtlasResolution = 0;

    if (!GenerateSDFAtlas(
        SDFFiles,
        AtlasPixels,
        AtlasResolution))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to generate SDF Atlas."));

        return FReply::Handled();
    }

    if (!SaveSDFAtlasTexture(
        AtlasPixels,
        AtlasResolution,
        TEXT("FaceSDF_Atlas")))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to save SDF Atlas."));

        return FReply::Handled();
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Face SDF: Atlas generation completed successfully."));

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

//生成PNG读取
bool SFaceSDFGeneratorWindow::LoadPNGAsGrayscale(
    const FString& FilePath,
    TArray<uint8>& OutPixels,
    int32& OutWidth,
    int32& OutHeight)
{
    TArray<uint8> CompressedData;

    if(!FFileHelper::LoadFileToArray(CompressedData, * FilePath))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to load PNG: %s"),
            *FilePath);

        return false;
    }

    IImageWrapperModule& ImageWrapperModule =
        FModuleManager::LoadModuleChecked<IImageWrapperModule>(
            TEXT("ImagerWrapper")
        );
    TSharedPtr<IImageWrapper> ImageWrapper =
        ImageWrapperModule.CreateImageWrapper(
            EImageFormat::PNG);
    if (!ImageWrapper.IsValid())
    {
        return false;
    }

    if (!ImageWrapper->SetCompressed(
        CompressedData.GetData(),
        CompressedData.Num()))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to decode PNG: %s"),
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

bool SFaceSDFGeneratorWindow::GenerateSDFAtlas(
    const TArray<FString>& SDFFiles,
    TArray<uint8>& OutAtlasPixels,
    int32& OutAtlasResolution)
{
    if (SDFFiles.Num() != 65)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: 9x9 Atlas requires 65 SDF files. Current=%d"),
            SDFFiles.Num());

        return false;
    }

    TArray<uint8> FirstPixels;//用于确定单张SDF分辨率
    int32 SDFWidth = 0;
    int32 SDFHeight = 0;

    if (!LoadPNGAsGrayscale(
        SDFFiles[0],
        FirstPixels,
        SDFWidth,
        SDFHeight))
    {
        return false;
    }

    if (SDFWidth != SDFHeight)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: SDF texture must be square."));

        return false;
    }//保护

    const int32 AtlasGridSize = 9;//后续可更改,现在必须是65张

    OutAtlasResolution =
        SDFWidth * AtlasGridSize;//计算Grid大小
    const int32 AtlasPixelCount =
        OutAtlasResolution * OutAtlasResolution;

    OutAtlasPixels.Init(
        0,
        AtlasPixelCount);//设定Grid大小
    //遍历每一张SDF
    for (const FString& FilePath : SDFFiles)
    {
        TArray<uint8> SDFPixels;
        int32 Width = 0;
        int32 Height = 0;

        if (!LoadPNGAsGrayscale(
            FilePath,
            SDFPixels,
            Width,
            Height))
        {
            return false;
        }

        if (Width != SDFWidth ||
            Height != SDFHeight)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: SDF resolution mismatch: %s"),
                *FilePath);

            return false;
        }//保护+Debug

        FString FileName = FPaths::GetBaseFilename(FilePath);
        int32 Row = -1;
        int32 Column = -1;

        TArray<FString> Parts;
        FileName.ParseIntoArray(
            Parts,
            TEXT("_"),
            true);//FileName严格要求

        if (Parts.Num() < 2)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Invalid SDF filename: %s"),
                *FileName);

            return false;
        }

        Row = FCString::Atoi(
            *Parts[Parts.Num() - 2]);
        Column = FCString::Atoi(
            *Parts[Parts.Num() - 1]);

        Row -= 1;
        Column -= 1;

        // 检查Atlas坐标
        if (Row < 0 || Row >= 9 ||
            Column < 0 || Column >= 9)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Invalid Atlas coordinate: %s"),
                *FileName);

            return false;
        }

        // 顶部和底部极点只能存在于第一列
        if ((Row == 0 || Row == 8) &&
            Column != 0)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Face SDF: Invalid pole position: %s"),
                *FileName);

            return false;
        }

        // 将SDF复制到Atlas
        for (int32 Y = 0; Y < SDFHeight; ++Y)
        {
            for (int32 X = 0; X < SDFWidth; ++X)
            {
                const int32 AtlasX =
                    Column * SDFWidth + X;

                const int32 AtlasY =
                    Row * SDFHeight + Y;

                const int32 AtlasIndex =
                    AtlasY * OutAtlasResolution +
                    AtlasX;

                const int32 SDFIndex =
                    Y * SDFWidth + X;

                OutAtlasPixels[AtlasIndex] =
                    SDFPixels[SDFIndex];
            }
        }
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Face SDF: 9x9 SDF Atlas generated. Resolution=%d"),
        OutAtlasResolution);

    return true;
    
}

//保存Atlas
bool SFaceSDFGeneratorWindow::SaveSDFAtlasTexture(
    const TArray<uint8>& Pixels,
    int32 AtlasResolution,
    const FString& AssetName)
{
    if (Pixels.Num() !=
        AtlasResolution * AtlasResolution)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Invalid Atlas pixel count."));

        return false;
    }
    //确定路径以及名称
    FString SafeAssetName =
        ObjectTools::SanitizeObjectName(
            AssetName);

    const FString PackagePath =
        TEXT("/Game/FaceSDF/") +
        SafeAssetName;
    //创建资源包
    UPackage* Package = CreatePackage(*PackagePath);
    if (!Package)
    {
        return false;
    }

    UTexture2D* Texture = NewObject<UTexture2D>(
        Package,
        *SafeAssetName,
        RF_Public | RF_Standalone);
    if (!Texture)
    {
        return false;
    }

    // 初始化纹理源数据
    Texture->Source.Init(
        AtlasResolution,
        AtlasResolution,
        1,
        1,
        TSF_G8);

    uint8* MipData =
        Texture->Source.LockMip(0);

    FMemory::Memcpy(
        MipData,
        Pixels.GetData(),
        Pixels.Num());

    // 解锁纹理数据
    Texture->Source.UnlockMip(0);

    // 纹理设置
    Texture->SRGB = false;
    Texture->CompressionSettings =
        TC_Grayscale;
    Texture->MipGenSettings =
        TMGS_NoMipmaps;
    Texture->Filter =
        TF_Bilinear;

    Texture->UpdateResource();

    FAssetRegistryModule::AssetCreated(
        Texture);

    Package->MarkPackageDirty();

    const FString PackageFileName =
        FPackageName::LongPackageNameToFilename(
            PackagePath,
            FPackageName::GetAssetPackageExtension());

    FSavePackageArgs SaveArgs;

    SaveArgs.TopLevelFlags =
        RF_Public | RF_Standalone;

    if (!UPackage::SavePackage(
        Package,
        Texture,
        *PackageFileName,
        SaveArgs))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Face SDF: Failed to save SDF Atlas."));

        return false;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Face SDF: SDF Atlas saved: %s"),
        *PackagePath);

    return true;

}

#undef LOCTEXT_NAMESPACE