#include "stdafx.h"
#include "CppUnitTest.h"
#include "CVkInstance.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace vesuvius;

namespace vesuviusTest
{
	TEST_CLASS(CVkPhysicalDeviceTest)
	{
	private:
		static std::shared_ptr<CVkInstance>   m_instance;
		static std::vector<CVkPhysicalDevice> m_physicalDevices;

	public:

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			m_instance = CreateInstance();
			Assert::IsNotNull(m_instance.get());

			VkResult result = m_instance->EnumeratePhysicalDevices(m_physicalDevices);
			Assert::AreEqual(result, VK_SUCCESS);
		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			m_physicalDevices.clear();
			m_instance.reset();
		}

		TEST_METHOD(GetPhysicalDeviceProperties_Success)
		{
			for each(CVkPhysicalDevice physicalDevice in m_physicalDevices)
			{
				VkPhysicalDeviceProperties properties;
				memset(&properties, 0, sizeof(properties));

				properties = physicalDevice.GetPhysicalDeviceProperties();

				Assert::IsNotNull(properties.deviceName);
				Assert::IsTrue(properties.vendorID > 0);
			}
		}

		TEST_METHOD(GetPhysicalDeviceQueueFamilyProperties_Success)
		{
			for each(CVkPhysicalDevice physicalDevice in m_physicalDevices)
			{
				std::vector<VkQueueFamilyProperties> properties;
				VkResult result = physicalDevice.GetPhysicalDeviceQueueFamilyProperties(properties);

				Assert::AreEqual(result, VK_SUCCESS);
				Assert::IsTrue(properties.size() > 0);
			}
		}

		TEST_METHOD(CreateDevice_Success)
		{
			for each(CVkPhysicalDevice physicalDevice in m_physicalDevices)
			{
				std::shared_ptr<CVkDevice> device;

				VkDeviceCreateInfo deviceCreateInfo;
				VkDeviceQueueCreateInfo deviceQueueCreateInfo;

				memset(&deviceCreateInfo, 0, sizeof(deviceCreateInfo));
				memset(&deviceQueueCreateInfo, 0, sizeof(deviceQueueCreateInfo));

				deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				deviceQueueCreateInfo.queueCount = 1;
				deviceQueueCreateInfo.queueFamilyIndex = 0;

				deviceCreateInfo.queueCreateInfoCount = 1;
				deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

				VkResult result =  physicalDevice.CreateDevice(deviceCreateInfo,
																NULL,
																device);
				Assert::AreEqual(result, VK_SUCCESS);
				Assert::IsNotNull(device.get());
			}
		}

	};

	std::shared_ptr<CVkInstance>   CVkPhysicalDeviceTest::m_instance;
	std::vector<CVkPhysicalDevice> CVkPhysicalDeviceTest::m_physicalDevices;

}