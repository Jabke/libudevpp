#include <stdexcept>

#include <libudev.h>

#include <udevpp.hpp>

namespace Udev
{
	UdevMonitor::UdevMonitor(UdevMonitorHandle *monitor) : handle(monitor)
	{
	}

	UdevMonitor::UdevMonitor(const UdevMonitor& other) : handle(udev_monitor_ref(other.handle))
	{
	}

	UdevMonitor::UdevMonitor(UdevMonitor&& other) : handle(other.handle)
	{
		other.handle = nullptr;
	}

	UdevMonitor& UdevMonitor::operator=(const UdevMonitor& other)
	{
		handle = udev_monitor_ref(other.handle);
		return *this;
	}

	UdevMonitor& UdevMonitor::operator=(UdevMonitor&& other)
	{
		handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	UdevMonitor::~UdevMonitor()
	{
		if (handle)
		{
			udev_monitor_unref(handle);
		}
	}

	void UdevMonitor::enable_receiving() const
	{
		if (udev_monitor_enable_receiving(handle) != 0)
		{
			throw std::runtime_error("Unable to enable receiving of events");
		}
	}

	int UdevMonitor::get_fd() const
	{
		return udev_monitor_get_fd(handle);
	}

	UdevDevice UdevMonitor::receive_device() const
	{
		auto device = udev_monitor_receive_device(handle);
		if (device == nullptr)
		{
			throw std::runtime_error("Device could not be received");
		}
		return UdevDevice(device);
	}
}
