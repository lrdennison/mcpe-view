# mcpe-view
Minecraft Pocket Edition Viewer (Linux &amp; C++)

## Purpose

I wanted to be able to view my Minecraft survival world in different
ways.  First to be able to fly around and see the terrain from a
height.  I also wanted to be able to spot where things like mob
spawners and melons were located.

## Status

Really rough, hard coded file paths, etc.  I'm able to load and view
the terrain.  Only a few popular block types are in.

## Process Basics

One needs to get the minecraft database off of the device.  For an iPad, I used iExplore.

A texture pack is also needed (instructions TBD).

## Code Prerequisites

You need to be to build an openGL application.

The Mojang version of leveldb.  It uses zlib for compression.
