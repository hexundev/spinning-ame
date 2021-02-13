
#include <string.h>

#include "toolbox.h"

#include "palette.h"
#include "tonc_font.h"
#include "ame2.h"
#include "amebg.h"

static s16 randomPositions[] = {
	50, 360, 163, 29, 411, 447, 327, 90, 214, 121, 343, 186, 174, 197,
	18, 172, 255, 67, 472, 6, 505, 494, 46, 99, 352, 318, 351, 316, 252,
	464, 260, 152, 491, 220, 55, 111, 417, 184, 232, 416, 79, 142, 425, 380,
	59, 234, 341, 110, 463, 328, 121, 401, 84, 130, 351, 462, 327, 1, 218,
	459, 230, 345, 462, 179};

typedef struct AmeSprite
{
	u16 x;
	u16 y;
	int sx;
	int sy;
	int vx;
	int vy;
	u16 rot;
} AmeSprite;

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *)obj_buffer;
AmeSprite ameSprites[32];
u16 textEntries[1024];

void drawText(const char *str, int x, int y, int screenblock)
{
	memset(textEntries, 0, sizeof(textEntries));

	int len = strlen(str);
	int startX = x;
	for (int i = 0; i < len; ++i)
	{
		char c = str[i];
		switch (c)
		{
		case '\n':
			y++;
			x = startX;
			continue;
		case '\t':
			x += 4;
			continue;
		default:
			break;
		}

		u16 charTile = (int)c - 32;

		textEntries[(x % 32) + (y % 32) * 32] = (charTile & 0x3FF) | (1 << 12);
		x++;
	}

	memcpy(&se_mem[screenblock][0], textEntries, sizeof(textEntries));
}

s16 getVel(u16 v)
{
	s16 vel = (s16)v >> 6;
	if (vel < 1)
	{
		vel = 1;
	}
	if (vel > 3)
	{
		vel = 3;
	}
	return vel;
}

void initAme()
{
	AmeSprite *ame;
	int s;
	s16 dx, dy;
	for (int i = 0; i < 32; ++i)
	{
		ame = &ameSprites[i];
		ame->x = randomPositions[i];
		ame->y = randomPositions[i + 32];

		dx = (ame->x % 2 == 0 ? -1 : 1);
		dy = (ame->y % 2 == 0 ? -1 : 1);

		ame->vx = getVel(ame->x) * dx;
		ame->vy = getVel(ame->y) * dy;

		ame->rot = ame->x;
		s = 256; //256 - (ame->x >> 4);
		ame->sx = dx * s;
		ame->sy = dy * s;
	}
}

int main()
{
	memcpy(pal_bg_mem, palette, paletteLen);
	memcpy(pal_sprite_mem, palette, paletteLen);

	memcpy(&tile_mem[0][0], tonc_fontTiles, tonc_fontTilesLen);
	memcpy(&tile_mem[1][0], amebgTiles, amebgTilesLen);
	memcpy(&tile_mem[4][0], ame2Tiles, ame2TilesLen);
	memcpy(&se_mem[24][0], amebgMap, amebgMapLen);

	REG_BG0CNT = BG_CBB(0) | BG_SBB(31) | BG_4BPP | BG_REG_32x32 | BG_MOSAIC | BG_PRIO(0);
	REG_BG1CNT = BG_CBB(1) | BG_SBB(24) | BG_4BPP | BG_REG_32x32 | BG_MOSAIC | BG_PRIO(3);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;

	initAme();
	drawText("@hexundev 2021", 8, 15, 31);
	oam_init(obj_buffer, 128);

	AmeSprite *ame;
	OBJ_ATTR *obj;
	OBJ_AFFINE *affine;

	int bgx = 0;
	int frame = 0;
	int animTime = 0;
	u16 rot = 0;

	while (1)
	{
		vid_vsync();

		animTime++;
		bgx += 4;
		rot += 256;

		frame = ((animTime >> 4) % 4) * 16;
		REG_BG1HOFS = bgx >> 2;

		for (int i = 0; i < 32; ++i)
		{
			ame = &ameSprites[i];
			obj = &obj_buffer[i];
			affine = &obj_aff_buffer[i];

			obj_aff_identity(affine);

			// Big Ame
			if (i == 31)
			{
				obj_set_attr(obj,
							 ATTR0_SQUARE | ATTR0_AFF | ATTR0_AFF_DBL,
							 ATTR1_SIZE_32 | ATTR1_AFF_ID(i),
							 ATTR2_PALBANK(6) | frame);

				obj_set_pos(obj, 88, 48);
				obj_aff_rotate(affine, rot + i * (ame->rot), 0x080, 0x080);
			}
			// Smaller Ames
			else
			{
				obj_set_attr(obj,
							 ATTR0_SQUARE | ATTR0_AFF,
							 ATTR1_SIZE_32 | ATTR1_AFF_ID(i),
							 ATTR2_PALBANK(6) | frame);

				ame->x += ame->vx;
				ame->y += ame->vy;

				obj_set_pos(obj, ame->x, ame->y);
				obj_aff_rotate(affine, rot + i * (ame->rot), ame->sx, ame->sy);
			}
		}

		oam_copy(oam_mem, obj_buffer, 128);
	}
	return 0;
}