void console_draw_background(int y, int h);
void console_text_pos(int x, int y, char * str);
void console_text(int x, int y, char * str);
void console_text_inv(int x, int y, char * str);
void console_text_centered(int y, char * str);
void console_text_num(int x, int y, int v);
void console_text_num_inv(int x, int y, int v);
void console_draw_window(int x, int y, int w, int h, char *title);
void console_draw_line(char *str);
void console_puts(char *str_orig);
void console_printf(const char *fmt, ...);
void console_init(void);
void console_draw_all_background(void);
void console_println(void);
void console_wait(void);
void console_pag(void);

extern int view_console;
