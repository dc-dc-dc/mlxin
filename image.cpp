
#include <stdio.h>

#include <jpeglib.h>
#include <jerror.h>
#include <png.h>

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

array load_png(const std::string &path)
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
        png_set_palette_to_rgb(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png);
    }
    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        png_set_swap_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png);
    }
    png_read_update_info(png, info);

    unsigned char temp[height * width * 4];
    auto buffer = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for(int i = 0; i < height; i++)
    {
        buffer[i] = (png_bytep)(temp + i * width * 4);
    }
    png_read_image(png, buffer);
    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
    free(buffer);
    return array(temp, {height, width, 4});
}

void save_png(const std::string &path, array& img)
{
    img.eval();
    if (img.dtype() != uint8)
    {
        throw std::runtime_error("Image must be uint8");
    }
    if (img.ndim() != 3)
    {
        throw std::runtime_error("Image must be 3d");
    }

    FILE *fp = fopen(path.c_str(), "wb");
    if (!fp)
    {
        throw std::runtime_error("Failed to open file");
    }
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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
    png_set_IHDR(png, info, img.shape(1), img.shape(0), 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    auto data = img.data<unsigned char>();
    auto buffer = (png_bytep *)malloc(sizeof(png_bytep) * img.shape(0));
    for(int i = 0; i < img.shape(0); i++)
    {
        buffer[i] = (png_bytep)(data + i * img.shape(1) * 4);
    }
    png_write_image(png, buffer);
    png_write_end(png, NULL);
    fclose(fp);
    free(buffer);
    png_destroy_write_struct(&png, &info);
}

bool jpeg_sig_cmp(unsigned char *buf)
{
    return (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF);
}

void save_jpeg(const std::string &path, array& arr) {
    arr.eval();
    if(arr.dtype() != uint8) {
        throw std::runtime_error("Image must be uint8");
    }
    if(arr.ndim() != 3) {
        throw std::runtime_error("Image must be 3d");
    }
    if(arr.shape(-1) != 3) {
        throw std::runtime_error("Image must have 3 channels");
    }
    FILE *fp = fopen(path.c_str(), "wb");
    if (!fp)
    {
        throw std::runtime_error("Failed to open file");
    }
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, fp);
    cinfo.image_width = arr.shape(1);
    cinfo.image_height = arr.shape(0);
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);
    jpeg_start_compress(&cinfo, TRUE);
    auto data = arr.data<unsigned char>();
    auto buffer = (JSAMPROW *)malloc(sizeof(JSAMPROW) * arr.shape(0));
    for(int i = 0; i < arr.shape(0); i++)
    {
        buffer[i] = (JSAMPROW)(data + i * arr.shape(1) * 3);
    }
    jpeg_write_scanlines(&cinfo, buffer, arr.shape(0));
    jpeg_finish_compress(&cinfo);
    fclose(fp);
    free(buffer);
    jpeg_destroy_compress(&cinfo);
}

array load_jpeg(const std::string &path)
{
    auto fp = load_file(path);
    unsigned char header[4];
    fread(header, 1, 3, fp);

    if (!jpeg_sig_cmp(header))
    {
        fclose(fp);
        throw std::runtime_error("File was not a valid jpg");
    }
    fseek(fp, 0, SEEK_SET);
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
    auto x = array(buffer, {height, width, channels});
    free(buffer);
    return x;
}