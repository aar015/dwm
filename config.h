/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "q", "w", "e", "r", "u", "i", "o", "p" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[@]",	spiral},    /* first entry is default */
	{ "[M]",	monocle },
	{ NULL,	NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define ALTKEY Mod1Mask
#define SUPERKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ ALTKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ ALTKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ ALTKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ ALTKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[] = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ ALTKEY,			XK_x,		killclient,	{0} },
	{ ALTKEY,                       XK_j,		focusstack,	{.i = +1 } },
	{ ALTKEY,                       XK_k,		focusstack,	{.i = -1 } },
	{ ALTKEY,                       XK_h,		setmfact,	{.f = -0.01} },
	{ ALTKEY,                       XK_l,		setmfact,	{.f = +0.01} },
	{ ALTKEY|ShiftMask,		XK_h,		incnmaster,	{.i = +1 } },
	{ ALTKEY|ShiftMask,		XK_l,		incnmaster,	{.i = -1 } },
	{ ALTKEY,			XK_v,		togglebar,	{0} },
	{ ALTKEY,			XK_Tab,		cyclelayout,	{.i = +1 } },
	{ ALTKEY,                       XK_space,       focusmon,       {.i = +1 } },
	{ ALTKEY|ShiftMask,             XK_space,       tagmon,         {.i = +1 } },
	{ ALTKEY,                       XK_t,		view,		{.ui = ~0 } },
	{ ALTKEY|ShiftMask,		XK_t,		tag,		{.ui = ~0 } },
	{ SUPERKEY,			XK_F2,		quit,		{0} },
	TAGKEYS(			XK_q,				0)
	TAGKEYS(			XK_w,				1)
	TAGKEYS(			XK_e,				2)
	TAGKEYS(			XK_r,				3)
	TAGKEYS(			XK_u,				4)
	TAGKEYS(			XK_i,				5)
	TAGKEYS(			XK_o,				6)
	TAGKEYS(			XK_p,				7)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         ALTKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         ALTKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         ALTKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            ALTKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            ALTKEY,         Button3,        toggletag,      {0} },
};
