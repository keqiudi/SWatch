SET(SOURCES screens/ui_LTSetPage.c
    screens/ui_SOffTimeSetPage.c
    screens/ui_DateTimeMenuPage.c
    screens/ui_DateSetPage.c
    screens/ui_TimeSetPage.c
    screens/ui_AllSettingPages.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_CuYuan40.c
    fonts/ui_font_LTSiconfont40.c
    fonts/ui_font_ALiDaKai20.c
    fonts/ui_font_CuYuan20.c
    fonts/ui_font_CuYuan30.c)

add_library(ui ${SOURCES})
