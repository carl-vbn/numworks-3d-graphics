#include "raymarching_view_controller.h"
#include <apps/i18n.h>
#include "math_utils.h"

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
  m_scene = new Raymarching::RaymarchingScene(0);
  m_scene->render(KDRect(0, 18, 320, 222), true);
}

RaymarchingViewController::~RaymarchingViewController() {
  delete m_scene;
}

const char * RaymarchingViewController::title() {
  return "Raymarching";
}

bool RaymarchingViewController::handleEvent(Ion::Events::Event event) {
  KDRect rect = KDRect(0, 18, 320, 222);
  if (event == Ion::Events::One) {
    delete m_scene;
    m_scene = new Raymarching::RaymarchingScene(0);
    m_scene->render(rect, true);
  } else if (event == Ion::Events::Two) {
    delete m_scene;
    m_scene = new Raymarching::RaymarchingScene(1);
    m_scene->render(rect, true);
  } else if (event == Ion::Events::Three) {
    delete m_scene;
    m_scene = new Raymarching::RaymarchingScene(2);
    m_scene->render(rect, true);

  } else if (event == Ion::Events::EXE || event == Ion::Events::OK) {
    m_scene->render(rect, false);
  } else if(event == Ion::Events::Right) {
    m_scene->translateCamera(multiply(m_scene->getCameraVector(90),10));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Left) {
    m_scene->translateCamera(multiply(m_scene->getCameraVector(-90),10));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Up) {
    m_scene->translateCamera(multiply(m_scene->getCameraVector(),10));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Down) {
    m_scene->translateCamera(multiply(m_scene->getCameraVector(),-10));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Exp) {
    m_scene->rotateCamera(-5);
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Log) {
    m_scene->rotateCamera(5);
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Toolbox) {
    m_scene->rotateCamera(0, -5);
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Sqrt) {
    m_scene->rotateCamera(0, 5);
    m_scene->render(rect, true);
  } else {
    return false;
  }

  return true;
}

View * RaymarchingViewController::view() {
  return &m_View;
}
