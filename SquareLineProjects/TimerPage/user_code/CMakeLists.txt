SET(SOURCES screens/ui_TimerPage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_TMPiconfont30.c
    fonts/ui_font_CuYuan40.c)

add_library(ui ${SOURCES})
