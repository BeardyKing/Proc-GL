#ifndef TEST_DOCK_H
#define TEST_DOCK_H

#include "test.h"

namespace test {
	class test_Dock : public Test
	{
	public:
		test_Dock();
		~test_Dock();

		void OnUpdate(double deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

	};
}

#endif // !TEST_DOCK_H
