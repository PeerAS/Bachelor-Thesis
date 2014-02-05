#include "pch.h"
#include "Lunch_v1.0.h"
#include "BasicTimer.h"

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Phone::UI::Input;
using namespace concurrency;

Lunch_v1_0::Lunch_v1_0() :
	m_windowClosed(false),
	m_windowVisible(true)
{
}

void Lunch_v1_0::Initialize(CoreApplicationView^ applicationView)
{
	///event handlers for the phone
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &Lunch_v1_0::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &Lunch_v1_0::OnSuspending);	///event handler for suspending the game

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &Lunch_v1_0::OnResuming);	///event handler for activating the game

	HardwareButtons::BackPressed +=
		ref new EventHandler<BackPressedEventArgs^>(this, &Lunch_v1_0::onBackPressed);
	m_renderer = ref new GameRenderer();	///create the renderer for the came
	
}

void Lunch_v1_0::SetWindow(CoreWindow^ window)
{

	DisplayProperties::AutoRotationPreferences = Windows::Graphics::Display::DisplayOrientations::Landscape |
		Windows::Graphics::Display::DisplayOrientations::LandscapeFlipped;	///set the game to only display in Landscape or Landscape Flipped


	DisplayProperties::OrientationChanged +=
		ref new DisplayPropertiesEventHandler(this, &Lunch_v1_0::onOrientationChanged);	///event handler for when the phone is rotated

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &Lunch_v1_0::OnVisibilityChanged);
	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Lunch_v1_0::OnWindowClosed);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Lunch_v1_0::OnPointerPressed);	///event handler for pressing the screen

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Lunch_v1_0::OnPointerMoved);	///event handler for moving your finger on the screen

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Lunch_v1_0::OnPointerReleased);	///event handler for releasing you finger

	m_renderer->Initialize(CoreWindow::GetForCurrentThread());
}

void Lunch_v1_0::onOrientationChanged(Platform::Object^ sender)
{
	m_renderer->UpdateForWindowSizeChange();
}

void Lunch_v1_0::Load(Platform::String^ entryPoint)
{
}

void Lunch_v1_0::Run()
{
	BasicTimer^ timer = ref new BasicTimer();

	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			timer->Update();
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
			m_renderer->Update(timer->Total, timer->Delta);
			m_renderer->Render();
			m_renderer->Present(); // This call is synchronized to the display frame rate.
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void Lunch_v1_0::Uninitialize()
{

}

void Lunch_v1_0::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;	///sets the visibility of the windows
}

void Lunch_v1_0::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void Lunch_v1_0::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	Point currentPoint = m_renderer->TransformToOrientation(args->CurrentPoint->Position);
	m_renderer->addInput(currentPoint);	//adds input to the renderer
}

void Lunch_v1_0::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	Point currentPointMoved = m_renderer->TransformToOrientation(args->CurrentPoint->Position);
}

void Lunch_v1_0::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{	
	Point currentPoint = m_renderer->TransformToOrientation((args->CurrentPoint->Position));
	m_renderer->addReleaseInput(currentPoint);	//adds input the renderer
}

void Lunch_v1_0::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void Lunch_v1_0::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
	m_renderer->ReleaseResourcesForSuspending();

	create_task([this, deferral]()
	{

		deferral->Complete();
	});
}
 
void Lunch_v1_0::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.
	 m_renderer->CreateWindowSizeDependentResources();
}

void Lunch_v1_0::onBackPressed(Platform::Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	m_renderer->onBackPressed(sender, args);
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new Lunch_v1_0();
}

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new Direct3DApplicationSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}