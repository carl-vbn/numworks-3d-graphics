#include "raymarching_view_controller.h"
#include "raymarching.h"
#include <apps/i18n.h>

RaymarchingViewController::RaymarchingView::RaymarchingView()
{
}

RaymarchingViewController::RaymarchingViewController(Responder * parentResponder, ViewController * rootViewController) :
  ViewController(parentResponder),
  m_View()
{
  rootViewController->setParentResponder(this);
  m_View.setSize(KDSize(320,240));
  Ion::Display::pushRectUniform(m_View.bounds(), KDColorBlack);
}

const char * RaymarchingViewController::title() {
  return "Raymarching";
}

bool RaymarchingViewController::handleEvent(Ion::Events::Event event) {
  KDRect rect = KDRect(0, 18, 320, 222);
  if (event == Ion::Events::One) {
    Raymarching::RaymarchingScene scene(0);
    scene.render(rect, false);

    return true;
  } else if (event == Ion::Events::Two) {
    Raymarching::RaymarchingScene scene(0);
    scene.render(rect, true);

    return true;
  } else if (event == Ion::Events::Four) {
    Raymarching::RaymarchingScene scene(1);
    scene.render(rect, false);

    return true;
  } else if (event == Ion::Events::Five) {
    Raymarching::RaymarchingScene scene(1);
    scene.render(rect, true);

    return true;
  }

  return false;
}

View * RaymarchingViewController::view() {
  return &m_View;
}
