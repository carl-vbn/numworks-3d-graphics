#ifndef KANDINSKY_ION_CONTEXT_H
#define KANDINSKY_ION_CONTEXT_H

#include <kandinsky/context.h>

class KDIonContext : public KDContext {
public:
  static KDIonContext * sharedContext();
private:
  KDIonContext();
  void pushRect(KDRect rect, const KDColor * pixels) override;
  void pushRectUniform(KDRect rect, KDColor color) override;
  void pullRect(KDRect rect, KDColor * pixels) override;
};

#endif
