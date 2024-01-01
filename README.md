# ★Tracbom 音·迹  4键下落式音游项目

<img width="140" alt="game - Copy" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/650a7137-f708-4483-b82b-d90de6b9e824">

**基于 `©COCOS-2D X` 游戏引擎开发**

------

**同济大学国豪书院-软件工程专业程序设计范式设计项目 By 吴忱 王天阔**

**感谢 bilibili@神渣冬小狼 bilibili@星鹿ELEC 对本项目的帮助与支持**

2023.12.29提交 开发时长约2周 采用MIT许可 项目截图见最下方

开发人员共2人：吴忱设计了游戏的核心游玩功能(贡献50-55%)，王天阔设计了游戏的美工、场景切换、参数调整和选歌等外围功能(贡献45-50%)。

-------

+ **最近更新**

最近尚无更新。

------

+ **下载及启动方法**

方法一：
如果您只是想游玩，可以直接拷贝**tracbom_release**,这是直接可以游玩的版本，双击文件夹下的**tracbom.exe**即可游玩。如果遇到无声、画面异常等问题，请**使用方法二获取完整项目并编译运行**。

方法二：
如果您想要完整的项目：
该GitHub存储库所保存的是**Visual Studio 2022**项目。您可以使用VS克隆此存储库，或者直接下载库对应的zip后缀压缩包。无论如何，请您确保库文件保存在一个不含有**中文**等字符的路径下(最好是**纯英文字符和数字的组合**)。如果您选择**直接使用VS2022打开**，请在启动项目中选择**tracbom**项目，然后开始编译和执行。如果有提示工具集问题，请使用Visual Studio推荐的设置。如果您下载的是**zip压缩包**，请解压该压缩包，进入**tracbom\proj.win32**路径下，用VS装载**tracbom.sln**解决方案即可，之后的步骤与前一种方案相同。

**需要注意，我们修改过tracbom\cocos2d\cocos\audio\win32目录下的MciPlayer.cpp，MciPlayer.h和SimpleAudioEngine.cpp，请不要只复制Resources和classes以创建项目，以免引起音频引擎异常！**

当您已经编译过一次项目后，您就可以使用proj.win32目录下的Debug.win32或Release.win32中的可执行exe文件启动游戏，而无需使用VS。


如果您是第一次接触tracbom，强烈推荐您在启动游戏后进入选歌页面时，按下**右上角的"?"按钮**了解玩法。

建议您每次游玩前，都根据设备情况，将延迟调整为合适的水平，以优化您的游玩体验。

------

+ **项目简介**

该项目是软件工程专业 **程序设计范式** 专业课的期末设计项目之一。tracbom项目是一种经典的4键式音乐节奏游戏，您可以使用键盘上的4个按键游玩。在游玩之前，您可以设置延迟、背景音量、击键音量、打击键位等选项。进入游戏后，您可以按住左侧的选歌列表并上下滑动来滚动列表，点击选择您要游玩的曲目。在右下方可以选择三种不同的难度，并按PLAY按钮进入游玩。游戏的按键应当与音符相配合，音符分为短击和长按两种，当短击键与轨道最下端重合时，迅速按下对应按键；当长按键始端与轨道最下端重合时，迅速按下对应按键并保持，直到长按键末端离开轨道。游戏根据您的完成度和准确率、连击率等方面有0-1000000的评分和M/V/S/A/B/C/F七种不同的评级，并可以保存您的历史记录。

需要注意的是，在您击键的过程中，轨道也可能有消失、出现、移动、旋转、缩放等行为，这通常是与节奏配合的。同时音符的组合也可能较难掌握，您需要多加练习。
按下EXIT按钮可以退出游戏，右上角的问号按钮是游戏指导，头像按钮是作者信息，齿轮按钮是游戏设置。

项目的核心是**竞技性、灵活性、扩展性和现代性**。我们希望制作一款游戏，它的玩法非常简单，不需要经过繁难的学习，但**想要玩好却非常不容易**，这是一种考验**技术力**的游戏。另外，我们希望这款游戏可以适应每一个人，每人可以**根据喜好调整键位和反馈等设置**，以达到最佳游戏体验。之后，我们希望它是可扩展的，用户可以**根据喜好添加或删除歌曲的谱面，同时又可以创作属于自己的谱面**，打造一款**个性化**的游戏。最后，我们希望它的界面和设计等是现代化的，符合**新时代游戏设计审美**。

此外，在transtools目录下，我们提供了**将已有的malody谱面和osu谱面转换为tracbom谱面的转换器**，以便于添加谱面。具体的使用方法，您可以联系我们了解。

------

+ **项目功能介绍**

选项设置：背景音量、音效音量、按键延迟、键位调整。

游玩功能：短击键、长按键、偏移判定、暂停和重玩、轨道的消失、出现、移动、旋转、缩放等。

评级功能：连击计数、Perfect、Good、Bad、Miss击键判定、等级评定。

记录存储：存储游戏属性、高分记录等。

谱面管理：添加谱面、malody和osu的已有谱面转换。

------

+ **谱面管理**

tracbom的谱面是高度可扩展的。在tracbom/Resources路径下描述了增删歌曲的方法。
如果您没有现成的谱面，那么自创谱面是十分困难的。项目根目录下简单描述了添加音符和轨道的方法供您查阅。

------

+ **后记**

此项目难度适中，体量较小，功能较为简单，不喜请勿喷。

该项目目前还处于实验阶段，有很多功能可能不稳定，敬请谅解。如果有改进，我们将在该README文件中添加改版说明，谢谢您的关注。

------

+ **项目截图**

选择歌曲

<img width="825" alt="Untitled" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/ab645052-d497-48f9-81a5-2cf6478898cc">

游戏页面

<img width="825" alt="Untitled1" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/b0704396-06cd-4b40-a95d-120d4e44488c">

属性设置

<img width="825" alt="Untitled2" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/db8595ad-6711-4221-b8de-672ccafc33ce">

延迟调整

<img width="825" alt="Untitled3" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/5ff13e0b-9310-473a-9008-46d744884aec">

分数结算

<img width="825" alt="Untitled4" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/9d42154a-397c-4490-9336-2d9eda33d6eb">

项目介绍

<img width="825" alt="Untitled5" src="https://github.com/Wangtk311/tracbom-project/assets/115420227/cbec0999-2549-4e84-a938-5e321710c767">

------

*适度游戏益脑，沉迷游戏伤身。*
