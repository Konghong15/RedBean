#include "pch.h"

#include "Sample.h"

#include <random>

#include "../RedBean/Animation.h"
#include "../RedBean/Material.h"

#include "../SweetRedBean/Box.h"

namespace entryPoint
{
	Sample::Sample(HINSTANCE hInstance, UINT width, UINT height, std::wstring name)
		: Processor(hInstance, width, height, name)
	{
	}

	Sample::~Sample()
	{
	}

	bool Sample::Init()
	{
		if (!Processor::Init())
		{
			return false;
		}

		mCamera.SetLens(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 10000.0f);

		// new framework test
		mSweetGrapic = make_unique<Graphics>(mhWnd, mWidth, mHeight);
		mSweetGrapic->SetProjection(mCamera.GetProj());

		class Factory
		{
		public:
			Factory(Graphics& gfx)
				:
				gfx(gfx)
			{}
			std::unique_ptr<IDrawable> operator()()
			{
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
				);
			}
		private:
			Graphics& gfx;
			std::mt19937 rng{ std::random_device{}() };
			std::uniform_real_distribution<float> adist{ 0.0f,3.14 * 2.0f };
			std::uniform_real_distribution<float> ddist{ 0.0f,3.14 * 0.5f };
			std::uniform_real_distribution<float> odist{ 0.0f,3.14 * 0.08f };
			std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
			std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
			std::uniform_int_distribution<int> latdist{ 5,20 };
			std::uniform_int_distribution<int> longdist{ 10,40 };
			std::uniform_int_distribution<int> typedist{ 0,2 };
		};

		int num = 10;

		Factory f(*mSweetGrapic.get());
		mDrawables.reserve(num);
		std::generate_n(std::back_inserter(mDrawables), num, f);

		return true;
	}

	void Sample::Update(float deltaTime)
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}

		for (auto& d : mDrawables)
		{
			d->Update(deltaTime);
		}
	}

	void Sample::Render()
	{
		if (mSweetGrapic == nullptr)
		{
			return;
		}

		mSweetGrapic->BeginFrame();

		for (auto& d : mDrawables)
		{
			d->Draw(*mSweetGrapic.get());
		}

		mSweetGrapic->EndFrame();
	}
}