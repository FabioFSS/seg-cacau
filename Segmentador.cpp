#include "segmentador.h"


Segmentador::Segmentador(string path, string path2)
{
    folder_samples = "samples";
    folder_result = "result";
    generated_format = "png";
    caminho_raiz = "";
    image = imread(path);
    original = imread(path2);
    n_image = "temp";
    Mat canny_image = first_pass();
    second_pass(n_image, canny_image);
    third_pass();
    fourth_pass(n_image);
}

Mat Segmentador::first_pass()
{
    Mat gray, blur, thresh, bgt1, canny_image;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    //GaussianBlur(gray, blur, Size(5, 5), 0);
    //medianBlur(gray, blur, 5);
    threshold(gray, thresh, 0, 255, THRESH_BINARY + THRESH_OTSU);
    dilate(thresh, bgt1, Mat(), Point(-1, -1), 7);
    erode(bgt1, bgt, Mat(), Point(-1, -1), 7);
    threshold(bgt, bg, 1, 128, 1);
    Canny(bg, canny_image, 255, 255);

    //imwrite(caminho_raiz + folder_result + "/" + "gaussian" + "." + generated_format, blur);
    imwrite(caminho_raiz + folder_result + "/" + "thresh1" + "." + generated_format, thresh);
    imwrite(caminho_raiz + folder_result + "/" + "dilate" + "." + generated_format, bgt1);
    imwrite(caminho_raiz + folder_result + "/" + "erode" + "." + generated_format, bgt);
    imwrite(caminho_raiz + folder_result + "/" + "thresh2" + "." + generated_format, bg);
    imwrite(caminho_raiz + folder_result + "/" + "bordas" + "." + generated_format, canny_image);

    return canny_image;
}

void Segmentador::second_pass(string n_image, Mat canny_image)
{
    Mat m, thresh, thresh_inv, res, res2, res3;
    vector<Vec4i> hierarchy;

    findContours(canny_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    convertScaleAbs(bg, m);
    threshold(m, thresh, 0, 255, THRESH_BINARY + THRESH_OTSU);
    bitwise_not(thresh, thresh_inv);
    bitwise_and(original, original, res, thresh);
    bitwise_and(original, original, res2, thresh_inv);
    addWeighted(res, 1, res2, 1, 0, res3);
    drawContours(res3, contours, -1, Scalar(0, 0, 255));
    imwrite(caminho_raiz + folder_result + "/" + n_image + "." + generated_format, res3);
}

void Segmentador::third_pass()
{
    Mat m, thresh, mask;

    convertScaleAbs(bgt, m);
    threshold(m, thresh, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
    bitwise_and(original, original, res, thresh);
    inRange(res, Scalar(0, 0, 0), Scalar(0, 0, 0), mask);
    res.setTo(Scalar(255, 255, 255), mask);
}

void Segmentador::fourth_pass(string n_image)
{
    Mat new_img;
    Rect rect;
    int idx = 0, count = 0;
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        rect = boundingRect(contours[i]);
        idx += 1;
        if (idx % 2 == 0)
        {
            count += 1;
            new_img = res(rect);
            imwrite(caminho_raiz + folder_samples + "/" + n_image + "_" + to_string(count) + "." + generated_format, new_img);
        }
    }
}
