# Night Vision Toggler

Hotkey-based gamma + contrast + digital vibrance toggle.

Requirements:
- MinGW-w64 (g++)
- NVIDIA GPU

Hotkeys:
- F3  = toggle ON/OFF
- F5  = Restore System Defaults
- F12 = exit (restores original gamma + DV)

Limitations
This is currently very early in production. Two key limitations are that contrast value does not perfectly line up with NVIDIA's control panel, and there's no way to have intermediate digital vibrance values using the method I am using. It's either off or on, so it can slam all the way to full vibrance or default, but not in between.
