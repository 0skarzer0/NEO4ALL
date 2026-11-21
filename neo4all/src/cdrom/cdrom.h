/**************************************
****    CDROM.H  -  File reading   ****
****         Header File           ****
**************************************/

#ifndef	CDROM_H
#define CDROM_H

#include <SDL.h>

/*-- Exported Variables -----------------------------------------------------*/
extern	int	neogeo_cdrom_current_drive;
extern SDL_Surface	*loading_pict;


/*-- cdrom.c functions ------------------------------------------------------*/
int	neogeo_cdrom_init1(void);
int	neogeo_cdrom_load_prg_file(char *, unsigned int);
int	neogeo_cdrom_load_z80_file(char *, unsigned int);
int	neogeo_cdrom_load_fix_file(char *, unsigned int);
int	neogeo_cdrom_load_spr_file(char *, unsigned int);
int	neogeo_cdrom_load_pcm_file(char *, unsigned int);
int	neogeo_cdrom_load_pat_file(char *, unsigned int, unsigned int);
int	neogeo_cdrom_process_ipl(int check);
void	neogeo_cdrom_shutdown(void);
void	neogeo_cdrom_load_title(void);

void	fix_conv(unsigned char *, unsigned char *, int, unsigned char *);
void	spr_conv(unsigned char *, unsigned char *, int, unsigned char *);


FILE *fopen_s(char *filename, char *mode);

/*-- extract8.asm functions -------------------------------------------------*/
void		extract8(char *, char *);
unsigned int	motorola_peek(unsigned char*);

/*-- sysdep functions --------------------------------------------------------*/
int getMountPoint(int drive, char *mountpoint);

#endif /* CDROM_H */
