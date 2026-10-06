``` text
V4L2 Webcam Capture

A Linux webcam capture application written in C using the Video4Linux2 (V4L2) API.

The project demonstrates how to communicate with a webcam through the Linux V4L2 subsystem, enumerate supported pixel formats and resolutions, configure the capture format, manage video buffers using MMAP and USERPTR, capture frames, and save them to files.

Features
V4L2 webcam device access
Device capability detection
Pixel format enumeration
Resolution enumeration
User-selectable pixel format and resolution
V4L2 format configuration
MMAP buffer support
USERPTR buffer support
Video buffer queue/dequeue handling
Video stream start/stop
Captured frame metadata display
MJPEG and YUYV capture
Captured frame saving

Prerequisites:
 Linux Operating System
 Compiler and Build tools
 v4l2 Headers
 Webcam 

Project Structure
v4l2_code/
│
├── main.c
│
├── v4l2_common.c
├── v4l2_common.h
│
├── querycap.c
├── querycap.h
├── enum_fmt.c
├── enum_fmt.h
├── enum_framesizes.c
├── enum_framesizes.h
├── s_fmt.c
├── s_fmt.h
│
├── reqbufs.c
├── reqbufs.h
├── querybuf.c
├── querybuf.h
├── qbuf.c
├── qbuf.h
├── dqbuf.c
├── dqbuf.h
│
├── streamon.c
├── streamon.h
├── streamoff.c
├── streamoff.h
│
├── memory.c
├── memory.h
├── frame.c
├── frame.h
│
├── Makefile
├── .gitignore
└── README.md

The project is divided into modules so that each V4L2 operation has a clear responsibility.

V4L2 Implementation:

The application follows the standard V4L2 streaming capture workflow:

Open Device
 
VIDIOC_QUERYCAP
     ↓
VIDIOC_ENUM_FMT
     ↓
Select Pixel Format
     ↓
VIDIOC_ENUM_FRAMESIZES
     ↓
Select Resolution
     ↓
VIDIOC_S_FMT
     ↓
VIDIOC_REQBUFS
     ↓
Setup MMAP / USERPTR Buffers
     ↓
VIDIOC_QBUF
     ↓
VIDIOC_STREAMON
     ↓
VIDIOC_DQBUF
     ↓
Process / Save Frame
     ↓
VIDIOC_QBUF
     ↓
Repeat
     ↓
VIDIOC_STREAMOFF
     ↓
Cleanup
     ↓
close()

Capability Detection

VIDIOC_QUERYCAP is used to obtain information about the webcam and verify that it supports:

V4L2_CAP_VIDEO_CAPTURE
V4L2_CAP_STREAMING
Pixel Format Enumeration

VIDIOC_ENUM_FMT is used to enumerate the formats supported by the webcam.

Common formats include:

MJPG
YUYV

The application displays the available formats and allows the user to select one.

Resolution Enumeration

After selecting a pixel format, VIDIOC_ENUM_FRAMESIZES is used to determine the resolutions supported for that format.

For example:

640x480
1280x720
1920x1080
Format Configuration

The selected format and resolution are passed to the driver using:

VIDIOC_S_FMT

The driver may modify the requested values and return the format it actually accepted.

Buffer Management

The application supports two V4L2 memory methods.

MMAP

With MMAP, the V4L2 driver provides the capture buffers and the application maps them into its address space.

The basic flow is:

VIDIOC_REQBUFS
      ↓
VIDIOC_QUERYBUF
      ↓
mmap()
      ↓
VIDIOC_QBUF

The driver provides the buffer offset and size through VIDIOC_QUERYBUF.

The application then uses mmap() to obtain a virtual address for the buffer.

USERPTR

With USERPTR, the application allocates the memory itself and provides the buffer address to the V4L2 driver.

The flow is:

VIDIOC_REQBUFS
      ↓
Allocate Memory
      ↓
Set m.userptr
      ↓
VIDIOC_QBUF

The project uses posix_memalign() for USERPTR buffer allocation.

Frame Capture

After the buffers are queued, the application starts streaming using:

VIDIOC_STREAMON

Captured frames are retrieved using:

VIDIOC_DQBUF

The driver returns information such as:

buf.index
buf.bytesused
buf.timestamp
buf.sequence
buf.flags

The buffer index identifies which application buffer contains the completed frame:

frame_data = buffers[buf.index].start;

The valid size of the captured frame is provided by:

buf.bytesused

For MJPEG capture, the frame size can change from frame to frame because JPEG compression produces variable-sized data.

After processing the frame, the buffer is returned to the driver using:

VIDIOC_QBUF

This creates the continuous capture cycle:

QBUF
  ↓
Driver captures frame
  ↓
DQBUF
  ↓
Application processes frame
  ↓
QBUF
  ↓
Repeat



Running

Run the application:

./v4l2_capture

The application will ask the user to select:

Pixel format
Resolution
Number of frames
Memory method

For example:

SUPPORTED PIXEL FORMATS

1. Motion-JPEG [MJPG]
2. YUYV [YUYV]

Enter pixel format number: 1

SUPPORTED RESOLUTIONS FOR MJPG

1. 640x480
2. 1280x720

Enter resolution number: 2

Enter number of frames to capture: 10

CHOOSE MEMORY METHOD

1. MMAP
2. USERPTR

Enter choice: 1

Captured MJPEG frames are saved as:

frame_001.jpg
frame_002.jpg
frame_003.jpg

YUYV frames are saved as raw:

frame_001.yuyv
frame_002.yuyv


Author

Lakshman M
```
