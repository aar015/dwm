/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int gappx     = 8;	/* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=14";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	NULL,       NULL,       0,            1,           -1 	   },
	{ "Firefox",  	NULL,       NULL,       1 << 0,       0,            0	   },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static int attachbelow = 1;    /* 1 means attach below */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[@]",	spiral},    /* first entry is default */
	{ "[C]",	centeredmaster},
	{ "[M]",	monocle },
	{ NULL,	NULL },    /* no layout function means floating behavior */
};

/* Functions to switch monitor and tag */
void focusonmon(const Arg *arg)
{
	Monitor *m;

	if (!mons->next)
		return;

	m = mons;
	for(int index = 0; m->next && index < arg->i; index++)
		m = m->next;

	if (m == selmon)
		return;

	unfocus(selmon->sel, 0);
	selmon = m;
	focus(NULL);
}
void viewmontag(const Arg *arg)
{
	Arg a = {.i=((int **)arg->v)[0]};
	focusonmon(&a);
	Arg b = {.ui=((int **)arg->v)[1]};
	view(&b);
}
void toggleviewmontag(const Arg *arg)
{
	Arg a = {.i=((int **)arg->v)[0]};
	focusonmon(&a);
	Arg b = {.ui=((int **)arg->v)[1]};
	toggleview(&b);
}
void movetagtomon(const Arg *arg)
{
	int diff = arg->i - selmon->num;
	if(diff == 0)
		return;
	Arg a = {.i=diff};
	tagmon(&a);
}
void tagmontag(const Arg *arg)
{
	Arg a = {.i=((int **)arg->v)[0]};
	movetagtomon(&a);
	focusonmon(&a);
	Arg b = {.ui=((int **)arg->v)[1]};
	tag(&b);
}
void tagmonredux(const Arg *arg)
{
	tagmon(arg);
	focusmon(arg);
	arrange(selmon);
}
/* key definitions */
#define ALTKEY Mod1Mask
#define SUPERKEY Mod4Mask
#define TAGKEYS(KEY,MON,TAG) \
	{ ALTKEY,                       KEY,      viewmontag,		{.v=(const int*[]){MON, 1<<TAG}}}, \
	{ ALTKEY|ShiftMask,             KEY,      tagmontag,     	{.v=(const int*[]){MON, 1<<TAG}}}, \
	{ ALTKEY|ControlMask,           KEY,      toggleviewmontag,	{.v=(const int*[]){MON, 1<<TAG}}}, \
	{ ALTKEY|ControlMask|ShiftMask, KEY,      toggletag,		{.ui = 1<<TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[] = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        	function        	argument 	*/
	{ ALTKEY,			XK_x,		killclient,		{0}		},
	{ ALTKEY,			XK_a,		spawn,			{.v=dmenucmd}	},
	{ ALTKEY,                       XK_j,		focusstack,		{.i = +1}	},
	{ ALTKEY|ShiftMask,		XK_j,		movestack,		{.i = +1} 	},
	{ ALTKEY,                       XK_k,		focusstack,		{.i = -1} 	},
	{ ALTKEY|ShiftMask,		XK_k,		movestack,		{.i = -1} 	},
	{ ALTKEY,                       XK_h,		setmfact,		{.f = -0.01}	},
	{ ALTKEY|ShiftMask,		XK_h,		incnmaster,		{.i = +1} 	},
	{ ALTKEY,                       XK_l,		setmfact,		{.f = +0.01} 	},
	{ ALTKEY|ShiftMask,		XK_l,		incnmaster,		{.i = -1}	},
	{ ALTKEY,			XK_v,		togglebar,		{0} 		},
	{ ALTKEY,			XK_Tab,		cyclelayout,		{.i = +1} 	},
	{ ALTKEY,                       XK_space,       focusmon,       	{.i = +1} 	},
	{ ALTKEY|ShiftMask,             XK_space,       tagmonredux,         	{.i = +1} 	},
	{ ALTKEY,                       XK_t,		view,			{.ui = ~0} 	},
	{ ALTKEY|ShiftMask,		XK_t,		tag,			{.ui = ~0} 	},
	{ ALTKEY|ShiftMask,		XK_Tab,		toggleAttachBelow, 	{0}		},
	{ SUPERKEY,			XK_F2,		quit,			{0} 		},
	{ SUPERKEY,                     XK_F5,     	xrdb,           	{.v = NULL } 	},
	/*				KEY		MON			TAG		*/
	TAGKEYS(			XK_q,		1,			0)
	TAGKEYS(			XK_w,		1,			1)
	TAGKEYS(			XK_e,		1,			2)
	TAGKEYS(			XK_r,		1,			3)
	TAGKEYS(			XK_t,		1,			4)
	TAGKEYS(			XK_y,		0,			0)
	TAGKEYS(			XK_u,		0,			1)
	TAGKEYS(			XK_i,		0,			2)
	TAGKEYS(			XK_o,		0,			3)
	TAGKEYS(			XK_p,		0,			4)
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
