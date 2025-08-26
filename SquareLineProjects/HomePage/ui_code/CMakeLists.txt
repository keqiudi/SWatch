SET(SOURCES screens/ui_HomePage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_HPiconfont25.c
    fonts/ui_font_OpenSasnsItalic100.c
    fonts/ui_font_OpenSasnsItalic15.c
    fonts/ui_font_CuYuan20.c)

add_library(ui ${SOURCES})
