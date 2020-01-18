#ifndef RAYMARCHING_VIEW_CONTROLLER_H
#define RAYMARCHING_VIEW_CONTROLLER_H

#include <escher/view_controller.h>
#include <escher/stack_view.h>
#include <escher/palette.h>
#include <ion/display.h>

class RaymarchingViewController : public ViewController {
public:
  RaymarchingViewController(Responder * parentResponder, ViewController * rootViewController);

  /* Push creates a new StackView and adds it */
  void push(ViewController * vc, KDColor textColor = Palette::SubTab, KDColor backgroundColor = KDColorWhite, KDColor separatorColor = Palette::GreyBright);
  void pop();

  int depth();
  View * view() override;
  const char * title() override;
  bool handleEvent(Ion::Events::Event event) override;

private:
  class Frame {
  public:
    Frame(ViewController * viewController = nullptr, KDColor textColor = Palette::SubTab, KDColor backgroundColor = KDColorWhite, KDColor separatorColor = Palette::GreyBright) :
      m_viewController(viewController),
      m_textColor(textColor),
      m_backgroundColor(backgroundColor),
      m_separatorColor(separatorColor) {}
    ViewController * viewController() { return m_viewController; }
    KDColor textColor() { return m_textColor; }
    KDColor backgroundColor() { return m_backgroundColor; }
    KDColor separatorColor() { return m_separatorColor; }
  private:
    ViewController * m_viewController;
    KDColor m_textColor;
    KDColor m_backgroundColor;
    KDColor m_separatorColor;
  };
  class RaymarchingView : public View {
    public:
      RaymarchingView();
  };
  RaymarchingView m_View;
};

#endif
