If you want to use the Vitis project, you first need to update the platform.
Open the Vitis project -> Then download the .xsa file -> Right click on the "hdmi_workspace" platform and select "Update Hardware Specifications" -> Now select the .xsa file you just downloaded.
Now you can build the hdmi_workspace platform and build the snake game. Once the builds are done, you can run the program on your hardware. Plug a HDMI cable into HDMI TX and connect it to a monitor. The game can be played with btn0 and btn3.

The project currently supports the hdmi_output on the board and the buttons. If you want to add more, you need to download the Vivado project and add these IPs.
