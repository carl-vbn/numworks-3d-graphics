#ifndef RAYMARCHING_VIEW_CONTROLLER_H
#define RAYMARCHING_VIEW_CONTROLLER_H

#include <escher/view_controller.h>
#include <escher/stack_view.h>
#include <escher/palette.h>
#include <ion/display.h>
#include "raymarching.h"

class RaymarchingViewController : public ViewController {
public:
  RaymarchingViewController(Responder * parentResponder, ViewController * rootViewController);
  ~RaymarchingViewController();

  View * view() override;
  const char * title() override;
  bool handleEvent(Ion::Events::Event event) override;

  Raymarching::RaymarchingScene * m_scene;

private:
  class Frame {
  public:
  private:
    ViewController * m_viewController;
  };
  class RaymarchingView : public View {
    public:
      RaymarchingView();
  };
  RaymarchingView m_View;
};

#endif
