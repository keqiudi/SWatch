SET(SOURCES screens/ui_AboutPage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_ALiDaKai20.c)

add_library(ui ${SOURCES})
