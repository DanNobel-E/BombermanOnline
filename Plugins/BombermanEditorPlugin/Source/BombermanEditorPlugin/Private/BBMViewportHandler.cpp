#include "BBMViewportHandler.h"
#include "PreviewScene.h"
#include "TickableEditorObject.h"
#include "AdvancedPreviewScene.h"
#include "Templates/SharedPointerInternals.h"

void SBBMViewportWidget::Construct(const FArguments& InArgs)
{

}


void SBBMViewportWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (ViewportClient.IsValid())
	{
		ViewportClient.Pin()->Tick(InDeltaTime);
	}

	if (SceneViewport.IsValid())
	{
		//SceneViewport.Pin()->Invalidate();
	}
}

void SBBMViewportWidget::SetSceneViewport(TSharedPtr<FSceneViewport> InSceneViewport)
{
	SceneViewport = InSceneViewport;
}

void SBBMViewportWidget::SetViewportClient(TSharedPtr<FBBMViewportClient> InViewportClient)
{
	ViewportClient = InViewportClient;
}


SBBMEditorViewport::SBBMEditorViewport()
{

}
SBBMEditorViewport::~SBBMEditorViewport()
{


}

void SBBMEditorViewport::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (ViewportClient.IsValid())
	{
		//ViewportClient->DeltaTime = InDeltaTime;
		ViewportClient->Tick(InDeltaTime);
		
	}

}



void SBBMEditorViewport::Construct(const FArguments& InArgs)
{
	FPreviewScene::ConstructionValues Vals;
	Vals.bDefaultLighting = true;
	Vals.bEditor = true;
	PreviewScene = MakeShareable(new FPreviewScene(Vals));
	SetCanTick(true);
	//SetClipping(EWidgetClipping::ClipToBounds);*/
	/*ViewportWidget = SNew(SBBMViewportWidget)
		.Clipping(EWidgetClipping::ClipToBounds)
		.Visibility(EVisibility::Visible);*/

	World = PreviewScene->GetWorld();

	MakeEditorViewportClient();

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
	
	FBBMViewportClient* NewClient = new FBBMViewportClient(SharedThis(this), StaticCastSharedPtr<FAdvancedPreviewScene>(PreviewScene).ToSharedRef(), World);
	ViewportClient = MakeShareable(NewClient);
	SetClient(ViewportClient);
	return ViewportClient.ToSharedRef();

}

TSharedPtr<SViewport> SBBMEditorViewport::GetViewportWidget()
{
	return ViewportWidget;
}

void SBBMEditorViewport::SetWorld(UWorld* InWorld)
{
	if (InWorld)
	{
		if (ViewportClient.IsValid())
		{
			ViewportClient->SetWorld(InWorld);

		}
		World = InWorld;
	}
}
FBBMViewportClient::FBBMViewportClient(const TSharedPtr<SBBMEditorViewport>& InBBMEditorViewport,  TSharedRef<FAdvancedPreviewScene> InPreviewScene, UWorld* InWorld)
	: FEditorViewportClient(nullptr, &InPreviewScene.Get(), StaticCastSharedPtr<SEditorViewport>(InBBMEditorViewport))
	, BBMEditorViewportPtr(InBBMEditorViewport)
{
	GridSize = 2048.0f;
	CellSize = 16;

	EditorViewportWidget = BBMEditorViewportPtr;

	SetRealtime(true);
	ShowWidget(true);
	SetViewMode(VMI_Lit);
	SetIsSimulateInEditorViewport(true);
	/** True if we should draw stats over the viewport */
	bShowStats = true;

	
	bUsesDrawHelper= true;

	/** True if this level viewport canbe used to view Simulate in Editor sessions */
	

	bUsingOrbitCamera = true;

	DrawHelper.bDrawPivot = true;
	DrawHelper.bDrawWorldBox = true;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = true;
	DrawHelper.GridColorAxis = FColor(160, 160, 160);
	DrawHelper.GridColorMajor = FColor(144, 144, 144);
	DrawHelper.GridColorMinor = FColor(128, 128, 128);
	DrawHelper.PerspectiveGridSize = 2048;
	DrawHelper.NumCells = DrawHelper.PerspectiveGridSize / (16 * 2);


	AdvancedPreviewScene = MakeShareable(static_cast<FAdvancedPreviewScene*>(PreviewScene));//&InPreviewScene.Get();
	World = InWorld;
	SetWorld(World);

	SetCameraSpeedScalar(1000);


}

FBBMViewportClient::~FBBMViewportClient()
{

}

void FBBMViewportClient::SetWorld(UWorld* InWorld)
{
	if (InWorld)
	{
		AdvancedPreviewScene->GetWorld()->ChangeFeatureLevel(InWorld->FeatureLevel);
		World = InWorld;

	}
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

	//TSharedPtr<IBBMWorldPresetEditor> BBMEditor = BBMEditorPtr.Pin();

}

void FBBMViewportClient::DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas)
{

	FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
}

bool FBBMViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	FVector Translation = FVector(0, 0, 0);
	FRotator Rotator = FRotator::ZeroRotator;


	if (EventArgs.Key == EKeys::W)
	{
		Translation.X = 1;
		MoveViewportCamera(Translation.GetSafeNormal() * GetCameraSpeed(),Rotator);
		return true;
	}


	if (EventArgs.Key == EKeys::S)
	{
		Translation.X = -1;
		MoveViewportCamera(Translation.GetSafeNormal() * GetCameraSpeed(), Rotator);
		return true;
	}

	return false;
}











