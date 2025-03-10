## ofxZED

[StereoLabs ZED](https://www.stereolabs.com/products/zed-2) wrapper for openFrameworks.

Includes support for:
* depth and color images.
* 3D point cloud.
* body tracking.
* object detection.

### Compatibility

Tested on OF 0.12.0 / Windows 11 / StereoLabs ZED 2i.

Should work on other platforms and using other StereoLabs devices.

Body tracking and object detection only work in `Release` mode! You'll need to create a `RelWithDebInfo` target if you need debug symbols.
