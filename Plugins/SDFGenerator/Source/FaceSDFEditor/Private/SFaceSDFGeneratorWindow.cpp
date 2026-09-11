#include "SFaceSDFGeneratorWindow.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "PropertyCustomizationHelpers.h"

#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshLODModel.h"

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
                            // ✅修复：LOCTEXT
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
                            // ✅修复：LOCTEXT
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

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(20.0f)
                .HAlign(HAlign_Center)
                [
                    SNew(SButton)
                        .Text(LOCTEXT("GenerateBtn", "Generate SDF"))
                        .OnClicked(FOnClicked::CreateSP(this, &SFaceSDFGeneratorWindow::OnGenerateClicked))
                ]
        ];
}

FReply SFaceSDFGeneratorWindow::OnGenerateClicked()
{
    // 这里以后写真正的生成逻辑
    UE_LOG(LogTemp, Warning, TEXT("Face SDF: Generate Clicked. Mesh: %s, LOD: %d, Section: %d, UV: %d, Res: %d"),
        SelectedMesh.IsValid() ? *SelectedMesh->GetName() : TEXT("None"),
        LODIndex, SectionIndex, UVChannel, Resolution);

	TArray<FFaceSDFTriangle> FaceTriangles;

    if (!ExtractFaceTriangles(FaceTriangles))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to extract face triangles."));

        return FReply::Handled();
    }

	const int32 DebugTriangleCount = FMath::Min(FaceTriangles.Num(), 5);

    for(int32 Index = 0; Index < DebugTriangleCount; ++Index)
    {
       const FFaceSDFTriangle& Triangle = FaceTriangles[Index];
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Triangle %d: UV0=(%f,%f), UV1=(%f,%f), UV2=(%f,%f)"),
            Index,
            Triangle.UV0.X, Triangle.UV0.Y,
            Triangle.UV1.X, Triangle.UV1.Y,
			Triangle.UV2.X, Triangle.UV2.Y);
	}
    TArray<uint8> FaceMaskPixels;

    if (!RasterizeFaceMask(FaceTriangles, FaceMaskPixels))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to rasterize face mask."));

        return FReply::Handled();
    }
    return FReply::Handled();
}

void SFaceSDFGeneratorWindow::OnMeshChanged(const FAssetData& AssetData)
{
    // 当用户选择了一个 Mesh 时触发
    SelectedMesh = Cast<USkeletalMesh>(AssetData.GetAsset());
    UE_LOG(LogTemp, Warning, TEXT("Mesh changed to: %s"), SelectedMesh.IsValid() ? *SelectedMesh->GetName() : TEXT("None"));
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

//获取模型三角面数量

bool SFaceSDFGeneratorWindow::ExtractFaceTriangles(TArray<FFaceSDFTriangle>& OutTriangles)
{
    USkeletalMesh* Mesh = SelectedMesh.Get();//获取当前选中的模型

    if (!Mesh)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: No Skeletal Mesh selected."));
        return false;
    }

    FSkeletalMeshModel* ImportedModel = Mesh->GetImportedModel();//得到模型信息和数据

    if (!ImportedModel)//Imported保护
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Failed to get imported model."));
        return false;
    }

    if (LODIndex < 0 || LODIndex >= ImportedModel->LODModels.Num())//LOD验证保护
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Invalid LOD index."));
        return false;
    }

    FSkeletalMeshLODModel& LODModel =
        ImportedModel->LODModels[LODIndex];//获取指定LOD的模型数据

    if (!LODModel.Sections.IsValidIndex(SectionIndex))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Invalid Section index."));
        return false;
    }

    const FSkelMeshSection& Section =
        LODModel.Sections[SectionIndex];//获取指定Section的模型数据

    const int32 NumTriangles =
        static_cast<int32>(Section.NumTriangles);//获取三角面数量

    if (NumTriangles <= 0)//三角面数量保护
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Section contains no triangles."));
        return false;
    }

    if (UVChannel < 0 || UVChannel >= static_cast<int32>(LODModel.NumTexCoords))//UV保护
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Invalid UV Channel."));
        return false;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Face SDF: Mesh=%s LOD=%d Section=%d"),
        *Mesh->GetName(),
        LODIndex,
        SectionIndex);

    UE_LOG(LogTemp, Warning,
        TEXT("Face SDF: NumVertices=%d NumTriangles=%d"),
        Section.NumVertices,
        NumTriangles);

    OutTriangles.Reset();
    OutTriangles.Reserve(NumTriangles);//预分配三角形数组大小

    for (int32 TriangleIndex = 0; TriangleIndex < NumTriangles; ++TriangleIndex)//遍历所有三角形并加入数组
    {
		const uint32 IndexOffset = Section.BaseIndex + TriangleIndex * 3;//从整个LOD的IndexBuffer找到当前Section的三角形的索引偏移量
        if (IndexOffset + 2 >= static_cast<uint32>(LODModel.IndexBuffer.Num()))
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Face SDF: IndexBuffer access out of range."));
            return false;
        }

		const uint32 Index0 = LODModel.IndexBuffer[IndexOffset];
		const uint32 Index1 = LODModel.IndexBuffer[IndexOffset + 1];
		const uint32 Index2 = LODModel.IndexBuffer[IndexOffset + 2];//获取三角形的三个顶点索引
        if (Index0 < Section.BaseVertexIndex || 
            Index1 < Section.BaseVertexIndex || 
            Index2 < Section.BaseVertexIndex)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Face SDF: Vertex index out of range."));
            return false;
		}

        const uint32 LocalVertex0 = Index0 - Section.BaseVertexIndex;
        const uint32 LocalVertex1 = Index1 - Section.BaseVertexIndex;
		const uint32 LocalVertex2 = Index2 - Section.BaseVertexIndex;//将全局顶点索引转换为Section内的局部顶点索引
        if (LocalVertex0 >= static_cast<uint32>(Section.SoftVertices.Num()) ||
            LocalVertex1 >= static_cast<uint32>(Section.SoftVertices.Num()) ||
            LocalVertex2 >= static_cast<uint32>(Section.SoftVertices.Num()))//检查
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Face SDF: SoftVertices access out of range."));
            return false;
        }
        const FSoftSkinVertex& Vertex0 = Section.SoftVertices[LocalVertex0];
        const FSoftSkinVertex& Vertex1 = Section.SoftVertices[LocalVertex1];
		const FSoftSkinVertex& Vertex2 = Section.SoftVertices[LocalVertex2];//获取三角形的三个顶点数据
        FFaceSDFTriangle Triangle;
        Triangle.UV0 = FVector2D(
            Vertex0.UVs[UVChannel].X,
            Vertex0.UVs[UVChannel].Y);

        Triangle.UV1 = FVector2D(
            Vertex1.UVs[UVChannel].X,
            Vertex1.UVs[UVChannel].Y);

        Triangle.UV2 = FVector2D(
            Vertex2.UVs[UVChannel].X,
            Vertex2.UVs[UVChannel].Y);//获取三角形的三个顶点的UV坐标
		OutTriangles.Add(Triangle);
    }
    return true;
}

//模型光栅化
bool SFaceSDFGeneratorWindow::RasterizeFaceMask(const TArray<FFaceSDFTriangle>& Triangles, TArray<uint8>& OutPixels)
{
    if (Triangles.Num() == 0)//三角形数量检查
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: No triangles to rasterize."));
        return false;
    }

    if (Resolution <= 0)//分辨率检查
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Face SDF: Invalid resolution."));
        return false;
    }

    const int32 Width = Resolution;
    const int32 Height = Resolution; //给生成的图片赋值

    OutPixels.Init(0, Width * Height);//初始化像素

    for (const FFaceSDFTriangle& Triangle : Triangles)//遍历所有三角形
    {
        const float MinU = FMath::Min3(
            Triangle.UV0.X,
            Triangle.UV1.X,
            Triangle.UV2.X);
        const float MaxU = FMath::Max3(
            Triangle.UV0.X,
            Triangle.UV1.X,
            Triangle.UV2.X);
        const float MinV = FMath::Min3(
            Triangle.UV0.Y,
            Triangle.UV1.Y,
            Triangle.UV2.Y);
        const float MaxV = FMath::Max3(
            Triangle.UV0.Y,
            Triangle.UV1.Y,
            Triangle.UV2.Y);//计算三角形的UV包围盒

        const int32 MinX = FMath::Clamp(
            FMath::FloorToInt(MinU * Width),
            0,
            Width - 1);
        const int32 MaxX = FMath::Clamp(
            FMath::FloorToInt(MaxU * Width),
            0,
            Width - 1);
        const int32 MinY = FMath::Clamp(
            FMath::FloorToInt(MinV * Height),
            0,
            Height - 1);
        const int32 MaxY = FMath::Clamp(
            FMath::FloorToInt(MaxV * Height),
            0,
            Height - 1);//计算包围盒在像素空间的范围
        const FVector2D A = Triangle.UV0;
        const FVector2D B = Triangle.UV1;
        const FVector2D C = Triangle.UV2;//获取三角形的三个顶点UV坐标

        const FVector2D AB = B - A;
        const FVector2D AC = C - A;//计算三角形的边向量

        const float Cross =
            AB.X * AC.Y - AB.Y * AC.X;//计算叉积用于判断三角形的面积和方向

        if (FMath::IsNearlyZero(Cross))//检查三角形是否退化
        {
            continue;
        }

        for (int32 Y = MinY; Y <= MaxY; ++Y)
        {
            for (int32 X = MinX; X <= MaxX; ++X)
            {
                const FVector2D P(
                    (static_cast<float>(X) + 0.5f) / Width,
                    (static_cast<float>(Y) + 0.5f) / Height);//计算像素中心点的UV坐标

                const FVector2D AP = P - A;
                const float CrossAB =
                    AB.X * AP.Y - AB.Y * AP.X;//计算点P与边AB的叉积
                const FVector2D BP = P - B;
                const FVector2D BC = C - B;
                const float CrossBC =
                    BC.X * BP.Y - BC.Y * BP.X;//同理计算点P与边BC的叉积
                const FVector2D CP = P - C;
                const FVector2D CA = A - C;
                const float CrossCA =
                    CA.X * CP.Y - CA.Y * CP.X;//同理计算点P与边CA的叉积
                const bool bSameSign =
                    (CrossAB >= 0.0f &&
                        CrossBC >= 0.0f &&
                        CrossCA >= 0.0f)
                    ||
                    (CrossAB <= 0.0f &&
                        CrossBC <= 0.0f &&
                        CrossCA <= 0.0f);//检查点P是否在三角形内，若三个叉积符号相同则在三角形内
                if (bSameSign)
                {
                    const int32 PixelIndex =
                        Y * Width + X;

                    OutPixels[PixelIndex] = 255;//将像素设置为白色，表示在三角形内
                }



            }
        }
    
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Face SDF: Rasterization finished. Resolution=%d, Triangles=%d"),
        Resolution,
        Triangles.Num());

    return true;
}


#undef LOCTEXT_NAMESPACE