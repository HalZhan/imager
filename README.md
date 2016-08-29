# imager
- One image processing tool for NodeJS.
- NodeJS图像处理工具

# dependencies
- node-gyp
- visual studio
- python 2.7.x (not 3.x)

# todo
- src目录下，分别进入BinaryScale,GrayScale,Negative,ScreenCapture执行build&generate.bat

# api
- Function:
    - binaryScale([readPath], [writePath]) 二值化
    - grayScale([readPath], [writePath]) 灰度化
    - negative([readPath], [writePath]) 负片
    - captureScreen([writePath]) 截屏

# 使用方法
(test目录下)
```
const imager = require('../src/imager');    // 引入imager模块

const readPath = '../resources/snail.bmp';   // 欲处理的图片

imager.binaryScale(readPath);   // 二值化
imager.grayScale(readPath);     // 灰度化
imager.negative(readPath);      // 负片
imager.captureScreen();         // 截屏

```