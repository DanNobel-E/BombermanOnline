# BombermanOnline

Bomberman Online Demo project for UE 4.27.

## Bomberman World Generator

Right click on Content Browser or inherit from BBMWorldPreset to instantiate you Bomberman World Generator.

![Bomberman - Unreal Editor 11_06_2022 10_39_39](https://user-images.githubusercontent.com/60443583/173185198-7a4dae45-d47f-40b7-b84b-2744590aec6d.png)


> World Preset Editor details tab exposes the following parameters:


| Parameter     | Description   |
| ------------- | ------------- |
|**Color Tolerance**| Texture *color parser* accuracy.|
| **World Texture** | A *2D Texture* which describes the world to create. Texture Size is converted to unreal unit meters.|
| **PresetColorPattern** | A *String/Color Map*. World Texture bytes array is parsed according to the specified colors.|
| **PresetActors** | A *String/Actor Map*. Strings must match ColorPresetMap ones. Actors are spawned on PresetColorPattern colors map relative positions.|
| **Light** | World Sky Sphere and directional light parameters. |
| **Character** | The *Pawn* to spawn for players.|

![Bomberman - Unreal Editor 21_06_2022 11_31_062](https://user-images.githubusercontent.com/60443583/174768216-95272d79-71fb-4f72-9c14-a1c4f600d112.png)

Fill the *Details* tab, then select an action by clicking on the matching button:
* **Preview World**: preview yuor Bomberman World looks inside the *Editor Viewport*.
* **Generate World**: preview and generate your Bomberman World as an *Unreal UWorld*, accessible through Content Browser.

![Bomberman - Unreal Editor 21_06_2022 11_31_06](https://user-images.githubusercontent.com/60443583/174772443-df80adff-667a-464d-83bc-f25970dbcef9.png)

> **You can also select your *BBM World Preset* assets on Content Browser and right click to show *Asset Menu*. Then click on *Create BBM World* entry to generate all Worlds at once.**


![Bomberman - Unreal Editor 18_05_2022 00_04_54](https://user-images.githubusercontent.com/60443583/173185864-bf5dd74e-5bf7-4bcf-ae9a-127fa7ae739d.png)

> **Search for *BombermanEditorPluginContent/Textures* directory to find some example texture to try, or add more on your own.**

![Captures 18_05_2022 00_14_24](https://user-images.githubusercontent.com/60443583/173186420-4362d279-56e1-4d28-937c-dbbb877ba447.png)


https://user-images.githubusercontent.com/60443583/173186519-d7b21873-bd5a-4724-91d4-2b47afdfc971.mp4



## Bomberman Online Demo

A simple Bomberman inspired game demo with Unreal online implementation.

Main features:

* Four Players online multiplayer match.
* Player movements and animations implemented via Unreal Blueprints (meshes and animation clips imported from [mixamo](https://www.mixamo.com/)).
* Players can shoot bombs with a max range of two bombs at the same time for each. 
* Bombs fx inspired by original Bomberman game.
* Bombs can destroy breakable walls.

> *BombermanEditorPluginContent* directory contains template blueprint classes for Player, Bomb, Breakable Wall and more.


https://user-images.githubusercontent.com/60443583/173187072-9101acde-0ba1-4785-a33c-081033d6ef7f.mp4


https://user-images.githubusercontent.com/60443583/173187074-160024ae-8053-4705-a191-77d618cc8ce0.mp4


https://user-images.githubusercontent.com/60443583/173187129-0d8515b1-9d8f-4815-9a15-54115c5f2252.mp4






