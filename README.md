# LuminaPrognosisPlayer
某寝室用

## Runtime requirement
- Windows OS later than Windows 10
- MSVC 2015-2022 (x64)

## How to play
First, prepare the files, commented below, with proper directory.

<pre>
...
├ Scenario_csv
│  ├ ...
│  ├ Hscene // Scenerio folder
│  │  ├ ...
│  │  ├ 3512005_h1.json // Select file like this from the application
│  │  ├ 3512005_h2.json
│  │  └ ...
│  └ ...
├ Sound
│  ├ ...
│  └ Voice // voice folder
│     ├ ...
│     ├ 3512005
│     │  ├ ...
│     │  ├ 3512005_h_001.mp3
│     │  ├ ...
│     │  ├ 3512005_H2_001.mp3
│     │  └ ...
│     └ ...
├ ...
└ Texture // image folder
   ├ ...
   ├ HCG
   │  ├ Animation
   │  │  ├ ...
   │  │  ├ 3512005
   │  │  │  ├ Fast
   │  │  │  │  ├ Fast_00.jpg
   │  │  │  │  └ ...
   │  │  │  ├ Finish
   │  │  │  │  ├ Finish_00.jpg
   │  │  │  │  └ ...
   │  │  │  └ Normal
   │  │  │     ├ Normal_00.jpg
   │  │  │     └ ...
   │  │  └ ...
   │  ├ ...
   │  └ Still
   │     ├ ...
   │     ├ 3512005_1
   │     │  ├ 3512005_1.jpg
   │     │  └ ...
   │     ├ 3512005_2
   │     │  ├ 3512005_1.jpg
   │     │  └ ...
   │     └ ...
   └ ...
</pre>

Then, select one of the json files under `Scenario_csv/Hscene` from application menu `File->Open`.  
The scene will be set up assuming that relevant voice and image files exist in the relative paths above.  

## Menu functions
| Entry | Item | Action |
| --- | --- | --- |
| File | Open | Open file-select-dialogue.
| Audio | Loop | Set/reset audio loop.
| - | Setting | Open a dialogue to set audio volume and rate.
| Image | Pause | Pause/resume animation.

## Mouse functions
| Input | Action | Note |
| --- | --- | --- |
| Mouse wheel | Scale up/down | |
| Left button + mouse wheel | Speed up/down the animation | There is no function for still. |
| Left button click | Switch to the next still/animation. | |
| Left button drag | Move view-point | This works only when the image is scaled beyond the monitor resolution. |
| Middle button | Reset scaling, animation speed, and view-point. | |
| Right button + mouse wheel | Show the next/previous text. | |
| Right button + middle button |Hide/show window's frame. | Hiding frame lets window go to the origin of the primary monitor. |
| Right button + left button | Move window. | This works only when the window's frame is hidden. |

## Keyboard functions
| Input | Action |
| --- | --- |
| C | Switch text colour between black and white. |
| T | Hide/show the text. |
| Esc | Close the application. |
| Up | Move on to the next folder. |
| Down | Move on to the previous folder. |

- The text will be rendered with `游明朝` font installed under Windows system folder.

## External libraries
- [JSON for Modern C++ v3.11.3](https://github.com/nlohmann/json/releases/tag/v3.11.3)

## Build
1. Run `deps/CMakeLists.txt` to obtain external library.
2. Open `LuminaPrognosisPlayer.sln` with Visual Studio.
3. Select `Build Solution` on menu item.

The `deps` folder will be as follows:
<pre>
LuminaPrognosisPlayer
├ deps
│  ├ nlohmann // JSON for Modern C++
│  │   └ json.hpp
│  └ CMakeLists.txt
└ ...
</pre>

## Note

- Mind that if the scene has animation part, huge memory consumption will be caused by loading all the animation images.
