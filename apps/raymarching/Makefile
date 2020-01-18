apps += Raymarching::App
app_headers += apps/raymarching/app.h

app_raymarching_src = $(addprefix apps/raymarching/,\
  app.cpp \
  raymarching_view_controller.cpp \
  raymarching.cpp \
  math_utils.cpp \
)

app_raymarching_src += $(app_raymarching_test_src)
app_src += $(app_raymarching_src)

i18n_files += $(addprefix apps/raymarching/,\
  base.de.i18n\
  base.en.i18n\
  base.es.i18n\
  base.fr.i18n\
  base.pt.i18n\
)

$(eval $(call depends_on_image,apps/raymarching/app.cpp,apps/raymarching/raymarching_icon.png))
