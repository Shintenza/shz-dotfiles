### PATH ###
set -e fish_user_paths
set -U fish_user_paths $HOME/.local/bin $fish_user_paths 
set -U fish_user_paths $HOME/.nix-profile/bin $fish_user_paths 
set -Ua fish_user_paths $HOME/.cargo/bin

### EXPORTS ###
set fish_greeting 
set EDITOR "nvim"
set VISUAL "neovide"
set QT_QPA_PLATFORMTHEME "qt5ct"
set NIXPKGS_ALLOW_UNFREE 1

### BASIC COLORING ###
set fish_color_error -o red
set fish_color_normal white
set fish_color_command white
set fish_color_param white

### VI MODE ENABLED ###
function fish_user_key_bindings
  # fish_default_key_bindings
  fish_vi_key_bindings
end

### FLATPAK SUPPORT
set -l xdg_data_home $XDG_DATA_HOME ~/.local/share
set -gx --path XDG_DATA_DIRS $xdg_data_home[1]/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share

for flatpakdir in ~/.local/share/flatpak/exports/bin /var/lib/flatpak/exports/bin
    if test -d $flatpakdir
        contains $flatpakdir $PATH; or set -a PATH $flatpakdir
    end
end

### ALIASES ###
alias untar="tar -xvf"
alias vim="nvim --clean"
alias grubcfg="sudo grub-mkconfig -o /boot/grub/grub.cfg"
alias ls="exa -lah --icons"
alias cat="bat"
alias py="python"
alias j="autojump"
alias code="vscodium"
alias potato-enable="sudo tc qdisc add dev wlp3s0 root netem loss 0%"
alias potato-set="sudo tc qdisc change dev wlp3s0 root netem loss"
alias discord "discord --enable-features=UseOzonePlatform --ozone-platform=wayland"

### PROMPT ###
starship init fish | source

### JUMP ###
jump shell fish | source
fish_add_path /home/kamil/.spicetify
