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
rm ./1280p/*
rm ./640p/*
rm ./320p/*

else
echo "building..."

${TP} --smart-update \
--format cocos2d \
--data 1280p/othelloscene.plist \
--sheet 1280p/othelloscene.pvr.ccz \
--dither-fs \
--opt RGB565 \
../OthelloArt/OthelloScene/*.png

${TP} --smart-update \
--format cocos2d \
--data 640p/othelloscene.plist \
--sheet 640p/othelloscene.pvr.ccz \
--dither-fs \
--scale 0.5 \
--opt RGB565 \
../OthelloArt/OthelloScene/*.png

${TP} --smart-update \
--format cocos2d \
--data 320p/othelloscene.plist \
--sheet 320p/othelloscene.pvr.ccz \
--dither-fs \
--scale 0.25 \
--opt RGB565 \
../OthelloArt/OthelloScene/*.png

fi
exit 0
