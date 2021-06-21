#ifndef TEST_H
#define TEST_H

#include "../vendor/imgui/imgui.h"

#include <iostream>
#include <vector>
#include <string>
#include <functional>

namespace test {

	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(double deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnExit() {}
	};

	class TestMenu : public Test {
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "register test" << name << std::endl;
			m_tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

		void DEBUG_SetActiveTest(const std::string& testName) {
			for (auto& test : m_tests) {
				if (test.first == testName){
					m_CurrentTest = test.second();
					break;
				}
			}
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_tests;
	};
}

#endif // !TEST_H
