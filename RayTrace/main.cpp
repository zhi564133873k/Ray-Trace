#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>  

#include "Object/GameObject.h"
#include "Render/RayTrace.h"
#include "PPMWriter.h"
#include "Collider/Sphere.h"
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include "Material/Dielectric.h"

using namespace std;
using namespace chrono;
using namespace RayTraceRenderer;

int main(int argc, char **argv)
{
	string outPutPath = "output.ppm";
	int pixelWidth = 800;
	int pixelHeight = 600;
	float aspect = (float)pixelWidth / pixelHeight;
	RenderTarget target(pixelWidth, pixelHeight);
	
	GameObject gobj;

	Transform& tran = gobj.GetTransform();
	RayTraceCamera& camera = gobj.AddComponent<RayTraceCamera>(target);

	tran.SetLocalPosition(Vector3(-2, 2, 1));
	tran.LookAt(Vector3::Zero());

	ColliderList worldObjectList;

	Lambertian ballMat(Vector3(0.1f, 0.2f, 0.5f));
	Lambertian groundMat(Vector3(0.8f, 0.8f, 0.0f));

	Metal metal1(Vector3(0.8f, 0.6f, 0.2f), 0.2f);
	Metal metal2(Vector3(0.8f, 0.8f, 0.8f), 0.5f);

	Dielectric diele(1.5f);

	Sphere ball(Vector3(0.0f, 0.0f, 0.0f), 0.5f, &ballMat);
	Sphere ground(Vector3(0.0f, -100.5f, 0.0f), 100.0f, &groundMat);

	Sphere ball1(Vector3(1.0f, 0.0f, 0.0f), 0.5f, &metal1);
	Sphere ball2(Vector3(-1.0f, 0.0f, 0.0f), 0.5f, &diele);
	
	worldObjectList.AddCollider(&ball);
	worldObjectList.AddCollider(&ground);
	worldObjectList.AddCollider(&ball1);
	worldObjectList.AddCollider(&ball2);

	cout << "初始化完成，开始输出" << endl;

	system_clock::time_point startTime = system_clock::now();
	
	camera.Draw(worldObjectList);

	system_clock::time_point drawCompleteTime = system_clock::now();

	cout << "光线追踪完成，耗时" << double(duration_cast<microseconds>(drawCompleteTime - startTime).count()) * microseconds::period::num / microseconds::period::den << "秒" << endl;

	if (WritePPMFile(outPutPath, target))
	{
		cout << "输出图片" << outPutPath << "成功，输出耗时" << double(duration_cast<microseconds>(system_clock::now() - drawCompleteTime).count()) * microseconds::period::num / microseconds::period::den << "秒";
	}
	else
	{
		cout << "输出图片" << outPutPath << "失败";
	}
	cin.get();
    return 0;
}

