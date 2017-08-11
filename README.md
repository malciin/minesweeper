# Minesweeper SFML
<img src="https://drive.google.com/uc?export=view&id=0B84Fb42SRsObbzJYRWFBSlJlOGs" />

## Info
* Using SFML v2.4.2
* Modifiable interface (user can cofig square size, margin between squares, color and font)
* Resizable window (when user resize window, map change its size too)
* Double click support
* All square textures are rendered once at runtime thus performance is fair due to use <b>sf::VertexArray</b> (even with 500^2 tiles)
* No recursion when reveal many empty tiles (stack overflow impossible on huge maps)
* MVC

## Controlls
> **Space** - generate map

> **Mouse wheel** - change next generated map mine number

> **CTRL + Mouse wheel** - change next generated map mine number by 25

> **CTRL + 1** - predefinied "Easy" difficulty

> **CTRL + 2** - predefined "Medium" difficulty

> **CTRL + 3** - predefined "Hard" difficulty

Project is still incomplete. All todos can be found <a href="https://github.com/malciin/minesweeper/projects/3">here</a>.

### License: MIT License