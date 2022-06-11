# BombermanOnline

Bomberman Online Demo project for UE 4.27.

## Bomberman World Generator

Right click on Content Browser or inherit from BBMWorldPreset to instantiate you Bomberman World Generator.

![Bomberman - Unreal Editor 11_06_2022 10_39_39](https://user-images.githubusercontent.com/60443583/173185198-7a4dae45-d47f-40b7-b84b-2744590aec6d.png)


> World Preset Editor details tab exposes the following parameters:


| Parameter     | Description   |
| ------------- | ------------- |
| **World Texture** | A *2D Texture* which describes the world to create. Texture Size is converted to unreal unit meters.|
| **PresetColorPattern** | A *String/Color Map*. World Texture bytes array is parsed according to the specified colors.|
| **PresetActors** | A *String/Actor Map*. Strings must match ColorPresetMap ones. Actors are spawned on PresetColorPattern colors map relative positions.|
| **Light** | World Sky Sphere and directional light parameters. |
| **Character** | The *Pawn* to spawn for players.|


> **Fill details tab, then click on Generate World button to create a your Bomberman World.**


![NewBBMWorldPreset 11_06_2022 13_17_54](https://user-images.githubusercontent.com/60443583/173185675-52bb3649-d496-4f1c-9622-39e6975a6caf.png)


> **You can also select your *BBM World Preset* assets on Content Browser and right click to show *Asset Menu*. Then click on *Create BBM World* entry to generate all Worlds at once.**


![Bomberman - Unreal Editor 18_05_2022 00_04_54](https://user-images.githubusercontent.com/60443583/173185864-bf5dd74e-5bf7-4bcf-ae9a-127fa7ae739d.png)

> **Search for *BombermanEditorPluginConte/Textures* directory to find some example texture to try, or add more on your own.**

![Captures 18_05_2022 00_14_24](https://user-images.githubusercontent.com/60443583/173186420-4362d279-56e1-4d28-937c-dbbb877ba447.png)


https://user-images.githubusercontent.com/60443583/173186519-d7b21873-bd5a-4724-91d4-2b47afdfc971.mp4



## Bomberman Online Demo


