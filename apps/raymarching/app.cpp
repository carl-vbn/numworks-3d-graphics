#include "app.h"
#include "raymarching_icon.h"
#include <apps/i18n.h>

using namespace Shared;

namespace Raymarching {

I18n::Message App::Descriptor::name() {
  return I18n::Message::RaymarchApp;
}

I18n::Message App::Descriptor::upperName() {
  return I18n::Message::RaymarchAppCapital;
}

const Image * App::Descriptor::icon() {
  return ImageStore::RaymarchingIcon;
}

App * App::Snapshot::unpack(Container * container) {
  return new (container->currentAppBuffer()) App(this);
}

void App::Snapshot::reset() {
  
}

App::Descriptor * App::Snapshot::descriptor() {
  static Descriptor descriptor;
  return &descriptor;
}

void App::Snapshot::tidy() {
  
}

App::App(Snapshot * snapshot) :
  Shared::InputEventHandlerDelegateApp(snapshot, &m_viewController),
  m_pythonHeap{},
  m_viewController(nullptr, &m_modalViewController)
{
}

}
