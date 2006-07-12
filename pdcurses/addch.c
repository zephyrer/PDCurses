/************************************************************************ 
 * This file is part of PDCurses. PDCurses is public domain software;	*
 * you may use it for any purpose. This software is provided AS IS with	*
 * NO WARRANTY whatsoever.						*
 *									*
 * If you use PDCurses in an application, an acknowledgement would be	*
 * appreciated, but is not mandatory. If you make corrections or	*
 * enhancements to PDCurses, please forward them to the current		*
 * maintainer for the benefit of other users.				*
 *									*
 * No distribution of modified PDCurses code may be made under the name	*
 * "PDCurses", except by the current maintainer. (Although PDCurses is	*
 * public domain, the name is a trademark.)				*
 *									*
 * See the file maintain.er for details of the current maintainer.	*
 ************************************************************************/

#define	CURSES_LIBRARY 1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef addch
#undef waddch
#undef mvaddch
#undef mvwaddch
#undef echochar
#undef wechochar

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
# undef refresh
# undef wrefresh
# undef move
# undef wmove
#endif

RCSID("$Id: addch.c,v 1.21 2006/07/12 05:19:31 wmcbrine Exp $");

/*man-start**************************************************************

  Name:                                                         addch

  Synopsis:
	int addch(const chtype ch);
	int waddch(WINDOW *win, const chtype ch);
	int mvaddch(int y, int x, const chtype ch);
	int mvwaddch(WINDOW *win, int y, int x, const chtype ch);
	int echochar(const chtype ch);
	int wechochar(WINDOW *win, const chtype ch);

  X/Open Description:
	The routine addch() inserts the character ch into the default
	window at the current cursor position and the window cursor is
	advanced.  The character is of the type chtype as containing
	both data and attributes.

	The routine waddch() inserts the character ch into the specified
	window at the current cursor position.  The cursor position is
	advanced.

	The routine mvaddch() moves the cursor to the specified (y, x)
	position and inserts the character ch into the default window.
	The cursor position is advanced after the character has been
	inserted.

	The routine mvwaddch() moves the cursor to the specified (y, x)
	position and inserts the character ch into the specified
	window.  The cursor position is advanced after the character
	has been inserted.

	The routine echochar() inserts the character ch into stdscr
	at the current cursor position and a refresh() is called.  
	The cursor position is advanced.

	The routine wechochar() inserts the character ch into the
	specified window at the current cursor position and a wrefresh() 
	is called. The cursor position is advanced.

	All these routines are similar to putchar().  The following
	information applies to all the routines.

	If the cursor moves on to the right margin, an automatic
	newline is performed.  If scrollok is enabled, and a character
	is added to the bottom right corner of the screen, the
	scrolling region will be scrolled up one line.  If scrolling
	is not allowed, ERR will be returned.

	If ch is a tab, newline, or backspace, the cursor will be
	moved appropriately within the window.  If ch is a newline,
	the clrtoeol routine is called before the cursor is moved to
	the beginning of the next line.  If newline mapping is off,
	the cursor will be moved to the next line, but the x
	coordinate will be unchanged.  If ch is a tab the cursor is
	moved to the next tab position within the window.  If ch is
	another control character, it will be drawn in the ^X
	notation.  Calling the inch() routine after adding a control
	character returns the representation of the control character,
	not the control character.

	Video attributes can be combined with a character by ORing
	them into the parameter.  This will result in these attributes
	being set.  The intent here is that text, including
	attributes, can be copied from one place to another using inch()
	and addch().

	NOTE: All these functions are implemented as macros.

  PDCurses Description:
	Depending upon the state of the raw character output, 7- or
	8-bit characters will be output.

  X/Open Return Value:
	All functions return OK on success and ERR on error.

  X/Open Errors:
	No errors are defined for this function.

  Portability				     X/Open    BSD    SYS V
					     Dec '88
	addch					Y	Y	Y
	waddch					Y	Y	Y
	mvaddch					Y	Y	Y
	mvwaddch				Y	Y	Y
	echochar				-	-      3.0
	wechochar				-	-      3.0

**man-end****************************************************************/

int addch(const chtype ch)
{
	PDC_LOG(("addch() - called: ch=%x\n", ch));

	return waddch(stdscr, ch);
}

int waddch(WINDOW *win, const chtype ch)
{
	PDC_LOG(("waddch() - called: win=%x ch=%x\n", win, ch));

	return PDC_chadd(win, ch, (bool)(!(SP->raw_out)), TRUE);
}

int mvaddch(int y, int x, const chtype ch)
{
	PDC_LOG(("mvaddch() - called: y=%d x=%d ch=%x\n", y, x, ch));

	if (move(y,x) == ERR)
		return ERR;

	return waddch(stdscr, ch);
}

int mvwaddch(WINDOW *win, int y, int x, const chtype ch)
{
	PDC_LOG(("mvwaddch() - called: win=%x y=%d x=%d ch=%d\n",
		win, y, x, ch));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return waddch(win, ch);
}

int echochar(const chtype ch)
{
	PDC_LOG(("echochar() - called: ch=%x\n", ch));

	return wechochar(stdscr, ch);
}

int wechochar(WINDOW *win, const chtype ch)
{
	PDC_LOG(("wechochar() - called: win=%x ch=%x\n", win, ch));

	if (waddch(win, ch) == ERR)
		return ERR;

	return wrefresh(win);
}

#ifdef USE_WIDE
int add_wch(const cchar_t *wch)
{
	PDC_LOG(("add_wch() - called: wch=%x\n", *wch));

	return wadd_wch(stdscr, wch);
}

int wadd_wch(WINDOW *win, const cchar_t *wch)
{
	PDC_LOG(("wadd_wch() - called: win=%x wch=%x\n", win, *wch));

	return wch ? PDC_chadd(win, *wch, (bool)(!(SP->raw_out)), TRUE) : ERR;
}

int mvadd_wch(int y, int x, const cchar_t *wch)
{
	PDC_LOG(("mvaddch() - called: y=%d x=%d wch=%x\n", y, x, *wch));

	if (move(y,x) == ERR)
		return ERR;

	return wadd_wch(stdscr, wch);
}

int mvwadd_wch(WINDOW *win, int y, int x, const cchar_t *wch)
{
	PDC_LOG(("mvwaddch() - called: win=%x y=%d x=%d wch=%d\n",
		win, y, x, *wch));

	if (wmove(win, y, x) == ERR)
		return ERR;

	return wadd_wch(win, wch);
}

int echo_wchar(const cchar_t *wch)
{
	PDC_LOG(("echo_wchar() - called: wch=%x\n", *wch));

	return wecho_wchar(stdscr, wch);
}

int wecho_wchar(WINDOW *win, const cchar_t *wch)
{
	PDC_LOG(("wecho_wchar() - called: win=%x wch=%x\n", win, *wch));

	if (!wch || (wadd_wch(win, wch) == ERR))
		return ERR;

	return wrefresh(win);
}
#endif
