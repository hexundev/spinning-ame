//
//  GBA Memory map 
//
//! \file tonc_memmap.h
//! \author J Vijn
//! \date 20060508 - 20060508
// 
// === NOTES ===
// * This is a _small_ set of typedefs, #defines and inlines that can 
//   be found in tonclib, and might not represent the 
//   final forms.

#ifndef __MEMMAP__
#define __MEMMAP__


// === MEMORY SECTIONS ================================================

// basic sections
#define MEM_IO		0x04000000
#define MEM_PAL		0x05000000	// no 8bit write !!
#define MEM_VRAM	0x06000000	// no 8bit write !!
#define MEM_OAM		0x07000000	// no 8bit write !!

// basic sizes
#define PAL_SIZE	0x00400
#define VRAM_SIZE	0x18000
#define OAM_SIZE	0x00400

// sub sizes
#define PAL_BG_SIZE		0x00200
#define PAL_OBJ_SIZE	0x00200
#define VRAM_BG_SIZE	0x10000
#define VRAM_OBJ_SIZE	0x08000
#define M3_SIZE			0x12C00
#define M4_SIZE			0x09600
#define M5_SIZE			0x0A000
#define VRAM_PAGE_SIZE	0x0A000

// sub sections
#define REG_BASE	MEM_IO

#define MEM_PAL_OBJ		(MEM_PAL + PAL_BG_SIZE)
#define MEM_VRAM_BACK	(MEM_VRAM+ VRAM_PAGE_SIZE)
#define MEM_VRAM_OBJ	(MEM_VRAM+ VRAM_BG_SIZE)


// === STRUCTURED MEMORY MAP ==========================================
// Heavily typedefed, watch your pointers
// Should simplify memory accesses


// --- PAL ---
// pal_bg_mem[y] = COLOR (color y)
#define pal_bg_mem		((COLOR*)MEM_PAL)
#define pal_obj_mem		((COLOR*)MEM_PAL_OBJ)
#define pal_sprite_mem pal_obj_mem

// pal_bg_bank[y] =	COLOR[] (bank y)
// pal_bg_bank[y][x] = COLOR (bank y, color x : color y*16+x)
#define pal_bg_bank		((PALBANK*)MEM_PAL)
#define pal_obj_bank	((PALBANK*)MEM_PAL_OBJ)


// --- VRAM ---
// tile_mem[y] = TILE[]   (char block y)
// tile_mem[y][x] = TILE (char block y, tile x)
#define tile_mem		( (CHARBLOCK*)MEM_VRAM)
#define tile8_mem		((CHARBLOCK8*)MEM_VRAM)

#define tile_mem_obj	( (CHARBLOCK*)MEM_VRAM_OBJ)
#define tile8_mem_obj	((CHARBLOCK8*)MEM_VRAM_OBJ)

// se_mem[y] = SB_ENTRY[] (screen block y)
// se_mem[y][x] = screen entry (screen block y, screen entry x)
#define se_mem			((SCREENBLOCK*)MEM_VRAM)

// vid_mem[a] = COLOR
#define vid_mem			((COLOR*)MEM_VRAM)


// --- OAM ---
// oatr_mem[a] = OBJ_ATTR (oam entry a)
#define oam_mem			((OBJ_ATTR*)MEM_OAM)
#define obj_mem			((OBJ_ATTR*)MEM_OAM)
#define obj_aff_mem		((OBJ_AFFINE*)MEM_OAM)


// === REGISTER LIST ==================================================


// === VIDEO REGISTERS ===
#define REG_DISPCNT			*(vu32*)(REG_BASE+0x0000)	// display control
#define REG_DISPSTAT		*(vu16*)(REG_BASE+0x0004)	// display interupt status
#define REG_VCOUNT			*(vu16*)(REG_BASE+0x0006)	// vertical count
#define REG_MOSAIC 			*(vu16*)(REG_BASE+0x004C)

// === KEYPAD ===
#define REG_KEYINPUT		*(vu16*)(REG_BASE+0x0130)	// Key status
#define REG_KEYCNT			*(vu16*)(REG_BASE+0x0132)

// --- background ---
#define REG_BLDCNT 			*(vu16*)(REG_BASE+0x0050)	
#define REG_BLDY 			*(vu16*)(REG_BASE+0x0054)	
#define REG_BG0CNT			*(vu16*)(REG_BASE+0x0008)	// bg 0-3 control
#define REG_BG1CNT			*(vu16*)(REG_BASE+0x000A)
#define REG_BG2CNT			*(vu16*)(REG_BASE+0x000C)
#define REG_BG3CNT			*(vu16*)(REG_BASE+0x000E)

#define REG_BG0HOFS			*(vu16*)(REG_BASE+0x0010)		// bg 0-3 origins
#define REG_BG0VOFS			*(vu16*)(REG_BASE+0x0012)
#define REG_BG1HOFS			*(vu16*)(REG_BASE+0x0014)
#define REG_BG1VOFS			*(vu16*)(REG_BASE+0x0016)
#define REG_BG2HOFS			*(vu16*)(REG_BASE+0x0018)
#define REG_BG2VOFS			*(vu16*)(REG_BASE+0x001A)
#define REG_BG3HOFS			*(vu16*)(REG_BASE+0x001C)
#define REG_BG3VOFS			*(vu16*)(REG_BASE+0x001E)

// --- REG_BGxCNT ---

#define BG_MOSAIC			0x0040	//!< Enable Mosaic
#define BG_4BPP					 0	//!< 4bpp (16 color) bg (no effect on affine bg)
#define BG_8BPP				0x0080	//!< 8bpp (256 color) bg (no effect on affine bg)
#define BG_WRAP				0x2000	//!< Wrap around edges of affine bgs
#define BG_SIZE0				 0
#define BG_SIZE1			0x4000
#define BG_SIZE2			0x8000
#define BG_SIZE3			0xC000
#define BG_REG_32x32			 0	//!< reg bg, 32x32 (256x256 px)
#define BG_REG_64x32		0x4000	//!< reg bg, 64x32 (512x256 px)
#define BG_REG_32x64		0x8000	//!< reg bg, 32x64 (256x512 px)
#define BG_REG_64x64		0xC000	//!< reg bg, 64x64 (512x512 px)
#define BG_AFF_16x16			 0	//!< affine bg, 16x16 (128x128 px)
#define BG_AFF_32x32		0x4000	//!< affine bg, 32x32 (256x256 px)
#define BG_AFF_64x64		0x8000	//!< affine bg, 64x64 (512x512 px)
#define BG_AFF_128x128		0xC000	//!< affine bg, 128x128 (1024x1024 px)

#define BG_PRIO_MASK	0x0003
#define BG_PRIO_SHIFT		0
#define BG_PRIO(n)		((n)<<BG_PRIO_SHIFT)

#define BG_CBB_MASK		0x000C
#define BG_CBB_SHIFT		 2
#define BG_CBB(n)		((n)<<BG_CBB_SHIFT)

#define BG_SBB_MASK		0x1F00
#define BG_SBB_SHIFT		 8
#define BG_SBB(n)		((n)<<BG_SBB_SHIFT)

#define BG_SIZE_MASK	0xC000
#define BG_SIZE_SHIFT		14
#define BG_SIZE(n)		((n)<<BG_SIZE_SHIFT)


#define BG_BUILD(cbb, sbb, size, bpp, prio, mos, wrap)		\
(															\
	   ((size)<<14)  | (((wrap)&1)<<13) | (((sbb)&31)<<8	\
	| (((bpp)&8)<<4) | (((mos)&1)<<6)   | (((cbb)&3)<<2)	\
	| ((prio)&3)											\
)

#endif // __MEMMAP__
