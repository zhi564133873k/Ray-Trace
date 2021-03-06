#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>  
#include<vector>

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
	RenderTarget target(pixelWidth, pixelHeight);
	
	GameObject gobj;
	Transform& tran = gobj.GetTransform();
	RayTraceCamera& camera = gobj.AddComponent<RayTraceCamera>(target);
	ColliderList worldObjectList;


	//Vector3 cameraPos(3, 3, -3);
	//tran.SetLocalPosition(cameraPos);
	//tran.LookAt(Vector3::Zero());

	//camera.OpenDepthOfField(2.0f);
	//camera.SetFocus(Distance(cameraPos, Vector3::Zero()));

	//Lambertian ballMat(Vector3(0.1f, 0.2f, 0.5f));
	//Lambertian groundMat(Vector3(0.8f, 0.8f, 0.0f));

	//Metal metal1(Vector3(0.8f, 0.6f, 0.2f), 0.2f);
	//Metal metal2(Vector3(0.8f, 0.8f, 0.8f), 0.5f);

	//Dielectric diele(1.5f);

	//Sphere ball(Vector3(0.0f, 0.0f, 0.0f), 0.5f, &ballMat);
	//Sphere ground(Vector3(0.0f, -100.5f, 0.0f), 100.0f, &groundMat);

	//Sphere ball1(Vector3(1.0f, 0.0f, 0.0f), 0.5f, &metal1);
	//Sphere ball2(Vector3(-1.0f, 0.0f, 0.0f), 0.5f, &diele);
	//Sphere ball3(Vector3(-1.0f, 0.0f, 0.0f), -0.45f, &diele);
	//
	//worldObjectList.AddCollider(&ball);
	//worldObjectList.AddCollider(&ground);
	//worldObjectList.AddCollider(&ball1);
	//worldObjectList.AddCollider(&ball2);
	//worldObjectList.AddCollider(&ball3);

	Vector3 cameraPos(8, 2, -2);
	tran.SetLocalPosition(cameraPos);
	tran.LookAt(Vector3::Zero());

	camera.OpenDepthOfField(0.05f);
	camera.SetFocus(Distance(cameraPos, Vector3::Zero()));

	vector<IBaseMaterial*> mats;
	vector<ICollider*> objects;
	vector<unsigned int> handles;
	ICollider* obj = nullptr;
	IBaseMaterial* mat = nullptr;
	Lambertian groundMat(Vector3(0.5f, 0.5f, 0.5f));
	Sphere ground(Vector3(0.0f, -1000.0f, 0.0f), 1000.0f, &groundMat);
	worldObjectList.AddCollider(&ground);

	Vector3 groundCenter(4.0f, 0.2f, 0.0f);

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			float matType = (float)Random();
			Vector3 center(a + 0.9f * (float)Random(), 0.2f, b + 0.9f * (float)Random());
			if (Distance(groundCenter, center) > 0.9f) {

				if (matType < 0.8f) {
					mat = new Lambertian(Vector3((float)Random()*(float)Random(), (float)Random()*(float)Random(), (float)Random()*(float)Random()));
				} else if (matType < 0.95f) {
					mat = new Metal(Vector3(0.5f * (1.0f + (float)Random()), 0.5f * (1.0f + (float)Random()), 0.5f * (1.0f + (float)Random())), 0.5f *(float)Random());
				} else {
					mat = new Dielectric(1.5f);
				}
				obj = new Sphere(center, 0.2f, mat);
				mats.push_back(mat);
				objects.push_back(obj);
				handles.push_back(worldObjectList.AddCollider(obj));
			}
		}
	}

	Lambertian lambert(Vector3(0.4f, 0.2f, 0.1f));
	Metal metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f);
	Dielectric diele(1.5f);
	
	Sphere lambertBall(Vector3(-4.0f, 1.0f, 0.0f), 1.0f, &lambert);
	Sphere metalBall(Vector3(4.0f, 1.0f, 0.0f), 1.0f, &metal);
	Sphere dieleBall(Vector3(0.0f, 1.0f, 0.0f), 1.0f, &diele);

	worldObjectList.AddCollider(&lambertBall);
	worldObjectList.AddCollider(&metalBall);
	worldObjectList.AddCollider(&dieleBall);


	cout << "初始化完成，开始输出" << endl;

	system_clock::time_point startTime = system_clock::now();
	
	camera.Draw(worldObjectList);

	for (unsigned int handle : handles)
	{
		worldObjectList.RemoveCollider(handle);
	}
	handles.clear();

	for (ICollider* obj : objects)
	{
		delete obj;
	}
	objects.clear();

	for (IBaseMaterial* mat : mats) {
		delete mat;
	}
	mats.clear();

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

