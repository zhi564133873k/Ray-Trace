#include "stdafx.h"

using namespace std;
using namespace chrono;
using namespace RayTraceRenderer;

int main(int argc, char **argv)
{
	string outPutPath = "output.ppm";
	int width = 800;
	int height = 600;
	PPMWriter writer(width, height);
	RayTrace renderer;

	system_clock::time_point startTime = system_clock::now();

	renderer.DrawToBuffer(writer.GetFrameBuffer(), width, height);

	double drawTime = double(duration_cast<microseconds>(system_clock::now() - startTime).count()) * microseconds::period::num / microseconds::period::den;

	if (writer.WritePPMFile(outPutPath))
	{
		cout << "输出图片" << outPutPath << "成功，耗时" << drawTime << "毫秒";
	}
	else
	{
		cout << "输出图片" << outPutPath << "失败";
	}
	cin.get();
    return 0;
}

