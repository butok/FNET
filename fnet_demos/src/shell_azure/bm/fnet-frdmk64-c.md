---
platform: FNET
device: FRDM-K64F
language: c
---

Run a simple C sample on FRDM-K64F device running FNET
===
---

# Table of Contents

-   [Introduction](#Introduction)
-   [Step 1: Prerequisites](#Prerequisites)
-   [Step 2: Prepare your Device](#PrepareDevice)
-   [Step 3: Build and Run the Sample](#Build)
-   [Next Steps](#NextSteps)

<a name="Introduction"/>
# Introduction

**About this document**

This document describes how to connect FRDM-K64F device running FNET with Azure IoT SDK. This multi-step process includes:
-   Configuring Azure IoT Hub
-   Registering your IoT device
-   Build and deploy Azure IoT SDK on device

<a name="Prerequisites"></a>
# Step 1: Prerequisites

You should have the following items ready before beginning the process:

-   [Prepare IAR Embedded Workbench development environment][setup-iar]
-   [Download and install FNET][download-fnet]
-   [Setup your IoT hub][lnk-setup-iot-hub]
-   [Provision your device and get its credentials][lnk-manage-iot-hub]
-   [FRDM-K64F device] [FRDM-K64F-board]
-   [Tera Term Pro serial terminal][tera-term-pro]

<a name="PrepareDevice"></a>
# Step 2: Prepare your Device

-   [Setup FRDM-K64F device][FRDM-K64F-board-setup]
-   Connect the evaluation board to an Ethernet network, which must be connected to Internet.
-   Use USB cable to connect your PC to the board.

<a name="Build"></a>
# Step 3: Build SDK and Run the sample

-   Start a new instance of IAR Embedded Workbench IDE. Open the **azure-frdmk64f.ewp** project in the **fnet\fnet_demos\build\frdmk64f\azure\iar** folder in your home directory.

-   In IAR Embedded Workbench IDE, in **Workspace**, navigate to **azure-frdmk64f** project, open the **fapp_user_config.h** file.

-   Add the following code:

        #define FAPP_CFG_AZURE_CMD_DEVICE_CONNECTION_STRING "[device connection string]";

-   Replace the above placeholder with device connection string you obtained in [Step 1](#Prerequisites) and save the changes.

-   See [Manage IoT Hub][lnk-manage-iot-hub] to learn how to observe the messages IoT Hub receives from the application.

-   In **Workspace**, click **Download and Debug** to build and run the sample. The console displays messages as the application sends device-to-cloud messages to IoT Hub.

-   See [Manage IoT Hub][lnk-manage-iot-hub] to learn how to send cloud-to-device messages to the application.

-   See [FNET Azure IoT Hub Demo Quick Start][FNET-Azure-Quick-Start] for more information about the FNET Azure sample application.

<a name="NextSteps"></a>
# Next Steps

You have now learned how to run a sample application that sends simulated telemetry data to your IoT hub. To explore how to store, analyze and visualize the data from this application in Azure using a variety of different services, please click on the following lessons:

-   [Manage cloud device messaging with iothub-explorer]
-   [Save IoT Hub messages to Azure data storage]
-   [Use Power BI to visualize real-time sensor data from Azure IoT Hub]
-   [Use Azure Web Apps to visualize real-time sensor data from Azure IoT Hub]
-   [Weather forecast using the sensor data from your IoT hub in Azure Machine Learning]
-   [Remote monitoring and notifications with Logic Apps]   

[Manage cloud device messaging with iothub-explorer]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-explorer-cloud-device-messaging
[Save IoT Hub messages to Azure data storage]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-store-data-in-azure-table-storage
[Use Power BI to visualize real-time sensor data from Azure IoT Hub]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-live-data-visualization-in-power-bi
[Use Azure Web Apps to visualize real-time sensor data from Azure IoT Hub]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-live-data-visualization-in-web-apps
[Weather forecast using the sensor data from your IoT hub in Azure Machine Learning]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-weather-forecast-machine-learning
[Remote monitoring and notifications with Logic Apps]: https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-monitoring-notifications-with-azure-logic-apps
[setup-iar]: http://ftp.iar.se/WWWfiles/arm/webic/doc/EWARM_IDEGuide.ENU.pdf
[lnk-setup-iot-hub]: ../../setup_iothub.md
[lnk-manage-iot-hub]: ../../manage_iot_hub.md
[tera-term-pro]: http://ttssh2.sourceforge.jp
[FRDM-K64F-board]: https://www.nxp.com/products/processors-and-microcontrollers/arm-based-processors-and-mcus/kinetis-cortex-m-mcus/k-seriesperformancem4/k2x-usb/freedom-development-platform-for-kinetis-k64-k63-and-k24-mcus:FRDM-K64F
[FRDM-K64F-board-setup]: https://www.nxp.com/products/processors-and-microcontrollers/arm-based-processors-and-mcus/kinetis-cortex-m-mcus/k-seriesperformancem4/k2x-usb/freedom-development-platform-for-kinetis-k64-k63-and-k24-mcus:FRDM-K64F?tab=In-Depth_Tab
[download-fnet]: http://fnet.sf.net/
[FNET-Azure-Quick-Start]: http://fnet.sourceforge.net/manual/quick_start_azure.html
