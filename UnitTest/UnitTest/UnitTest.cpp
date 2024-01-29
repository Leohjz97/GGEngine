#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GGEngine::GGMath;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(TestConstruct)
		{
			Vector3 v;
			Assert::AreEqual(v.x, 0.0f);
			Assert::AreEqual(v.y, 0.0f);
			Assert::AreEqual(v.z, 0.0f);
		}
	};
}
