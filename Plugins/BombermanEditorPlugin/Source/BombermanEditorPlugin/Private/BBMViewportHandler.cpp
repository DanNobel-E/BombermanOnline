#include "BBMViewportHandler.h"
#include "PreviewScene.h"
#include "TickableEditorObject.h"
#include "AdvancedPreviewScene.h"
#include "Templates/SharedPointerInternals.h"


SBBMEditorViewport::SBBMEditorViewport() : PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{
	
}
SBBMEditorViewport::~SBBMEditorViewport()
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->Viewport = nullptr;
		ViewportClient.Reset();
	}



}

void SBBMEditorViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (ViewportClient.IsValid())
	{

		ViewportClient->Tick(InDeltaTime);
	}

}



void SBBMEditorViewport::Construct(const FArguments& InArgs)
{
	SEditorViewport::Construct(SEditorViewport::FArguments());
	

}




void SBBMEditorViewport::SetViewportClient(TSharedPtr<FBBMViewportClient> InViewportClient)
{
	SetClient(InViewportClient);
	ViewportClient = InViewportClient;

}

void SBBMEditorViewport::SetSceneViewport(TSharedPtr<FSceneViewport> InSceneViewport)
{
	
	if (InSceneViewport.IsValid())
	{
		SceneViewport = InSceneViewport;

	}

}

void SBBMEditorViewport::SetClient(TSharedPtr<FBBMViewportClient> InViewportClient)
{
	if (InViewportClient.IsValid())
	{
		Client = InViewportClient;

	}
}

TSharedRef<FEditorViewportClient> SBBMEditorViewport::MakeEditorViewportClient()
{
	
	FBBMViewportClient* NewClient = new FBBMViewportClient(SharedThis(this), PreviewScene.ToSharedRef());
	ViewportClient = MakeShareable(NewClient);
	return ViewportClient.ToSharedRef();

}

void SBBMEditorViewport::BindCommands()
{
	SEditorViewport::BindCommands();
}

void SBBMEditorViewport::OnFocusViewportToSelection()
{

}
void SBBMEditorViewport::OnFloatingButtonClicked()
{

}


TSharedRef<SEditorViewport> SBBMEditorViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SBBMEditorViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result = MakeShareable(new FExtender);
	return Result;
}



FBBMViewportClient::FBBMViewportClient(const TSharedRef<SBBMEditorViewport>& InBBMEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene)
	: FEditorViewportClient(nullptr, &InPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(InBBMEditorViewport))
	, BBMEditorViewportPtr(InBBMEditorViewport)
{
	AdvancedPreviewScene = MakeShareable(static_cast<FAdvancedPreviewScene*>(PreviewScene));
	EditorViewportWidget = BBMEditorViewportPtr;
	
	bUsesDrawHelper= true;
	SetRealtime(true);
	
	DrawHelper.bDrawPivot = true;
	DrawHelper.bDrawGrid = true;
	DrawHelper.AxesLineThickness = 5.f;
	DrawHelper.PivotSize = 5.f;
	DrawHelper.bDrawWorldBox = true;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.GridColorAxis = FColor(160, 160, 160);
	DrawHelper.GridColorMajor = FColor(144, 144, 144);
	DrawHelper.GridColorMinor = FColor(128, 128, 128);
	DrawHelper.PerspectiveGridSize = 2048;
	DrawHelper.NumCells = DrawHelper.PerspectiveGridSize / (16 * 2);

	AdvancedPreviewScene->SetFloorVisibility(false);

	SetViewLocation(FVector(75, 75, 75));
	SetViewRotation(FVector(-75, -75, -75).Rotation());
	EngineShowFlags.SetScreenPercentage(true);

	ELevelViewportType Type = LVT_Perspective;
	SetViewportType(Type);
	SetViewModes(VMI_Lit, VMI_Lit);

	EngineShowFlags.SetPostProcessMaterial(true);
	EngineShowFlags.SetPostProcessing(true);

	
	ShowWidget(true);
	SetIsSimulateInEditorViewport(true);

	GridSize = 2048.f;
	CellSize = 16;

	bShowStats = true;
	bUsingOrbitCamera = true;
	//SetCameraSpeedScalar(1000);




}

FBBMViewportClient::~FBBMViewportClient()
{
	BBMEditorViewportPtr.Reset();
	Viewport = nullptr;
	AdvancedPreviewScene.Reset();
	FEditorViewportClient::~FEditorViewportClient();
}




void FBBMViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread)
	{
		if (AdvancedPreviewScene)
		{
			AdvancedPreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
		}
	}
}

void FBBMViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);


}

void FBBMViewportClient::DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas)
{

	FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
}



bool FBBMViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	FVector Translation = FVector(0, 0, 0);
	FRotator Rotator = FRotator::ZeroRotator;

	if (EventArgs.Event == EInputEvent::IE_Pressed)
	{


		if (EventArgs.Key == EKeys::W)
		{
			Translation.Z = 1;
			MoveViewportCamera(FVector::ZeroVector, Translation.Rotation());
			return true;
		}


		if (EventArgs.Key == EKeys::S)
		{
			Translation.Z = -1;
			MoveViewportCamera(FVector::ZeroVector, Translation.Rotation());
			return true;
		}


	}

	
	return false;
}











