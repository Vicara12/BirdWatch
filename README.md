# Bird Watcher

Bird Watcher is a program designed to display RC flight data both live and from a data file. It allows to generate a virtual flight deck environment that emulates a real airliner, thus enhancing the immersive experience of flying RC aircraft.

**This program comes with no guarantees, use it on your own discretion.**

## How to compile

Compiling is easy in Linux (I haven't developed a Windows or Mac version yet). Execute the following commands in order:

1. `sudo apt-get install make libgl1-mesa-dev libsdl2-dev libglew-dev libfreetype6-dev libavcodec-dev libavutil-dev libavformat-dev libswscale-dev libavdevice-dev` (these are the library names in the Debian repo, some might change with other distros).
2. `make`

The result is an executable of the program in the *bin* folder. If command 2 fails the most probable cause is that one or more of the libraries of step 1 were not found.

## How to use the program

All the configuration is made though a json file, whose path is given as the first argument. The parameters available for the config file will be shown below. There are a few config files in the *test* folder. If executed from the root directory of the program, the command to launch the program would look like this:

`./bin/BirdWatch ./test/test_config_file.json`

Where `./test/test_config_file.json` is to be replaced with the path to your config file. Executing the command as is shown above should result in a primary flight display being shown.

## The config file

Below is a list of all the available options in the config file:

* **window_size** (optional): sets the initial window width in pixels (must be an integer).
* **data_format**: this field specifies the format of the serial input. Possible values are "ascii" or "binary". More information on data input in its section.
* **data_fields**: this field is a list of the instruments that should be displayed on screen. Current valid values are "YPR" for yaw, pitch and roll data and "video" for video feed input.
* **print_data** (optional): if set to true the data received by the program will be echoed on screen. Default is false.
* **no_data_timeout** (optional): this parameter sets the amount of milliseconds that must pass without the program not receiving any data before the "NO DATA" warning is displayed on screen. Default is 250 ms.
* **data_separator** (optional): if the input data format is ascii, this indicates the data separator. Only one character must be provided (if various are given, only the first is taken). The default is space.
* **video_source**: if video is in data_fields this item sets the path to the video input file (more on video input below).
* **data_source**: this is a struct field that must contain the following subfields:
    * **type**: this field indicates wether the input comes from a file or from live serial data. Possible values are "file" and "serial".
    * In the case that the input comes from a file these other fields must be specified:
        * **file_path**: the path to the file with the input data.
    * If the case that the input comes form a serial port these other fields must be specified:
        * **port**: the name of the serial source to be used. If this value is unknown just leave it blank and an error message will show up with a list of the available ports (generally you are looking for a USB port).
        * **baud**: baud rate to be used in the serial communication.
        * **line_buffer_size** (optional): this parameter specifies the buffer size used to hold the incoming data. This should not be a problem unless you are trying to feed an enormous amount of data into de program. If the buffer size is too small an error will show on the terminal while the program is running. The default value is 2048.

## Data input

All the data the program needs (except for the video feed) should be provided through the serial input (for live data stream) or through a file (for offline data displaying).

In the case of the file input, the file must only contain the data to be displayed (no headers or field names), each data field separated by the indicated separator (see the JSON attribute *data_separator*) and each data packet followed by a newline character. The first field of each data packet must be a timestamp in milliseconds followed by all the data fields. Below is an example (which can be found at *test/out_ascii.txt*) that displays timestamp, yaw, pitch and roll data in ascii format.

```
6994 -0.39 -2.54 -0.92
7105 -0.32 -0.13 -1.01
7215 -0.76 2.94 -2.99
7325 -1.12 5.35 -8.09
7436 -2.13 6.89 -18.03
```

**WARNING:** binary format is not yet supported for file input.

For the live serial input both ascii and binary data can be used. The benefit of using binary is that the code is clearer, but it takes more bytes to transmit the data. On the other hand, binary is less intuitive to program but the data takes less space. If ascii input is being used the format is very similar to the file input, but in this case the data packet **must not** include a timestamp (as packets are displayed the moment they are received). For the binary input mode, the data should be transmitted through the serial channel but in this case what is transmitted is the raw bytes of data (this can be achieved with a cast from float to (char*)). It is assumed that each data item takes 4 bytes and that lines end with a carriage return followed by a newline.

The data to be transmitted is given by the contents of the *data_fields* list in the JSON ("video" is always ignored here, as this data is handled separately, not though the serial interface). For example, if the data fields is `["YPR", "video", "alt"]` then the packet must contain this data in this order: (timestamp?), yaw, pitch, roll, altitude.


## Video input

The program is capable of displaying both live video from a FPV camera and playing a video file in the hard drive (even though it will not be synchronized with the other data fields). In order to set up the video input just add "video" to the data_fields entry of the json file and then set the *video_source* field of the json to the input stream. The *video_source* field should be the video path or the `/dev/video` file corresponding to the live input source desired. If you don't know the video file of your FPV camera just leave it blank and a message like this will show up:

```
ERROR: could not open video source ""
Available camera sources are:
   * USB2.0 PC CAMERA: /dev/video2 /dev/video3
   * EasyCamera: EasyCamera: /dev/video0 /dev/video1
```

My computer has two available cameras: USB2.0 PC CAMERA (which is the FPV camera) and EasyCamera (which is my webcam). To the right of each camera various video files appear. One of these is the one that must be set as *video_source*. In my case it would be "/dev/vieo2" (usually it is the leftmost one).