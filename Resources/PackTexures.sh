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
rm ./1536p/*.*
rm ./768p/*.*
rm ./320p/*.*

else
echo "building..."

${TP} --smart-update \
--format cocos2d \
--data 1536p/othelloscene.plist \
--sheet 1536p/othelloscene.pvr.ccz \
--dither-fs-alpha \
--opt RGBA4444 \
../OthelloArt/OthelloScene/*.png

${TP} --smart-update \
--format cocos2d \
--data 768p/othelloscene.plist \
--sheet 768p/othelloscene.pvr.ccz \
--dither-fs-alpha \
--scale 0.5 \
--opt RGBA4444 \
../OthelloArt/OthelloScene/*.png

${TP} --smart-update \
--format cocos2d \
--data 320p/othelloscene.plist \
--sheet 320p/othelloscene.pvr.ccz \
--dither-fs-alpha \
--scale 0.25 \
--opt RGBA4444 \
../OthelloArt/OthelloScene/*.png

fi
exit 0
