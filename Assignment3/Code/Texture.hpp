//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        
        float a = floor(u_img) + 0.5f;
        float b = floor(v_img) + 0.5f;

        float u_min;
        float v_min;

        if (u_img < a && v_img > b)
        {
            u_min = a - 1.0f;
            v_min = b + 1.0f;
        }
        else if (u_img < a && v_img < b)
        {
            u_min = a - 1.0f;
            v_min = b;
        }
        else if (u_img > a && v_img > b)
        {
            u_min = a;
            v_min = b + 1.0f;
        }
        else if (u_img > a && v_img < b)
        {
            u_min = a;
            v_min = b;
        }

        float s = v_min - v_img;
        float t = u_img - u_min; 

        auto s1 = image_data.at<cv::Vec3b>(v_min - 1.0f, u_min);
        auto s2 = image_data.at<cv::Vec3b>(v_min - 1.0f, u_min + 1.0f);
        auto s3 = image_data.at<cv::Vec3b>(v_min, u_min);
        auto s4 = image_data.at<cv::Vec3b>(v_min, u_min + 1.0f);

        auto color = (t * s3 + (1 - t) * s4) * s + (1 - s) * (t * s1 + (1 - t) * s2);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

};
#endif //RASTERIZER_TEXTURE_H
