

## 项目说明

&emsp;&emsp;

| 项目                                   | 说明 | 环境              | 其他依赖 |
|----------------------------------------|------|-------------------|----------|
| [ScreenShot](#jump_ScreenShot)         | 截屏 | Qt5.15.2_msvc2019 | 无       |
| [DynamicDesktop](#jump_DynamicDesktop) | 截屏 | Qt5.15.2_msvc2019 | 无       |

+ 命令行工具

  src/script/encoding中包含2个exe，提供utf8和utf8互相转换的功能。  
  参数格式为 ```.\*.exe param```，param可以为普通文件，也可以是文件夹。  
  如果是文件夹，则遍历文件夹及其子目录中的```cpp```、```h```、```hpp```、文件，对符合条件的文件进行格式转换。


## <span id="jump_ScreenShot">截屏</span>

### 动态效果图
  
<center><img src=./doc/image/screen_shot.gif width=80% /></center>

### 功能

&emsp;&emsp;一个功能简单的截屏软件，提供如下功能：

- 支持多屏、高分屏幕；
  
- 支持绘制矩形、绘制文字以及绘制箭头的功能；
  
- 支持撤销操作；
  
- 随时调整截屏区域的位置以及大小；
  
  拖拽选择区域后，可以整体拖拽移动，也可以拖动边框调整大小；

- 提供保存到本地以及剪切板的功能；


## <span id="jump_DynamicDesktop">动态桌面</span>

<center><img src=./doc/image/dynamic_desktop.gif width=80% /></center>

### 功能

### 动态效果图

&emsp;&emsp;一个嵌入windos系统中的动态桌面，提供如下功能：

- 保持比不变的显示相册，边缘空白地方进行模糊填充；

- 图片右上方有一个旋转立方体（QOpenGLWidget），其和本地图片同步变化；

- 下方有一个相册，轮播相册(QPropertyAnimation + QGraphicsObject)；
  
- 支持多屏；
  
- 支持快捷键；
  - 在焦点在任何地方，```Ctrl``` + ```Esc```，程序退出
  - 焦点在桌面，```Ctrl``` + ```->```，加速切换相册