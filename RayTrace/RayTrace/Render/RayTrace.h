﻿#pragma once
#include<math.h>
#include "RenderTarget.h"
#include"../Collider/Collider.h"
#include"../Material/Material.h"

namespace RayTraceRenderer
{
	class RayTraceCamera
	{
		//改成挂个transform或者写成unity的component形式来控制位置

		private:
		const unsigned int c_nSample = 16;
		const int c_nMaxStep = 64;
		const Color c_cSkyBoyLow = Color(1.0f, 1.0f, 1.0f);//暂时代替天空盒用
		const Color c_cSkyBoyTop = Color(0.5f, 0.7f, 1.0f);

		//vector3缓存，用于加快运算速度
		Vector3 m_vOrigin;
		Vector3 m_vHorizontal;
		Vector3 m_vVertical;
		Vector3 m_vLowLeftCorner;

		protected:
		RenderTarget & m_rTarget;

		float m_fNear = 0.3f;
		float m_fFar = 1000.0f;

		bool m_bOrthographic = false;//还没实现平行投影

		//Orthographic
		float m_fOrthographicSize = 5.0f;

		//Perspective
		float m_fFov = 60.0f;

		protected:
		bool m_bPerspectiveDataDirty = true;

		public:
		RayTraceCamera(RenderTarget & target)
			: m_rTarget(target) { }

		~RayTraceCamera() {}

		void Draw(const ColliderList& colliderObjects);

		inline float Aspect() const {
			return (float)m_rTarget.Width() / m_rTarget.Height();
		}

		inline float GetNear() const { return m_fNear; }

		inline void SetNear(float value) { m_fNear = value; m_bPerspectiveDataDirty = true; }

		inline float GetFar() const { return m_fFar; }

		inline void SetFar(float value) { m_fFar = value; m_bPerspectiveDataDirty = true; }

		inline float GetOrthographicSize() const { return m_fOrthographicSize; }

		inline void SetOrthographicSize(float value) { m_fOrthographicSize = value; m_bPerspectiveDataDirty = true; }

		inline float GetFov() const { return m_fFov; }

		inline void SetFov(float value) { m_fFov = value; m_bPerspectiveDataDirty = true; }

		protected:
		void _GetRay(Ray& ray, float u, float v);

		void _RayCatchColor(Ray& ray, const ColliderList& colliderObjects, Color& col, int step = 0);//ray先不用const复用同一个，后面有问题了再每次重新生成一个

		void _WriteColor(Color& target, const Color& value);

		private:
		void _UpdateRayScanData();
	};
}