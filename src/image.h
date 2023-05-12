#ifndef IMAGE_H
#define IMAGE_H

// DO NOT CHANGE THIS FILE

typedef struct{
    int w,h,c;
    float *data;
} image;

// Basic operations
__declspec(dllexport) float get_pixel(image im, int x, int y, int c);
__declspec(dllexport)  void set_pixel(image im, int x, int y, int c, float v);
__declspec(dllexport) image copy_image(image im);
__declspec(dllexport) image rgb_to_grayscale(image im);
__declspec(dllexport) image grayscale_to_rgb(image im, float r, float g, float b);
__declspec(dllexport) void rgb_to_hsv(image im);
__declspec(dllexport) void hsv_to_rgb(image im);
__declspec(dllexport) void shift_image(image im, int c, float v);
__declspec(dllexport) void scale_image(image im, int c, float v);
__declspec(dllexport) void clamp_image(image im);

// Loading and saving
__declspec(dllexport) image make_image(int w, int h, int c);
__declspec(dllexport) image load_image(char *filename);
__declspec(dllexport) void save_image(image im, const char *name);
__declspec(dllexport) void save_png(image im, const char *name);
__declspec(dllexport) void free_image(image im);

// Resizing
__declspec(dllexport) float nn_interpolate(image im, float x, float y, int c);
__declspec(dllexport) image nn_resize(image im, int w, int h);
__declspec(dllexport) float bilinear_interpolate(image im, float x, float y, int c);
__declspec(dllexport) image bilinear_resize(image im, int w, int h);



#endif

