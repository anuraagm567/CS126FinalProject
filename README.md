# Genshin Impact Wish Simulator
## Description
This project emulates the wishing system of the popular video game *Genshin Impact*. The project includes a console
version with basic wishing features, and a more extensive Cinder simulation with an inventory system, a system for
saving and resetting wish progress, and an image display for the wish banner and items received.

![Banner Image](https://gamewith-en.akamaized.net/article/thumbnail/rectangle/23861.png)


[Additional information regarding Genshin Impact's wishing system](https://genshin-impact.fandom.com/wiki/Wishes)

## Dependencies
* [cmake](https://cmake.org/)
* [libcinder](https://github.com/cinder/Cinder)
* [nlohmannjson](https://github.com/nlohmann/json)

## Installation Guide
1. [Install Cinder for Windows/macOS](https://courses.grainger.illinois.edu/cs126/sp2021/notes/cinder-installation-notes/)
2. Create a "my-projects" folder in the downloaded Cinder folder and clone this repository inside
3. Update working directory  in Run -> Edit Configurations if necessary to deal with file loading (for CLion users)

## Running the Simulator
To run the console version of the simulator, use the run configuration titled "console-simulator".
To run the Cinder wish simulator, use the run configuration titled "wish-simulator". For CLion users, the run
configuration can be changed using the drop down menu to the right of the green build icon on the toolbar.

## Controls for Cinder Simulator
| Key | Action |
| --- | --- |
|Left mouse click on button | Select corresponding button |
|Left mouse click during wish display | Display next wish or return to banner screen when on last wish |

## Image Sources
* [Characters](https://genshin-impact.fandom.com/wiki/Characters)
* [Weapons](https://www.gensh.in/database/weapon)