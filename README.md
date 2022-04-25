# wombatregistry
Stand Alone Version of the Registry Viewer found in Wombat Forensics

I needed a free linux based registry viewer and report generator, so I decoupled the registry viewer from wombat forensics.
The registry parsing is thanks to the [libregf library from Joachim Metz](https://github.com/libyal/libregf).
On top of using his library to parse the registry artifacts, I added the ability to tag individual values for inclusion in a report.
I also auto parse a few common registry keys/values and am happy to autoparse more upon request.

This tool is closer to yaru for it's display and I like the fine grained reporting better than other registry tools. Plus it runs on linux and is open source and free.

The libregf library is statically linked, so other than qt5, you shouldn't need any dependencies.

Screenshot of the main window

![Main Window](./Resources/mainwindow.png)
