# Tracbom project 音·迹 4键下落式音游开发项目
**同济大学国豪书院·软件工程专业期末设计项目 By 吴忱 王天阔**

2023.12.29提交 开发时长约2.5周

**下载及启动方法**

该GitHub存储库所保存的是**Visual Studio 2022**项目。您可以下克隆此存储库，或者下载库对应的zip后缀压缩包。无论如何，请您确保库文件保存在一个不含有**中文**等字符的路径下(最好是**纯英文字符和数字的组合**)。如果您选择**直接使用VS2022打开**，请在启动项目中选择**tracbom**项目，然后开始编译和执行。如果有提示工具集问题，请使用Visual Studio推荐的设置。如果您下载的是**zip压缩包**，请解压该压缩包，进入**tracbom\proj.win32**路径下，用VS装载**tracbom.sln**解决方案即可，之后的步骤与前一种方案相同。

如果您是第一次接触tracbom，强烈推荐您在启动游戏后进入选歌页面时，按下**右上角的"?"按钮**了解玩法。

**项目简介**

该项目是软件工程专业 程序设计范式 专业课的期末设计项目之一。tracbom项目是一种经典的4键式音乐节奏游戏，您可以使用键盘上的4个按键游玩。在游玩之前，您可以设置延迟、背景音量、击键音量、打击键位等选项。进入游戏后，您可以按住左侧的选歌列表并上下滑动来滚动列表，点击选择您要游玩的曲目。在右下方可以选择三种不同的难度，并按PLAY按钮进入游玩。游戏的按键应当与音符相配合，音符分为短击和长按两种，当短击键与轨道最下端重合时，迅速按下对应按键；当长按键始端与轨道最下端重合时，迅速按下对应按键并保持，直到长按键末端离开轨道。游戏根据您的完成度和准确率、连击率等方面有M/V/S/A/B/C/F七种不同的评级，并可以保存您的历史记录。

需要注意的是，在您击键的过程中，轨道也可能有消失、出现、移动、旋转、缩放等行为，这通常是与节奏配合的。同时音符的组合也可能较难掌握，您需要多加练习。
按下EXIT按钮可以退出游戏，右上角的问号按钮是游戏指导，头像按钮是作者信息，齿轮按钮是游戏设置。

此外，在transtools目录下，我们提供了将已有的malody谱面和osu谱面转换为tracbom谱面的转换器，以便于添加谱面。具体的使用方法，您可以联系我们了解。

**项目功能介绍**
选项设置：背景音量、音效音量、按键延迟、键位调整。
游玩功能：短击键、长按键、偏移判定、暂停和重玩、轨道的消失、出现、移动、旋转、缩放等。
评级功能：连击计数、Perfect、Good、Bad、Miss击键判定、等级评定。
记录存储：存储游戏属性、高分记录等。
谱面管理：添加谱面、malody和osu的已有谱面转换。

**谱面管理**
tracbom的谱面是高度可扩展的。在tracbom/Resources路径下描述了增删歌曲的方法。
