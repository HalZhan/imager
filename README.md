# imager
One image processing tool.

# dependencies
- node-gyp
- visual studio
- python 2.7.x (not 3.x)

# todo
- 进入src目录下的GrayScale与ScreenCapture文件夹，分别运行build&generate.bat

# api
- Function:
    - grayScale([readPath], [savePath]) 灰度化
    - captureScreen([savePath]) 截屏

# 使用方法
(test目录下)
```
const imager = require('../src/imager');    // 引入imager模块

const readPath = 'snail.bmp';   // 欲处理的图片

imager.grayScale(readPath);     // 灰度化
imager.captureScreen();         // 截屏

```