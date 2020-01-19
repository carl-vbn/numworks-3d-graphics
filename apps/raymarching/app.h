#ifndef RAYMARCHING_APP_H
#define RAYMARCHING_APP_H

#include "../shared/text_field_delegate_app.h"
#include <escher.h>
#include "raymarching_view_controller.h"

namespace Raymarching {

class App : public Shared::InputEventHandlerDelegateApp {
public:
  class Descriptor : public ::App::Descriptor {
  public:
    I18n::Message name() override;
    I18n::Message upperName() override;
    const Image * icon() override;
  };
  class Snapshot : public ::App::Snapshot {
  public:
    App * unpack(Container * container) override;
    void reset() override;
    Descriptor * descriptor() override;
  private:
    void tidy() override;
  };
  static App * app() {
    return static_cast<App *>(Container::activeApp());
  }

private:
  char m_raymarchingHeap[0];
  RaymarchingViewController m_viewController;

  App(Snapshot * snapshot);
};

}

#endif
