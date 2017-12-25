FishAnnotator                                                  {#mainpage}
============

Introduction
============

**FishAnnotator** is a standalone, cross-platform application for creating 
and visualizing video annotations.  It currently supports annotations in 
the form of bounding boxes, lines, and dots.  Annotations are saved in the 
widely supported JSON format, which can be imported separately for use in 
algorithms or other tools.  Species (annotation labels) can be specified by 
the user with the flexible graphical user interface.  It includes a single 
frame increment and decrement, and guarantees frame level accuracy for 
annotations, even after seeking.  Videos are decoded using FFMPEG, so a 
wide variety of codecs are supported; there is no need to convert your 
videos to imagery or other format before annotating.

FishAnnotator is maintained and supported by [CVision][CVision].

![fishannotatorscreenshot][Screenshot]

Getting started
===============

* [Read the manual][Manual]
* [Download the latest release][LatestRelease]
* [Build from source](doc/build.md)
* [Developer info](doc/developer.md)

[CVision]: http://www.cvisionconsulting.com
[Screenshot]: https://user-images.githubusercontent.com/7937658/34340866-b71f8fea-e959-11e7-9b02-03bf2b43d483.png
[Manual]: https://github.com/BGWoodward/FishAnnotator/releases/download/v0.2.1/Video.And.Image.Annotator.Manual.v0_2_1.pdf
[LatestRelease]: http://github.com/BGWoodward/FishAnnotator/releases
