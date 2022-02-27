//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2021-2022 Graham Sanderson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//  all external data is defined here
//  most of the data is loaded into different structures at run time
//  some internal structures shared by many modules are here
//

#ifndef __DOOMDATA__
#define __DOOMDATA__

// The most basic types we use, portability.
#include "doomtype.h"

// Some global defines, that configure the game.
#include "doomdef.h"



//
// Map level types.
// The following data structures define the persistent format
// used in the lumps of the WAD files.
//

// Lump order in a map WAD: each map needs a couple of lumps
// to provide a complete scene geometry description.
enum
{
  ML_LABEL,		// A separator, name, ExMx or MAPxx
  ML_THINGS,		// Monsters, items..
  ML_LINEDEFS,		// LineDefs, from editing
  ML_SIDEDEFS,		// SideDefs, from editing
  ML_VERTEXES,		// Vertices, edited and BSP splits generated
  ML_SEGS,		// LineSegs, from LineDefs split by BSP
  ML_SSECTORS,		// SubSectors, list of LineSegs
  ML_NODES,		// BSP nodes
  ML_SECTORS,		// Sectors, from editing
  ML_REJECT,		// LUT, sector-sector visibility	
  ML_BLOCKMAP		// LUT, motion clipping, walls/grid element
};


// A single Vertex.
typedef PACKED_STRUCT (
{
  short		x;
  short		y;
}) mapvertex_t;


// A SideDef, defining the visual appearance of a wall,
// by setting textures and offsets.
typedef PACKED_STRUCT (
{
  short		textureoffset;
  short		rowoffset;
  char		toptexture[8];
  char		bottomtexture[8];
  char		midtexture[8];
  // Front sector, towards viewer.
  short		sector;
}) mapsidedef_t;



// A LineDef, as used for editing, and as input
// to the BSP builder.
typedef PACKED_STRUCT (
{
  short		v1;
  short		v2;
  short		flags;
  short		special;
  short		tag;
  // sidenum[1] will be -1 if one sided
  short		sidenum[2];		
}) maplinedef_t;


//
// LineDef attributes.
//

// Solid, is an obstacle.
#define ML_BLOCKING		1

// Blocks monsters only.
#define ML_BLOCKMONSTERS	2

// Backside will not be present at all
//  if not two sided.
#define ML_TWOSIDED		4

// If a texture is pegged, the texture will have
// the end exposed to air held constant at the
// top or bottom of the texture (stairs or pulled
// down things) and will move with a height change
// of one of the neighbor sectors.
// Unpegged textures allways have the first row of
// the texture at the top pixel of the line for both
// top and bottom textures (use next to windows).

// upper texture unpegged
#define ML_DONTPEGTOP		8

// lower texture unpegged
#define ML_DONTPEGBOTTOM	16	

// In AutoMap: don't map as two sided: IT'S A SECRET!
#define ML_SECRET		32

// Sound rendering: don't let sound cross two of these.
#define ML_SOUNDBLOCK		64

// Don't draw on the automap at all.
#define ML_DONTDRAW		128

// Set if already seen, thus drawn in automap.
#define ML_MAPPED		256

#if USE_WHD && WHD_SUPER_TINY
#define ML_NO_PREDICT_SIDE      256 // can share with ML_MAPPED
#define ML_NO_PREDICT_V1        512
#define ML_NO_PREDICT_V2        1024
#define ML_HAS_SPECIAL          2048
#define ML_HAS_TAG              4096
#define ML_SIDE_MASK            0xe000u
#endif
// Sector definition, from editing.
typedef	PACKED_STRUCT (
{
  short		floorheight;
  short		ceilingheight;
  char		floorpic[8];
  char		ceilingpic[8];
  short		lightlevel;
  short		special;
  short		tag;
}) mapsector_t;

// SubSector, as generated by BSP.
typedef PACKED_STRUCT (
{
  short		numsegs;
  // Index of first one, segs are stored sequentially.
  short		firstseg;	
}) mapsubsector_t;


// LineSeg, generated by splitting LineDefs
// using partition lines selected by BSP builder.
typedef PACKED_STRUCT (
{
  short		v1;
  short		v2;
  short		angle;		
  short		linedef;
  short		side;
  short		offset;
}) mapseg_t;



// BSP node structure.

// Indicate a leaf.
#define	NF_SUBSECTOR	0x8000

typedef PACKED_STRUCT (
{
  // Partition line from (x,y) to x+dx,y+dy)
  short		x;
  short		y;
  short		dx;
  short		dy;

  // Bounding box for each child,
  // clip against view frustum.
  short		bbox[2][4];

  // If NF_SUBSECTOR its a subsector,
  // else it's a node of another subtree.
  unsigned short	children[2];

}) mapnode_t;




// Thing definition, position, orientation and type,
// plus skill/visibility flags and attributes.
typedef PACKED_STRUCT (
{
    short		x;
    short		y;
    short		angle;
    short		type;
    short		options;
}) mapthing_t;

#if !WHD_SUPER_TINY
#define TAG_666 666
#define TAG_667 667
#else
// we only have one byte tags
#define TAG_666 254
#define TAG_667 255
#endif


#endif			// __DOOMDATA__
