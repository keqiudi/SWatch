SET(SOURCES screens/ui_SpO2MeasurePage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_ALiDaKai20.c
    fonts/ui_font_CuYuan60.c
    fonts/ui_font_O2MPiconfont50.c)

add_library(ui ${SOURCES})
