
#include <stdio.h>

#include <jpeglib.h>
#include <jerror.h>
#include <png.h>

#include <mlx/ops.h>

#include "image.h"

FILE *load_file(const std::string &path)
{
    FILE *fp = fopen(path.c_str(), "rb");
    if (fp == NULL)
    {
        throw std::runtime_error("Failed to open file");
    }
    return fp;
}

mlx::core::array load_png(const std::string &path)
{
    auto fp = load_file(path);
    unsigned char header[8];
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fclose(fp);
        throw std::runtime_error("File was not a valid png");
    }
    fseek(fp, 0, SEEK_SET);
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        throw std::runtime_error("Failed to create png struct");
    }
    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        throw std::runtime_error("Failed to create png info");
    }
    if (setjmp(png_jmpbuf(png)))
    {
        throw std::runtime_error("Failed to setjmp");
    }
    png_init_io(png, fp);
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    auto color_type = png_get_color_type(png, info);
    auto bit_depth = png_get_bit_depth(png, info);
    if (bit_depth == 16)
    {
        png_set_strip_16(png);
    }
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        printf("palette\n");
        png_set_palette_to_rgb(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        printf("bit_depth\n");
        png_set_expand_gray_1_2_4_to_8(png);
    }
    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        printf("RNS\n");
        png_set_tRNS_to_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
    {
        printf("color_tpye\n");
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        printf("GOT GRAY\n");
        png_set_gray_to_rgb(png);
    }
    png_read_update_info(png, info);
    auto buffer = new unsigned char[sizeof(unsigned char) * height * png_get_rowbytes(png, info)];

    png_read_image(png, (png_bytep *)buffer);
    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);

    return mlx::core::array(buffer, {height, width, 4});
}

mlx::core::array load_jpeg(const std::string &path)
{
    auto fp = load_file(path);
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    auto channels = cinfo.output_components;
    auto row_stride = width * channels;
    auto buffer = new unsigned char[row_stride * height];
    while (cinfo.output_scanline < cinfo.output_height)
    {
        auto row_pointer = buffer + row_stride * cinfo.output_scanline;
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(fp);
    return mlx::core::array(buffer, {height, width, channels});
}