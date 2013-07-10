#!/bin/sh
xcodebuild clean -project LostEngine2.xcodeproj/ -target LostEngine2 -configuration Debug
time xcodebuild -project LostEngine2.xcodeproj/ -target LostEngine2 -configuration Debug
