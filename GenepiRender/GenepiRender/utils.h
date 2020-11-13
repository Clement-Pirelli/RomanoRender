#pragma once
#include <vector>
#include <random>

#define _CRT_SECURE_NO_WARNINGS

struct Vertex { float x, y, z, r; };
struct Triangle { int v0, v1, v2; };

#include <embree3/rtcore.h>

void errorFunction(void* userPtr, enum RTCError error, const char* str)
{
    printf("error %d: %s\n", error, str);
}

RTCDevice initializeDevice()
{
    RTCDevice device = rtcNewDevice(NULL);

    if (!device)
        printf("error %d: cannot create device\n", rtcGetDeviceError(NULL));

    rtcSetDeviceErrorFunction(device, errorFunction, NULL);
    return device;
}

extern "C" void abort();
extern "C" void exit(int);
extern "C" int puts(const char* str);
extern "C" int putchar(int character);


vec3 generate_random_vector(float min, float max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    vec3 random(dist(mt));
    return random;
}


float generate_random_float(float min, float max)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}


template <typename T>
void
substract_vector(std::vector<T>& a, const std::vector<T>& b)
{
    typename std::vector<T>::iterator       it = a.begin();
    typename std::vector<T>::const_iterator it2 = b.begin();

    while (it != a.end())
    {
        while (it2 != b.end() && it != a.end())
        {
            if (*it == *it2)
            {
                it = a.erase(it);
                it2 = b.begin();
            }

            else
                ++it2;
        }
        if (it != a.end())
            ++it;

        it2 = b.begin();
    }
}


inline float deg2rad(const float& deg)
{
    return deg * M_PI / 180;
}


template <typename T>
T
lerp(T& a, T& b, float& t) 
{
    return (1 - t) * a + t * b;
}


vec3 reflect(vec3& i, vec3& n, float& R)
{
    vec3 random = generate_random_vector(-1.0, 1.0);
    vec3 r = i - 2 * dot(i, n) * n + lerp(vec3(0), random, R);
    return r.normalize();
}


vec3 refract(vec3& i, vec3& n, float ior) 
{
    vec3 T(0.f);
    vec3 n_t = n.normalize();
    float n1 = 1.0;
    float n2 = ior;

    if (dot(i.normalize(), n) > 0.0)
    {
        n1 = ior;
        n2 = 1.0;
        n_t = -n;
    }
    
    float dt = dot(i.normalize(), n_t);
    float n_ior = n1 / n2;

    float radical = 1.0 - (n_ior * n_ior) * (1.0 - dt * dt);

    if (radical > 0.0)
    {
        T = (n1 / n2) * (i.normalize() - dt * n_t) - n_t * sqrt(radical);
    }
    return T;
}

/*
vec3 refract(vec3& i, vec3& n, float ior)
{
    ior = 2.f - ior;
    float cosi = dot(n, i);
    vec3 o = i * ior - n * (-cosi + ior * cosi);
    return o;
}*/


float modulo(float x)
{
    return x - std::floor(x);
}


float estimate_sample_variance(vec3 samples[], int n)
{
    vec3 sum = 0;
    vec3 sum_sq = 0;

    for (int i = 0; i < n; i++)
    {
        sum += samples[i];
        sum_sq += samples[i] * samples[i];
    }
    
    float var_sum = sum_sq.x / (n * (n - 1)) - sum.x * sum.x / ((n - 1) * n * n) +
                    sum_sq.y / (n * (n - 1)) - sum.y * sum.y / ((n - 1) * n * n) +
                    sum_sq.z / (n * (n - 1)) - sum.z * sum.z / ((n - 1) * n * n);

    return var_sum / 3;
}


inline vec3 face_forward(const vec3& dir, const vec3& _Ng) {
    const vec3 Ng = _Ng;
    return dot(dir, Ng) < 0.0f ? Ng : Ng * -1;
}
