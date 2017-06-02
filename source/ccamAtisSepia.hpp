#pragma once

#include <sepia.hpp>
#include <libusb-1.0/libusb.h>

#include <array>

/// ccamAtisSepia specialises Sepia for the CCam ATIS.
/// In order to use this header, an application must link to the dynamic library usb-1.0.
namespace ccamAtisSepia {

    /// Camera represents a CCam ATIS.
    class Camera {
        public:

            /// availableSerials returns the connected CCam ATIS cameras' serials.
            static std::unordered_set<uint16_t> availableSerials() {
                auto serials = std::unordered_set<uint16_t>{};
                libusb_context* context;
                checkUsbError(libusb_init(&context), "initialize the USB context");
                libusb_device** devices;
                const auto count = libusb_get_device_list(context, &devices);
                for (std::size_t index = 0; index < count; ++index) {
                    libusb_device_descriptor descriptor;
                    if (libusb_get_device_descriptor(devices[index], &descriptor) == 0) {
                        if (descriptor.idVendor == 1204 && descriptor.idProduct == 244) {
                            libusb_device_handle* handle;
                            checkUsbError(libusb_open(devices[index], &handle), "open the device");
                            if (libusb_claim_interface(handle, 0) == 0) {
                                auto data = std::array<uint8_t, 8>{};
                                checkUsbError(libusb_control_transfer(handle, 192, 85, 32, 0, data.data(), data.size(), 0), "send control packet");
                                libusb_release_interface(handle, 0);
                                serials.insert((static_cast<uint16_t>(data[6]) << 8) | static_cast<uint16_t>(data[7]));
                            }
                            libusb_close(handle);
                        }
                    }
                }
                libusb_free_device_list(devices, 1);
                libusb_exit(context);
                return serials;
            }

            /// defaultParameter returns the default parameter used by the CCam ATIS.
            static std::unique_ptr<sepia::Parameter> defaultParameter() {
                return sepia::make_unique<sepia::ObjectParameter>(
                    "changeDetection",             sepia::make_unique<sepia::ObjectParameter>(
                        "resetSwitchBulkPotential",     sepia::make_unique<sepia::CharParameter>(207),
                        "photoreceptorFeedback",        sepia::make_unique<sepia::CharParameter>(243),
                        "refractoryPeriod",             sepia::make_unique<sepia::CharParameter>(216),
                        "follower",                     sepia::make_unique<sepia::CharParameter>(239),
                        "eventSourceAmplifier",         sepia::make_unique<sepia::CharParameter>( 42),
                        "onEventThreshold",             sepia::make_unique<sepia::CharParameter>( 51),
                        "offEventThreshold",            sepia::make_unique<sepia::CharParameter>( 39),
                        "offEventInverter",             sepia::make_unique<sepia::CharParameter>( 61),
                        "cascodePhotoreceptorFeedback", sepia::make_unique<sepia::CharParameter>(154)
                    ),
                    "exposureMeasurement",         sepia::make_unique<sepia::ObjectParameter>(
                        "comparatorTail",        sepia::make_unique<sepia::CharParameter>( 54),
                        "comparatorHysteresis",  sepia::make_unique<sepia::CharParameter>( 47),
                        "comparatorOutputStage", sepia::make_unique<sepia::CharParameter>( 57),
                        "upperThreshold",        sepia::make_unique<sepia::CharParameter>(243),
                        "lowerThreshold",        sepia::make_unique<sepia::CharParameter>(235)
                    ),
                    "pullup",                      sepia::make_unique<sepia::ObjectParameter>(
                        "exposureMeasurementAbscissaRequest", sepia::make_unique<sepia::CharParameter>(131),
                        "exposureMeasurementOrdinateRequest", sepia::make_unique<sepia::CharParameter>(155),
                        "changeDetectionAbscissaRequest",     sepia::make_unique<sepia::CharParameter>(151),
                        "changeDetectionOrdinateRequest",     sepia::make_unique<sepia::CharParameter>(117),
                        "abscissaAcknoledge",                 sepia::make_unique<sepia::CharParameter>(162),
                        "abscissaEncoder",                    sepia::make_unique<sepia::CharParameter>(162),
                        "ordinateEncoder",                    sepia::make_unique<sepia::CharParameter>(120)
                    ),
                    "control",                     sepia::make_unique<sepia::ObjectParameter>(
                        "exposureMeasurementTimeout",           sepia::make_unique<sepia::CharParameter>( 49),
                        "sequentialExposureMeasurementTimeout", sepia::make_unique<sepia::CharParameter>( 45),
                        "abscissaAcknoledgeTimeout",            sepia::make_unique<sepia::CharParameter>( 56),
                        "latchCellScanPulldown",                sepia::make_unique<sepia::CharParameter>(134),
                        "abscissaRequestPulldown",              sepia::make_unique<sepia::CharParameter>( 87)
                    )
                );
            }

            /// width returns the sensor width.
            static constexpr std::size_t width() {
                return 304;
            }

            /// height returns the sensor height.
            static constexpr std::size_t height() {
                return 240;
            }

            /// configuration contains the settings for the digital-to-analog converters on the FPGA.
            static std::unordered_map<
                std::string, std::unordered_map<
                    std::string, std::unordered_map<
                        std::string, uint32_t>>> configuration() {
                return std::unordered_map<
                    std::string, std::unordered_map<
                        std::string, std::unordered_map<
                            std::string, uint32_t>>>({
                    {"changeDetection", {
                        {"resetSwitchBulkPotential",     {{"address", 0x02}, {"tension", 0x5900}}},
                        {"photoreceptorFeedback",        {{"address", 0x03}, {"tension", 0x5900}}},
                        {"refractoryPeriod",             {{"address", 0x04}, {"tension", 0x5900}}},
                        {"follower",                     {{"address", 0x05}, {"tension", 0x5900}}},
                        {"eventSourceAmplifier",         {{"address", 0x06}, {"tension", 0x7900}}},
                        {"onEventThreshold",             {{"address", 0x07}, {"tension", 0x7900}}},
                        {"offEventThreshold",            {{"address", 0x08}, {"tension", 0x7900}}},
                        {"offEventInverter",             {{"address", 0x09}, {"tension", 0x7900}}},
                        {"cascodePhotoreceptorFeedback", {{"address", 0x0a}, {"tension", 0x7900}}},
                    }},
                    {"exposureMeasurement", {
                        {"comparatorTail",        {{"address", 0x0b}, {"tension", 0x7900}}},
                        {"comparatorHysteresis",  {{"address", 0x0c}, {"tension", 0x7900}}},
                        {"comparatorOutputStage", {{"address", 0x0d}, {"tension", 0x7900}}},
                        {"upperThreshold",        {{"address", 0x0e}, {"tension", 0x5900}}},
                        {"lowerThreshold",        {{"address", 0x0f}, {"tension", 0x5900}}},
                    }},
                    {"pullup", {
                        {"exposureMeasurementAbscissaRequest", {{"address", 0x10}, {"tension", 0x5900}}},
                        {"exposureMeasurementOrdinateRequest", {{"address", 0x11}, {"tension", 0x5900}}},
                        {"changeDetectionAbscissaRequest",     {{"address", 0x12}, {"tension", 0x5900}}},
                        {"changeDetectionOrdinateRequest",     {{"address", 0x13}, {"tension", 0x7900}}},
                        {"abscissaAcknoledge",                 {{"address", 0x14}, {"tension", 0x5900}}},
                        {"abscissaEncoder",                    {{"address", 0x15}, {"tension", 0x5900}}},
                        {"ordinateEncoder",                    {{"address", 0x16}, {"tension", 0x7900}}},
                    }},
                    {"control", {
                        {"exposureMeasurementTimeout",           {{"address", 0x17}, {"tension", 0x7900}}},
                        {"sequentialExposureMeasurementTimeout", {{"address", 0x18}, {"tension", 0x7900}}},
                        {"abscissaAcknoledgeTimeout",            {{"address", 0x19}, {"tension", 0x7900}}},
                        {"latchCellScanPulldown",                {{"address", 0x1a}, {"tension", 0x5900}}},
                        {"abscissaRequestPulldown",              {{"address", 0x1b}, {"tension", 0x7900}}},
                    }},
                    {"static", {
                        {"resetTimestamp",   {{"address", 0x00}, {"tension", 0x5900}, {"value", 0}}},
                        {"testEvent",        {{"address", 0x01}, {"tension", 0x7900}, {"value", 0}}},
                        {"resetPhotodiodes", {{"address", 0x1c}, {"tension",   0x00}, {"value", 3}}},
                    }},
                });
            }

            Camera() {}
            Camera(const Camera&) = delete;
            Camera(Camera&&) = default;
            Camera& operator=(const Camera&) = delete;
            Camera& operator=(Camera&&) = default;
            virtual ~Camera() {}

            /// trigger sends a trigger signal to the camera.
            /// With default settings, this signal will trigger a change detection on every pixel.
            virtual void trigger() = 0;

        protected:

            /// checkUsbError throws if the given value is not zero.
            static void checkUsbError(int error, const std::string& message) {
                if (error < 0) {
                    throw std::logic_error(message + " failed: " + libusb_strerror(static_cast<libusb_error>(error)));
                }
            }

            /// sendCommand sends a setup command to the given camera.
            static void sendCommand(libusb_device_handle* handle, uint16_t wValue, std::array<uint8_t, 4> data, const std::string& message) {
                checkUsbError(libusb_control_transfer(handle, 64, 86, wValue, 0, data.data(), data.size(), 0), message);
            }
    };

    /// SpecialisedCamera represents a template-specialised CCam ATIS.
    template <typename HandleEvent, typename HandleException>
    class SpecialisedCamera : public Camera, public sepia::SpecialisedCamera<sepia::AtisEvent, HandleEvent, HandleException> {
        public:
            SpecialisedCamera<HandleEvent, HandleException>(
                HandleEvent handleEvent,
                HandleException handleException,
                std::unique_ptr<sepia::UnvalidatedParameter> unvalidatedParameter,
                std::size_t fifoSize,
                uint16_t serial,
                std::chrono::milliseconds sleepDuration
            ) :
                sepia::SpecialisedCamera<sepia::AtisEvent, HandleEvent, HandleException>(
                    std::forward<HandleEvent>(handleEvent),
                    std::forward<HandleException>(handleException),
                    fifoSize,
                    sleepDuration
                ),
                _parameter(defaultParameter()),
                _acquisitionRunning(true)
            {
                if (unvalidatedParameter->isString()) {
                    _parameter->load(unvalidatedParameter->jsonData());
                } else {
                    _parameter->load(unvalidatedParameter->parameter());
                }

                // initialize the context
                checkUsbError(libusb_init(&_context), "initialize the USB context");

                // find requested / available devices
                {
                    auto deviceFound = false;
                    libusb_device** devices;
                    const auto count = libusb_get_device_list(_context, &devices);
                    for (std::size_t index = 0; index < count; ++index) {
                        libusb_device_descriptor descriptor;
                        if (libusb_get_device_descriptor(devices[index], &descriptor) == 0) {
                            if (descriptor.idVendor == 1204 && descriptor.idProduct == 244) {
                                checkUsbError(libusb_open(devices[index], &_handle), "open the device");
                                if (libusb_claim_interface(_handle, 0) == 0) {
                                    if (serial == 0) {
                                        deviceFound = true;
                                        break;
                                    } else {
                                        auto data = std::array<uint8_t, 8>{};
                                        checkUsbError(libusb_control_transfer(_handle, 192, 85, 32, 0, data.data(), data.size(), 0), "send control packet");
                                        if ((serial & 0xff) == data[6] && ((serial & 0xff00) >> 8) == data[7]) {
                                            deviceFound = true;
                                            break;
                                        }
                                    }
                                    libusb_release_interface(_handle, 0);
                                }
                                libusb_close(_handle);
                            }
                        }
                    }
                    libusb_free_device_list(devices, 1);
                    if (!deviceFound) {
                        libusb_exit(_context);
                        if (serial == 0) {
                            throw std::runtime_error("there are no CCam ATIS connected or they are all busy");
                        } else {
                            throw std::runtime_error("the CCam ATIS with the given serial is not connected or it is busy");
                        }
                    }
                }

                // allocate a transfer
                _transfer = libusb_alloc_transfer(0);

                // send setup commands to the camera
                checkUsbError(libusb_reset_device(_handle), "reset the device");
                sendCommand(_handle, 0x01a, {0, 0, 0x00, 0x01}, "set the role");
                sendCommand(_handle, 0x41a, {0, 0, 0x00, 0x02}, "set the role");
                {
                    auto data = std::vector<uint8_t>{};
                    for (const auto& categoryPair : configuration()) {
                        for (const auto& settingPair : categoryPair.second) {
                            {
                                const auto value = (
                                    categoryPair.first == "static" ? settingPair.second.at("value")
                                    : static_cast<uint32_t>(_parameter->getNumber({categoryPair.first, settingPair.first}))
                                );
                                data.push_back(static_cast<uint8_t>(value >> 24));
                                data.push_back(static_cast<uint8_t>(value >> 16));
                                data.push_back(static_cast<uint8_t>(value >> 8));
                                data.push_back(static_cast<uint8_t>(value & 0xff));
                            }
                            {
                                const auto tension = settingPair.second.at("tension");
                                data.push_back(static_cast<uint8_t>(tension >> 24));
                                data.push_back(static_cast<uint8_t>(tension >> 16));
                                data.push_back(static_cast<uint8_t>(tension >> 8));
                                data.push_back(static_cast<uint8_t>(tension & 0xff));
                            }
                            {
                                const auto address = settingPair.second.at("address");
                                data.push_back(static_cast<uint8_t>(address >> 24));
                                data.push_back(static_cast<uint8_t>(address >> 16));
                                data.push_back(static_cast<uint8_t>(address >> 8));
                                data.push_back(static_cast<uint8_t>(address & 0xff));
                            }
                        }
                    }
                    checkUsbError(libusb_control_transfer(_handle, 64, 97, 0, 0, data.data(), data.size(), 0), "load the biases");
                    checkUsbError(libusb_control_transfer(_handle, 64, 98, 0, 0, data.data(), data.size(), 0), "load the biases");
                }
                sendCommand(_handle, 0x00a, {0, 0, 0x00, 0x40}, "flush the biases");
                sendCommand(_handle, 0x40a, {0, 0, 0x00, 0x40}, "flush the biases");
                sendCommand(_handle, 0x008, {0, 0, 0x03, 0x2c}, "set the mode");
                sendCommand(_handle, 0x408, {0, 0, 0x03, 0x2c}, "set the mode");
                {
                    auto data = std::array<uint8_t, 1024>{};
                    int32_t transferred;
                    libusb_bulk_transfer(_handle, 129, data.data(), data.size(), &transferred, 100);
                }
                sendCommand(_handle, 0x000, {0, 0, 0x0c, 0x81}, "start reading");
                sendCommand(_handle, 0x400, {0, 0, 0x0c, 0x81}, "start reading");

                // start the reading loop
                _acquisitionLoop = std::thread([this, serial]() -> void {
                    try {
                        auto data = std::vector<uint8_t>(1 << 17);
                        sepia::AtisEvent event;
                        uint64_t timestampOffset;
                        while (_acquisitionRunning.load(std::memory_order_relaxed)) {
                            int32_t transferred = 0;
                            const auto error = libusb_bulk_transfer(
                                _handle,
                                129,
                                data.data(),
                                static_cast<uint32_t>(data.size()),
                                &transferred,
                                static_cast<uint32_t>(this->_sleepDuration.count())
                            );
                            if ((error == 0 || error == LIBUSB_ERROR_TIMEOUT) && transferred % 4 == 0) {
                                for (auto byteIterator = data.begin(); byteIterator != std::next(data.begin(), transferred); std::advance(byteIterator, 4)) {
                                    if (*std::next(byteIterator, 3) == 0x80) {
                                        timestampOffset = (
                                            static_cast<uint64_t>(*byteIterator)
                                            | (static_cast<uint64_t>(*std::next(byteIterator, 1)) << 8)
                                            | (static_cast<uint64_t>(*std::next(byteIterator, 2)) << 16)
                                        ) * 0x800;
                                    } else {
                                        event.x = (static_cast<uint16_t>(*std::next(byteIterator, 2) & 0x1) << 8) + *std::next(byteIterator, 1);
                                        event.y = 239 - *byteIterator;
                                        event.timestamp = timestampOffset + (
                                            (static_cast<uint64_t>((*std::next(byteIterator, 3) & 0xf)) << 7)
                                            | (*std::next(byteIterator, 2) >> 1)
                                        );
                                        event.polarity = ((*std::next(byteIterator, 3) & 0b10000) >> 4) == 1;
                                        event.isThresholdCrossing = ((*std::next(byteIterator, 3) & 0b100000) >> 5) == 1;
                                        if (!this->push(event)) {
                                            throw std::runtime_error("Computer's FIFO overflow");
                                        }
                                    }
                                }
                            } else {
                                throw sepia::DeviceDisconnected("CCam ATIS");
                            }
                        }
                    } catch (...) {
                        this->_handleException(std::current_exception());
                    }
                });
            }
            SpecialisedCamera(const SpecialisedCamera&) = delete;
            SpecialisedCamera(SpecialisedCamera&&) = default;
            SpecialisedCamera& operator=(const SpecialisedCamera&) = delete;
            SpecialisedCamera& operator=(SpecialisedCamera&&) = default;
            virtual ~SpecialisedCamera() {
                _acquisitionRunning.store(false, std::memory_order_relaxed);
                _acquisitionLoop.join();
                libusb_release_interface(_handle, 0);
                libusb_free_transfer(_transfer);
                libusb_close(_handle);
                libusb_exit(_context);
            }
            virtual void trigger() override {
                // @DEV trigger the camera
            }

        protected:
            std::unique_ptr<sepia::Parameter> _parameter;
            std::atomic_bool _acquisitionRunning;
            libusb_context* _context;
            libusb_device_handle* _handle;
            libusb_transfer* _transfer;
            std::thread _acquisitionLoop;
    };

    /// make_camera creates a camera from functors.
    template <typename HandleEvent, typename HandleException>
    std::unique_ptr<SpecialisedCamera<HandleEvent, HandleException>> make_camera(
        HandleEvent handleEvent,
        HandleException handleException,
        std::unique_ptr<sepia::UnvalidatedParameter> unvalidatedParameter = sepia::make_unique<sepia::UnvalidatedParameter>(std::string()),
        std::size_t fifoSize = 1 << 24,
        uint16_t serial = 0,
        std::chrono::milliseconds sleepDuration = std::chrono::milliseconds(10)
    ) {
        return sepia::make_unique<SpecialisedCamera<HandleEvent, HandleException>>(
            std::forward<HandleEvent>(handleEvent),
            std::forward<HandleException>(handleException),
            std::move(unvalidatedParameter),
            fifoSize,
            serial,
            sleepDuration
        );
    }
}
