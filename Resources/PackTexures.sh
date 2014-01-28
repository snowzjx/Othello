#!/bin/sh

#  PackTexures.sh
#  Othello
#
#  Created by Snow on 1/28/14.
#

TP="/usr/local/bin/TexturePacker"
if [ "${ACTION}" = "clean" ]

then
echo "cleaning..."
rm ./1280p/sprite*
rm ./640p/sprite*
rm ./320p/sprite*

else
echo "building..."

${TP} --smart-update \
--format cocos2d \
--data 1280p/sprite.plist \
--sheet 1280p/sprite.pvr.ccz \
--dither-fs \
--opt RGB565 \
../OthelloArt/sprite/*.png

${TP} --smart-update \
--format cocos2d \
--data 640p/sprite.plist \
--sheet 640p/sprite.pvr.ccz \
--dither-fs \
--scale 0.5 \
--opt RGB565 \
../OthelloArt/sprite/*.png

${TP} --smart-update \
--format cocos2d \
--data 320p/sprite.plist \
--sheet 320p/sprite.pvr.ccz \
--dither-fs \
--scale 0.25 \
--opt RGB565 \
../OthelloArt/sprite/*.png

fi
exit 0
