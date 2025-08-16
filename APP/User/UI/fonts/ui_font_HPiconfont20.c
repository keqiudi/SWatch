/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/keqiu/myProjects/SquareLineProjects/HomePage/assets/iconfont.ttf -o C:/Users/keqiu/myProjects/SquareLineProjects/HomePage/assets\ui_font_HPiconfont20.c --format lvgl -r 0xe669 -r 0xe602 -r 0xe6eb -r 0xe7a8 --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_HPICONFONT20
#define UI_FONT_HPICONFONT20 1
#endif

#if UI_FONT_HPICONFONT20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+E602 "" */
    0x0, 0x40, 0x4, 0x0, 0xe0, 0xe, 0x9, 0x70,
    0xdb, 0x1c, 0xe3, 0xe0, 0x7f, 0x7, 0xf0, 0xff,
    0x8d, 0xf8, 0xdf, 0x84, 0xf8, 0x67, 0x1, 0xe0,

    /* U+E669 "" */
    0x1, 0x0, 0x80, 0x20, 0x18, 0xe, 0x7, 0x83,
    0xc1, 0xff, 0xff, 0x83, 0xc1, 0xe0, 0x70, 0x18,
    0x4, 0x1, 0x0, 0x80,

    /* U+E6EB "" */
    0x1, 0xc0, 0x7, 0x0, 0x1c, 0x0, 0x0, 0xf,
    0x80, 0x7f, 0x23, 0xff, 0xcd, 0xdc, 0x27, 0x0,
    0x3f, 0x0, 0xfc, 0x3b, 0x38, 0x7c, 0x60, 0x61,
    0x80, 0x6, 0x0, 0x0,

    /* U+E7A8 "" */
    0x1, 0x80, 0x4b, 0xe9, 0x7d, 0x20, 0x25, 0xf4,
    0xbc, 0x90, 0x12, 0xfb, 0xc0, 0x7b, 0xef, 0x7d,
    0xe0, 0x7e, 0xf, 0xc1, 0xf8, 0x1e
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 256, .box_w = 12, .box_h = 16, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 24, .adv_w = 256, .box_w = 10, .box_h = 16, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 44, .adv_w = 256, .box_w = 14, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 72, .adv_w = 256, .box_w = 11, .box_h = 16, .ofs_x = 3, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x67, 0xe9, 0x1a6
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 58882, .range_length = 423, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_HPiconfont20 = {
#else
lv_font_t ui_font_HPiconfont20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_HPICONFONT20*/

