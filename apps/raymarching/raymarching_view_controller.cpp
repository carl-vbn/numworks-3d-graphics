#include "raymarching_view_controller.h"
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

    return true;
  } else if (event == Ion::Events::Two) {
    delete m_scene;
    m_scene = new Raymarching::RaymarchingScene(1);
    m_scene->render(rect, true);

    return true;

  } else if (event == Ion::Events::EXE || event == Ion::Events::OK) {
    m_scene->render(rect, false);
  } else if(event == Ion::Events::Right) {
    m_scene->translateCamera(float3(10,0,0));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Left) {
    m_scene->translateCamera(float3(-10,0,0));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Up) {
    m_scene->translateCamera(float3(0,10,0));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Down) {
    m_scene->translateCamera(float3(0,-10,0));
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Exp) {
    m_scene->rotateCamera(-10);
    m_scene->render(rect, true);
  } else if(event == Ion::Events::Log) {
    m_scene->rotateCamera(10);
    m_scene->render(rect, true);
  }

  return false;
}

View * RaymarchingViewController::view() {
  return &m_View;
}
