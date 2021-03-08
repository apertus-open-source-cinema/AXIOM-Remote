# Imageformat

The converted icons (eg. home_icon.h) used in the AXIOM Remote are basically XBM files.
You can export/import XBM files in GIMP or use an online editor like https://xbm.jazzychad.net/ - the format with headers/structure is just slightly different but the actual image data is 100% XBM compatible.

```convert_xbm_to_h.sh``` script is used to convert the structure from *.xbm to the used *.h format.

Often images that are converted from SVG or other vector based sources need manual fine tuning to clean up edges.

# Attributions

See [Attributions.md](./Attributions.md) for details.
