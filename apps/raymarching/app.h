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
  /* Python delegate:
   * MicroPython requires a heap. To avoid dynamic allocation, we keep a working
   * buffer here and we give to controllers that load Python environment. We
   * also memoize the last Python user to avoid re-initiating MicroPython when
   * unneeded. */
  static constexpr int k_pythonHeapSize = 16384;
  char m_pythonHeap[k_pythonHeapSize];
  const void * m_pythonUser;
  RaymarchingViewController m_viewController;

  App(Snapshot * snapshot);
};

}

#endif
