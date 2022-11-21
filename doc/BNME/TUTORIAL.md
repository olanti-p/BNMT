# Bright Nights Mapgen Editor

## Setup

1. Run `cataclysm-bnme`
2. Create a new world, add `Map Editor` mod to it and other mods you plan to work with, give it a convenient name (e.g. `BNME`).
3. Create a new character in that world (default will do), then save and exit.
4. Create `run_editor.bat` (or `run_editor.sh`), and paste the following (replace `BNME` with the name of your world):
    ```
    cataclysm-bnme --editor --world BNME
    ```

5. On Windows, double click the `.bat` to run. On Linux, enable execution for script and then run it from terminal.
6. (Optional) You may have to skip some debug messages, depending on how broken your mods are.

If you need to work with multiple sets of mod, you can repeat steps 2-4 for each mod set, just have to rename the world and the `.bat`/`.sh` file accordingly.

The command in `.bat`/`.sh` is just a shortcut to bring you straight to the editor, you can launch it manually as well:
1. Run `cataclysm-bnme`
2. Load the world (e.g. `BNME`)
3. Open debug menu
4. `[m] Map` -> `[A] Advanced Map editor`

WARNING: running an editor **will** make the world unplayable for regular game. Make sure you don't run it in same world as your favorite 2-years-old-rolling-around-in-death-fortress-alpha-cyborg-mage survivor.

## Usage (short tutorial)

Click `New Project`. You'll see multiple windows:

1. `Toolbar` - currently selected tool, hover with mouse to see details.
2. `File history` - history of edits. Hover with mouse over grey `(?)` to see details.
3. `Advanced Map Editor` - project controls. Can be used to save/export/close project, move/zoom camera, toggle other windows.
4. `Project Overview` - project overview. Each project is a combination of multiple mapgens grouped together and optionally dependent on same palettes.

Each window can be collapsed, closed, dragged around and resized with LMB (Left Mouse Button).

Click `New mapgen` and then click on the new mapgen name in the list.

Now you'll see:

1. `File Info` window. Contains generic settings for the selected mapgen. 
2. Visual representation of the mapgen underneath all windows. It's empty right now.

You can pan and zoom the view with RMB and Mouse Wheel, see `(?: Camera controls)` hint in `Advanced Map Editor` window.

In `File Info`, choose `Oter` mapgen type and `Rows` mapgen base.

Click on `om_terrain` to assign the mapgen to some overmap terrain (you can type to search).
Let's use `animalshelter`.
Set `weight` to something huge, like `10000`, to make it easier to debug, and `fill_ter` to something mundane, like `t_region_grass`.

In `Project Overview`, click `inline palette` button to the ledt of mapgen name. You can hover with cursor to see what each button does.

A new window opens up, `Palette`. This is the inline palette used by the mapgen.
You can add new entries by pressing `+` at the bottom, and then assigning data to them by pressing a triangle on the right.

WARNING: Not all data types are completely implemented right now. 

For now, let's make some walls. Add a new entry, change the symbol from `a` to `|`, then press triangle on the right.
A new `Mappings` window opens up, which contains data associated with the symbol.

Add an `AltTerrain` mapping, and then set id (box on the right, should be highlighted red) to `t_wall`.

Back in `Palette` window, you may notice the symbol now displays `t_wall` terrain. To the left of the symbol itself there are 2 other buttons: `Select as active for brush` and `Color on canvas`.

Click on `Select as active for brush`, then move your cursor to canvas and press LMB. Congrats, you placed a wall!

You can set custom color for the symbol in `Palette` window to make it stand out, or remove transparency to make it appear as clear tileset sprite.

You can press `Select as active for brush` button again to clear selection, or simply click with `MMB` outside canvas to pick "empty" tile and make your brush act as eraser. `MMB` works as "Pipette" tool, clicking on a wall with `MMB` will make your brush place walls.

Draw an enclosed square of walls. If you have misplaced a tile, you can always press `Ctrl+Z` to undo your mistake (or `Ctrl+Shift+Z` to undo your undo).

Now create a new entry with `t_floor` and use `Bucket` tool to fill the area inside walls with floor, same as you would do in graphical editors such as MSPaint.

It's time to place the door. Click `Duplicate entry` button for the floor entry, then open mappings list and add `AltFurniture` mapping. Choose a door you like, then plop it right in the middle of some wall.

Remember: you can undo with `Ctrl+Z` if you forgot to switch to Brush and accidentally replaced all walls with your door!

The project's going good, would be a shame if something happened to it. Press `Save` button in `Advanced Map Editor` window (shortcut: `Ctrl+S`), choose a folder, enter project name and click `Ok`.

Now, each house needs some furniture. Add a new entry with `t_floor` and `f_fridge`, then place it somewhere.

The fridge is useless without food, so we'll add some food. In mappings list for the fridge, add a new `Igroup` mapping. This will randomly place items from an existing item group. Now, set `chance (%)` as `80, 80`, uncheck `Once`, set `repeat` to `5, 5` and set `group_id` as `fridgesnacks`. You can see in `Palette` window that the fridge entry now has a little `+ 1` note on the right, hover over it and you'll see our itemgroup.

It's sometimes hard to keep track of stuff on canvas, but luckily you can press `Ctrl` and hover over symbols, and a little tooltip will show everything that'll be placed in that tile.

As a final touch, let's cover the walls with graffiti. From `Project overview`, click `Show/hide map objects` button to the left of the mapgen name. This will open `Objects list` window which lists all mapgen pieces that'll be placed over an area (`place_*` functions in JSON).

Add a new object of `Graffiti` type, set `x` as `1, 5`, `y` as `2, 3`, `repeat` as `5, 5` and then give it some color. Don't forget alpha channel, it's transparent by default!

You'll see a colorful blob on the canvas labeled `Graffiti: ""`. Adjust `x` and `y` to place it on some of your walls. If it becomes annoying later on, you can hide it by pressing the "eye" button for corresponding entry in `Objects list` window.

Having a row of identical graffities would look unnatural, so we'll use random text snippets. Click `Use snippet from category`, and in id prompt below enter `shelter_graffiti_snippets`.

With creative part done, it's time to see our changes in game. First, save your project, then click `Export` (or `Ctrl+E`). Choose a directory somewhere inside your mod, choose a name for your file and click `Ok`.

Close the `Advanced Map Editor` window. This will close the project, and let you back into editor title screen. Click `Exit To Desktop` to get back into the game, then quit to main menu.

Now:
1. Run your regular `cataclysm-tiles`
2. Create a world with your mod that contains the exported mapgen
3. Create a new character
4. From debug menu, open overmap editor and place `animalshelter_north` overmap terrain outside red zone
5. Long teleport to that overmap tile. Unless you're extremely unlucky, the game will use the new mapgen definition, and you'll be able to see your new building "in the flesh".

There are many plans to automate this part and improve both UI and functionality of the editor, see [TODO.md](TODO.md) for details.

This concludes the tutorial. If you found any bugs, or had trouble following this document, please open an issue or contact me on Discord (see [README.md](../../README.md) for details).
