SET(SOURCES screens/ui_MenuPage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_ALiDaKai20.c
    fonts/ui_font_FangZhengCuYuan20.c
    fonts/ui_font_MPiconfont25.c
    fonts/ui_font_MPiconfont30.c
    fonts/ui_font_SiYuanBold20.c)

add_library(ui ${SOURCES})
