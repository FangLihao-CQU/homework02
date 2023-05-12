#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "image.h"
#include "test.h"
#include "args.h"

int tests_total = 0;
int tests_fail = 0;

int within_eps(float a, float b){
    return a-EPS<b && b<a+EPS;
}

int same_image(image a, image b){
    int i;
    if(a.w != b.w || a.h != b.h || a.c != b.c) {
        printf("Expected %d x %d x %d image, got %d x %d x %d\n", b.w, b.h, b.c, a.w, a.h, a.c);
        return 0;
    }
    for(i = 0; i < a.w*a.h*a.c; ++i){
        if(!within_eps(a.data[i], b.data[i])) 
        {
            printf("The value should be %f, but it is %f! \n", b.data[i], a.data[i]);
            return 0;
        }
    }
    return 1;
}

void test_get_pixel(){
    image im = load_image("data/dots.png");
    // Test within image
    TEST(within_eps(0, get_pixel(im, 0,0,0)));
    TEST(within_eps(1, get_pixel(im, 1,0,1)));
    TEST(within_eps(0, get_pixel(im, 2,0,1)));

    // Test padding
    TEST(within_eps(1, get_pixel(im, 0,3,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,0)));
    TEST(within_eps(0, get_pixel(im, 7,8,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,2)));
    free_image(im);
}

void test_set_pixel(){
    image gt = load_image("data/dots.png");
    image d = make_image(4,2,3);
    set_pixel(d, 0,0,0,0); set_pixel(d, 0,0,1,0); set_pixel(d, 0,0,2,0); 
    set_pixel(d, 1,0,0,1); set_pixel(d, 1,0,1,1); set_pixel(d, 1,0,2,1); 
    set_pixel(d, 2,0,0,1); set_pixel(d, 2,0,1,0); set_pixel(d, 2,0,2,0); 
    set_pixel(d, 3,0,0,1); set_pixel(d, 3,0,1,1); set_pixel(d, 3,0,2,0); 

    set_pixel(d, 0,1,0,0); set_pixel(d, 0,1,1,1); set_pixel(d, 0,1,2,0); 
    set_pixel(d, 1,1,0,0); set_pixel(d, 1,1,1,1); set_pixel(d, 1,1,2,1); 
    set_pixel(d, 2,1,0,0); set_pixel(d, 2,1,1,0); set_pixel(d, 2,1,2,1); 
    set_pixel(d, 3,1,0,1); set_pixel(d, 3,1,1,0); set_pixel(d, 3,1,2,1); 

    // Test images are same
    TEST(same_image(d, gt));
    free_image(gt);
    free_image(d);
}

void test_grayscale()
{
    image im = load_image("data/colorbar.png");
    image gray = rgb_to_grayscale(im);
    image gt = load_image("figs/gray.png");
    TEST(same_image(gray, gt));
    free_image(im);
    free_image(gray);
    free_image(gt);
}

void test_copy()
{
    image gt = load_image("data/dog.jpg");
    image c = copy_image(gt);
    TEST(same_image(c, gt));
    free_image(gt);
    free_image(c);
}

void test_shift()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    shift_image(c, 1, .1);
    TEST(within_eps(c.data[0], im.data[0]));
    TEST(within_eps(c.data[im.w*im.h + 13], im.data[im.w*im.h+13] + .1));
    TEST(within_eps(c.data[2*im.w*im.h + 72], im.data[2*im.w*im.h+72]));
    TEST(within_eps(c.data[im.w*im.h + 47], im.data[im.w*im.h+47] + .1));
    free_image(im);
    free_image(c);
}

void test_rgb_to_hsv()
{
    image im = load_image("data/dog.jpg");
    rgb_to_hsv(im);
    image hsv = load_image("figs/dog.hsv.png");
    TEST(same_image(im, hsv));
    free_image(im);
    free_image(hsv);
}

void test_hsv_to_rgb()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    rgb_to_hsv(im);
    hsv_to_rgb(im);
    TEST(same_image(im, c));
    free_image(im);
    free_image(c);
}

void test_nn_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = nn_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-nn-for-test.png");
    TEST(same_image(resized, gt));
    free_image(im);
    free_image(resized);
    free_image(gt);

    image im2 = load_image("data/dog.jpg");
    image resized2 = nn_resize(im2, 713, 467);
    image gt2 = load_image("figs/dog-resize-nn.png");
    TEST(same_image(resized2, gt2));
    free_image(im2);
    free_image(resized2);
    free_image(gt2);
}

void test_bl_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = bilinear_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-bl.png");
    TEST(same_image(resized, gt));
    free_image(im);
    free_image(resized);
    free_image(gt);

    image im2 = load_image("data/dog.jpg");
    image resized2 = bilinear_resize(im2, 713, 467);
    image gt2 = load_image("figs/dog-resize-bil.png");
    TEST(same_image(resized2, gt2));
    free_image(im2);
    free_image(resized2);
    free_image(gt2);
}

void test_multiple_resize()
{
    image im = load_image("data/dog.jpg");
    int i;
    for (i = 0; i < 10; i++){
        image im1 = bilinear_resize(im, im.w*4, im.h*4);
        image im2 = bilinear_resize(im1, im1.w/4, im1.h/4);
        free_image(im);
        free_image(im1);
        im = im2;
    }
    image gt = load_image("figs/dog-multipleresize.png");
    TEST(same_image(im, gt));
    free_image(im);
    free_image(gt);
}





int do_test()
{
    TEST('1' == '1');
    TEST('0' == '1');
    return 0;
}

void run_tests()
{
    test_get_pixel();
    test_set_pixel();
    test_copy();
    test_shift();
    test_grayscale();
    test_rgb_to_hsv();
    test_hsv_to_rgb();
    test_nn_resize();
    test_bl_resize();
    test_multiple_resize();
   
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}

