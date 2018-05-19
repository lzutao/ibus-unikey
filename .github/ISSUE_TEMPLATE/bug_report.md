---
name: Bug report
about: Create a report to help us improve

---

**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Issue description**:

Please complete the following information:

1. Which distribution and version?: (E.g. Fedora 27. Check `/etc/fedora-release`)
1. Which desktop environment and version?: (E.g. GNOME 3.24. Check `$XDG_CURRENT_DESKTOP` and your ISO image.)
1. Which session type?: (X11 or Wayland. Check `$XDG_SESSION_TYPE`)
1. Which application and version?: (E.g. gedit 3.24, leafpad, kate, xterm)
1. IBus version?: (Run `ibus version`)
1. `ibus-unikey` version?: (Open "IBus unikey preferences" dialog (`ibus-setup-unikey`) from IBus panel menu or `ibus-setup`, and select "About" tab)

*Optional info*:
1. Can you reproduce your problem when you restart `ibus-daemon`? (yes / no): (Run `ibus exit` and `ibus-daemon --xim &`)
1. Do you see any errors when you run `ibus-daemon` with the verbose option?: (Run `ibus-daemon --xim --verbose &` and look at the output when you encounter your problem.)
1. Can you reproduce your problem with a new user account instead of the current your account? (yes / no):

**Additional context**
Add any other context about the problem here.
