#!/bin/bash

#. $HOME/.zshrc
xsetroot -cursor_name left_ptr
feh --bg-fill $HOME/Pictures/wallpapers/onedark_wallpaper.png &
setxkbmap pl &
redshift -l 50.60705:22.10381 & 
/usr/bin/lxpolkit &
picom -b &
dwmblocks &
/usr/bin/dunst &
xfce4-power-manager &
wmname LG3D
